#include <bit>
#include <charconv>
#include <cstring>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "file_reader.hpp"
#include "mapped_file.hpp"
#include "timer.hpp"
#include "tpch.hpp"

std::vector<Lineitem> readLineitem(const std::string& path) {
    FileReader reader(path);
    return reader.readAll<Lineitem>();
}

std::vector<Order> readOrders(const std::string& path) {
    FileReader reader(path);
    return reader.readAll<Order>();
}

// Exercise 2: compare every line item with every order.
double computeQuantityNestedLoop(const std::vector<Lineitem>& lineitems, const std::vector<Order>& orders) {
    double sum = 0;
    for (const Lineitem& lineitem : lineitems) {
        for (const Order& order : orders) {
            if (lineitem.orderkey == order.orderkey && order.orderpriority == "1-URGENT") {
                sum += lineitem.quantity;
            }
        }
    }
    return sum;
}

// Exercise 2: remember the urgent orders in a hash table, then look each line item up.
double computeQuantityHashJoin(const std::vector<Lineitem>& lineitems, const std::vector<Order>& orders) {
    //std::unordered_set<int> urgentOrders;
    std::unordered_map<int, Order> orderMap;

    for (const Order& order : orders) {
        if (order.orderpriority == "1-URGENT") {
            orderMap.insert({order.orderkey, order});
        }
        //orderMap[order.orderkey] = order;
    }
    
    double sum = 0;
    for (const Lineitem& lineitem : lineitems) {
        if (orderMap.find(lineitem.orderkey) != orderMap.end()) {
            sum += lineitem.quantity;
        }
    }
    return sum;
}

// Exercise 3: helpers that work directly on the mapped bytes.

// Sheet 4, Exercise 1: returns the position of the first c at or after pos
// (or size if there is none), looking at 8 bytes at a time.
[[gnu::noinline]] std::size_t findCharSwar(const char* data, std::size_t size, std::size_t pos, char c) {
    // TODO (Exercise 1): replace this byte-by-byte loop with the SWAR trick from the exercise sheet.
    while (pos < size && data[pos] != c) {
        ++pos;
    }
    return pos;
}

// Returns the position of the first c at or after pos (or size), one byte at a time.
[[gnu::noinline]] std::size_t findCharBytewise(const char* data, std::size_t size, std::size_t pos, char c) {
    while (pos < size && data[pos] != c) {
        ++pos;
    }
    return pos;
}

template <bool UseSwar>
std::size_t findChar(const char* data, std::size_t size, std::size_t pos, char c) {
    if constexpr (UseSwar) {
        return findCharSwar(data, size, pos, c);
    } else {
        return findCharBytewise(data, size, pos, c);
    }
}

// Returns the field that starts at pos and moves pos behind the next '|'.
template <bool UseSwar>
[[gnu::noinline]] std::string_view nextField(const char* data, std::size_t size, std::size_t& pos) {
    std::size_t end = findChar<UseSwar>(data, size, pos, '|');
    std::string_view field{data + pos, end - pos};
    pos = end + 1;
    return field;
}

// Moves pos to the start of the next line.
template <bool UseSwar>
[[gnu::noinline]] void skipLine(const char* data, std::size_t size, std::size_t& pos) {
    pos = findChar<UseSwar>(data, size, pos, '\n') + 1;
}

// Parses the leading digits of a field, e.g. "17" or "17.00" both give 17.
[[gnu::noinline]] int toInt(std::string_view field) {
    int value = 0;
    std::from_chars(field.data(), field.data() + field.size(), value);
    return value;
}

// Build side: the keys of all urgent orders.
template <bool UseSwar>
std::unordered_set<int> buildUrgentOrders(const std::string& directory) {
    std::unordered_set<int> urgentOrders;
    MappedFile file(directory + "/orders.tbl");
    const char* data = file.data();
    std::size_t size = file.size();
    std::size_t pos = 0;
    while (pos < size) {
        int orderkey = toInt(nextField<UseSwar>(data, size, pos));   // column 1: orderkey
        for (int i = 2; i <= 5; ++i) {
            nextField<UseSwar>(data, size, pos);                     // columns 2-5: not needed
        }
        if (nextField<UseSwar>(data, size, pos) == "1-URGENT") {     // column 6: orderpriority
            urgentOrders.insert(orderkey);
        }
        skipLine<UseSwar>(data, size, pos);
    }
    return urgentOrders;
}

// Probe side: adds up the quantities of the urgent line items whose line starts in [begin, end).
// A line that starts before end is processed completely, even if it continues behind end.
template <bool UseSwar>
std::int64_t probeChunk(const char* data, std::size_t size, std::size_t begin, std::size_t end,
                        const std::unordered_set<int>& urgentOrders) {
    std::size_t pos = begin;
    // TODO (Exercise 2): begin is usually in the middle of a line. Move pos so that every line
    //   is processed by exactly one chunk.

    std::int64_t sum = 0;   // local sum: the threads do not share it
    while (pos < end) {
        int orderkey = toInt(nextField<UseSwar>(data, size, pos));   // column 1: orderkey
        for (int i = 2; i <= 4; ++i) {
            nextField<UseSwar>(data, size, pos);                     // columns 2-4: not needed
        }
        int quantity = toInt(nextField<UseSwar>(data, size, pos));   // column 5: quantity
        if (urgentOrders.contains(orderkey)) {
            sum += quantity;
        }
        skipLine<UseSwar>(data, size, pos);
    }
    return sum;
}

// Sheet 3, Exercise 3 (byte by byte), and Sheet 4, Exercise 1 (with SWAR).
template <bool UseSwar>
double computeQuantityMapped(const std::string& directory) {
    std::unordered_set<int> urgentOrders = buildUrgentOrders<UseSwar>(directory);
    MappedFile file(directory + "/lineitem.tbl");
    return static_cast<double>(probeChunk<UseSwar>(file.data(), file.size(), 0, file.size(), urgentOrders));
}

// Sheet 4, Exercise 2: build single-threaded, probe with several threads.
double computeQuantityParallel(const std::string& directory, unsigned numThreads) {
    std::unordered_set<int> urgentOrders;
    {
        ScopedTimer timer("build");
        urgentOrders = buildUrgentOrders<true>(directory);
    }

    ScopedTimer timer("probe");
    MappedFile file(directory + "/lineitem.tbl");
    const char* data = file.data();
    std::size_t size = file.size();

    std::vector<std::int64_t> results(numThreads, 0);
    std::vector<std::thread> threads;
    // TODO (Exercise 2): split [0, size) into numThreads chunks of about the same size, and start one
    //   thread per chunk that stores probeChunk<true>(...) of its chunk in results[i].
    //   Then wait for all threads to finish with join().
    (void)data;
    (void)size;
    return static_cast<double>(std::accumulate(results.begin(), results.end(), std::int64_t{0}));
}

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        std::cerr << "Usage: " << argv[0] << " <data directory> <nested|hash|mmap|swar|parallel> [threads]\n"
                  << "Example: " << argv[0] << " tpch/sf0_01 parallel 4\n";
        return 1;
    }
    const std::string directory = argv[1];
    const std::string variant = argv[2];
    unsigned numThreads = std::thread::hardware_concurrency();
    if (argc == 4) {
        numThreads = static_cast<unsigned>(std::stoul(argv[3]));
    }
    if (numThreads == 0) {
        numThreads = 1;
    }

    double result = 0;
    if (variant == "mmap") {
        ScopedTimer timer("total");
        result = computeQuantityMapped<false>(directory);
    } else if (variant == "swar") {
        ScopedTimer timer("total");
        result = computeQuantityMapped<true>(directory);
    } else if (variant == "parallel") {
        std::cerr << "threads: " << numThreads << "\n";
        ScopedTimer timer("total");
        result = computeQuantityParallel(directory, numThreads);
    } else if (variant == "nested" || variant == "hash") {
        std::vector<Lineitem> lineitems;
        std::vector<Order> orders;
        {
            ScopedTimer timer("load");
            lineitems = readLineitem(directory + "/lineitem.tbl");
            orders = readOrders(directory + "/orders.tbl");
        }
        {
            ScopedTimer timer("query");
            result = (variant == "nested") ? computeQuantityNestedLoop(lineitems, orders)
                                           : computeQuantityHashJoin(lineitems, orders);
        }
    } else {
        std::cerr << "Unknown variant: " << variant << " (use nested, hash, mmap, swar or parallel)\n";
        return 1;
    }

    std::cout << std::fixed << std::setprecision(0) << result << std::endl;
    return 0;
}
