#include <array>
#include <cstddef>
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

#include "lineitem.hpp"
#include "scoped_timer.hpp"
#include "table.hpp"

void exercise3(const Table& table) {
    for (std::size_t i = 0; i < 3; ++i) {
        const LineItem& r = table.row(i);
        std::cout << r.l_orderkey << " | " << r.l_extendedprice << " | "
                  << r.l_discount << " | " << r.l_returnflag << '\n';
    }

    // TODO 3.5: print sizeof(LineItem) and the sum of the sizes of its fields
}

// TODO 7.1: replace both total_revenue functions with one template
// TODO 4.1: pass rows as const std::vector<LineItem>&
double total_revenue(std::vector<LineItem> rows) {
    // TODO 4.3
    double sum = 0.0;
    for (const LineItem& r : rows)
        sum += r.l_extendedprice * (1.0 - r.l_discount);
    return sum;
}

double total_revenue(const std::list<LineItem>& rows) {
    double sum = 0.0;
    for (const LineItem& r : rows)
        sum += r.l_extendedprice * (1.0 - r.l_discount);
    return sum;
}

void exercise4(const Table& table) {
    ScopedTimer timer("total_revenue, 100 calls (vector)");
    double revenue = 0.0;
    for (int i = 0; i < 100; ++i)
        revenue += total_revenue(table.rows());
    std::cout << "Revenue: " << revenue / 100 << '\n';
}

void exercise5(const Table& table) {
    // TODO 5.2: uncomment the next four lines
    // std::array<LineItem, 600'000> rows;
    // for (std::size_t i = 0; i < rows.size(); ++i)
    //     rows[i] = table.row(i % table.size());
    // std::cout << rows[table.size() / 2].l_orderkey << '\n';

    std::cout << "sizeof(std::vector<LineItem>): " << sizeof(std::vector<LineItem>) << '\n';
}

void exercise7(const Table& table) {
    std::list<LineItem> list(table.rows().begin(), table.rows().end());
    ScopedTimer timer("total_revenue, 100 calls (list)");
    double revenue = 0.0;
    for (int i = 0; i < 100; ++i)
        revenue += total_revenue(list);
    std::cout << "Revenue: " << revenue / 100 << '\n';
}

void exercise8(const Table& table) {
    std::vector<int> keys;
    for (std::size_t i = 0; i < 1000; ++i)
        keys.push_back(table.row(i * table.size() / 1000).l_orderkey);

    {
        ScopedTimer timer("1,000 scans");
        std::size_t found = 0;
        for (int key : keys)
            for (std::size_t i = 0; i < table.size(); ++i)
                if (table.row(i).l_orderkey == key)
                    ++found;
        std::cout << "Found " << found << " rows\n";
    }

    std::unordered_map<int, std::vector<std::size_t>> by_order;
    {
        ScopedTimer timer("Building the index");
        for (std::size_t i = 0; i < table.size(); ++i) {
            // TODO 8.3: record row i under its order key
        }
    }
    {
        ScopedTimer timer("1,000 lookups");
        std::size_t found = 0;
        for (int key : keys)
            found += by_order[key].size();
        std::cout << "Found " << found << " rows\n";
    }
}

int main() {
    Table table("data/lineitem.tbl");
    std::cout << "Loaded " << table.size() << " rows\n";

    exercise3(table);
    exercise4(table);
    exercise5(table);
    exercise7(table);
    exercise8(table);
}
