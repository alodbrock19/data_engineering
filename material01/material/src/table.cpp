#include "table.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

Table::Table(const std::string& path) {
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("Cannot open " + path + ". Run the program from the project folder.");

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> fields;
        std::stringstream stream(line);
        std::string field;
        while (std::getline(stream, field, '|'))
            fields.push_back(field);

        LineItem row;
        row.l_orderkey = std::stoi(fields[0]);
        row.l_extendedprice = std::stod(fields[5]);
        row.l_discount = std::stod(fields[6]);
        row.l_returnflag = fields[8][0];
        rows_.push_back(row);
    }
}

std::size_t Table::size() const {
    return rows_.size();
}

const LineItem& Table::row(std::size_t i) const {
    return rows_[i];
}

const std::vector<LineItem>& Table::rows() const {
    return rows_;
}
