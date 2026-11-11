#ifndef TPCH_JOIN_MAPPED_FILE_HPP
#define TPCH_JOIN_MAPPED_FILE_HPP

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>

// Maps a whole file into the address space of the process (read-only).
// The mapping is created in the constructor and removed in the destructor.
class MappedFile {
public:
    explicit MappedFile(const std::string& path) {
        // TODO (Exercise 3):
        (void)path;
    }

    ~MappedFile() {
        // TODO (Exercise 3)
    }

    // Copying is forbidden. Why? (Exercise 3)
    MappedFile(const MappedFile&) = delete;
    MappedFile& operator=(const MappedFile&) = delete;

    [[nodiscard]] const char* data() const {
        return data_;
    }
    [[nodiscard]] std::size_t size() const {
        return size_;
    }

private:
    char* data_;
    std::size_t size_;
};
#endif //TPCH_JOIN_MAPPED_FILE_HPP
