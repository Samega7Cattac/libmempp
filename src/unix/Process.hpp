#ifndef MEMPP_LINUX_PROCESS_HPP
#define MEMPP_LINUX_PROCESS_HPP

#include "../ProcessInterface.hpp"

#include <cstdint>
#include <fcntl.h>
#include <vector>
#include <string>
#include <sys/mman.h>
#include <cstring>
#include <unistd.h>

namespace MEM
{
enum Permission
{
    READ = O_RDONLY,
    WRITE = O_WRONLY,
    READ_WRITE = O_RDWR
};

struct MemoryPointer
{
    std::string segment_name;
    off_t offset{};
};

class Process : public ProcessInterface
{
public:
    Process(std::uintptr_t pid, Permission permissions);

    ~Process() override;

    template<typename T>
    std::vector<MemoryPointer> ScanByValue(const T& value)
    {
        std::vector<MemoryPointer> ptrs;
        for (const MemorySegment& map : m_maps)
        {
            for (off_t pos = map.start; pos < map.end; ++pos)
            {
                T data;
                if (pread(m_mem_fd, &data, sizeof(T), pos) == -1)
                {
                    std::perror("Error reading memory");
                }
                if (data == value)
                {
                    MemoryPointer ptr;
                    ptr.offset = pos;
                    ptr.segment_name = map.path;
                    ptrs.emplace_back(ptr);
                }
            }
        }
        return ptrs;
    }

    template<typename T>
    T GetValue(const MemoryPointer& ptr)
    {
        if (lseek(m_mem_fd, ptr.offset, SEEK_SET) != ptr.offset)
        {
            std::perror("Error pointing to address");
        }

        T value;
        if (read(m_mem_fd, &value, sizeof(T)) == -1)
        {
            std::perror("Error reading value to memory");
        }
        return value;
    }

    template<typename T>
    void WriteValue(const MemoryPointer& ptr, const T& value)
    {
        if (lseek(m_mem_fd, ptr.offset, SEEK_SET) != ptr.offset)
        {
            std::perror("Error pointing to address");
        }

        if (write(m_mem_fd, &value, sizeof(T)) == -1)
        {
            std::perror("Error writing value to memory");
        }
    }

private:

    struct MemorySegment
    {
        std::string permissions;
        off_t start;
        off_t end;
        std::string path;
        // void* map;
    };

    int m_mem_fd;

    std::vector<MemorySegment> m_maps;
};
}

#endif // MEMPP_LINUX_PROCESS_HPP