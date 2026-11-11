#include <charconv>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
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
            sum += (order.orderpriority == "1-URGENT" && lineitem.orderkey == order.orderkey) ? lineitem.quantity : 0;
        }
    }
    return sum;
}

// Exercise 2: use a hash table instead of the inner loop.
double computeQuantityHashJoin(const std::vector<Lineitem>& lineitems, const std::vector<Order>& orders) {
    double sum = 0;
    // TODO (Exercise 2)
    (void)lineitems;
    (void)orders;
    return sum;
}

// Exercise 3: helpers that work directly on the mapped bytes.

std::string_view nextField(const char* data, std::size_t& pos) {
    // TODO (Exercise 3)
    (void)data;
    (void)pos;
    return {};
}

void skipLine(const char* data, std::size_t size, std::size_t& pos) {
    // TODO (Exercise 3)
    (void)data;
}

// Parses the leading digits of a field, e.g. "17" or "17.00" both give 17.
int toInt(std::string_view field) {
    int value = 0;
    std::from_chars(field.data(), field.data() + field.size(), value);
    return value;
}

// Exercise 3: the hash join, reading the files through mmap and parsing only the needed columns.
double computeQuantityMapped(const std::string& directory) {
    // TODO (Exercise 3)
    (void)directory;
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <data directory> <nested|hash|mmap>\n"
                  << "Example: " << argv[0] << " tpch/sf0_01 nested\n";
        return 1;
    }
    const std::string directory = argv[1];
    const std::string variant = argv[2];

    double result = 0;
    if (variant == "mmap") {
        ScopedTimer timer("total");
        result = computeQuantityMapped(directory);
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
        std::cerr << "Unknown variant: " << variant << " (use nested, hash or mmap)\n";
        return 1;
    }

    std::cout << std::fixed << std::setprecision(0) << result << std::endl;
    return 0;
}
