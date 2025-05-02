#ifndef MEMPP_LINUX_PROCESS_HPP
#define MEMPP_LINUX_PROCESS_HPP

#include "../ProcessInterface.hpp"

// STD headers
#include <cstdint>
#include <vector>
#include <cstring>

// UNIX headers
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

namespace MEM
{
enum Permission
{
    READ = O_RDONLY,
    WRITE = O_WRONLY,
    READ_WRITE = O_RDWR
};

class Process : public ProcessInterface
{
public:
    Process(const std::uintptr_t pid, const Permission permissions);

    ~Process() override;

    void UpdateMemoryMap() override;

    template<typename T>
    std::vector<MemoryPointer> ScanByValue(const T& value)
    {
        return ScanByValue(&value, sizeof(T));
    }

    template<typename T>
    T GetValue(const MemoryPointer& ptr)
    {
        void* data = GetValue(ptr, sizeof(T));
        return reinterpret_cast<T>(data);
    }

    template<typename T>
    void WriteValue(const MemoryPointer& ptr, const T& value)
    {
        WriteValue(ptr, &value, sizeof(T));
    }

protected:
    std::vector<MemoryPointer>
    ScanByValue(const void* value, const std::size_t data_size) override;

    void*
    GetValue(const MemoryPointer& ptr, const std::size_t data_size) override;

    void
    WriteValue(const MemoryPointer& ptr,
               const void* value,
               std::size_t data_size) override;

private:
    struct MemoryRegion
    {
        std::string permissions;
        off_t start;
        off_t end;
        std::string path;
        // void* map;
    };

    int m_mem_fd;

    std::vector<MemoryRegion> m_maps;
};
}

#endif // MEMPP_LINUX_PROCESS_HPP