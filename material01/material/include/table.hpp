#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "lineitem.hpp"

class Table {
public:
    explicit Table(const std::string& path);

    std::size_t size() const;
    const LineItem& row(std::size_t i) const;
    const std::vector<LineItem>& rows() const;

private:
    std::vector<LineItem> rows_;
};
