#ifndef TPCH_JOIN_FILE_READER_HPP
#define TPCH_JOIN_FILE_READER_HPP

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class FileReader {
public:
    explicit FileReader(const std::string& path, char delimiter = '|') : path_(path), delimiter_(delimiter) {}

    // Reads every line of the file and turns it into a Row.
    template <typename Row>
    std::vector<Row> readAll() const {
        std::ifstream file(path_);
        if (!file) {
            throw std::runtime_error("Could not open file: " + path_);
        }

        std::vector<Row> rows;
        std::string line;
        while (std::getline(file, line)) {
            // Remove the '\r' left behind by files created on Windows.
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            if (line.empty()) {
                continue;
            }
            rows.push_back(Row::fromFields(split(line)));
        }
        return rows;
    }

private:
    std::vector<std::string> split(const std::string& line) const {
        std::vector<std::string> fields;
        std::stringstream stream(line);
        std::string field;
        while (std::getline(stream, field, delimiter_)) {
            fields.push_back(field);
        }
        return fields;
    }

    std::string path_;
    char delimiter_;
};
#endif //TPCH_JOIN_FILE_READER_HPP
