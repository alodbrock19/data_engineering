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
        int fd = ::open(path.c_str(), O_RDONLY);
        if (fd < 0) {
            throw std::runtime_error("Could not open file: " + path);
        }

        struct stat info {};
        if (::fstat(fd, &info) != 0) {
            ::close(fd);
            throw std::runtime_error("Could not determine the size of: " + path);
        }
        size_ = static_cast<std::size_t>(info.st_size);

        if (size_ > 0) {
            void* address = ::mmap(nullptr, size_, PROT_READ, MAP_PRIVATE, fd, 0);
            if (address == MAP_FAILED) {
                ::close(fd);
                throw std::runtime_error("Could not map: " + path);
            }
            data_ = static_cast<char*>(address);
        }

        // The mapping stays valid after the file descriptor is closed.
        ::close(fd);
    }

    ~MappedFile() {
        if (data_ != nullptr) {
            ::munmap(data_, size_);
        }
    }

    // Copying would lead to two objects unmapping the same memory.
    MappedFile(const MappedFile&) = delete;
    MappedFile& operator=(const MappedFile&) = delete;

    [[nodiscard]] const char* data() const {
        return data_;
    }
    [[nodiscard]] std::size_t size() const {
        return size_;
    }

private:
    char* data_ = nullptr;
    std::size_t size_ = 0;
};
#endif //TPCH_JOIN_MAPPED_FILE_HPP
