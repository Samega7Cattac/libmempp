#ifndef MEMPP_PROCESS_HPP
#define MEMPP_PROCESS_HPP

// STD headers
#include <cstdint>
#include <vector>
#include <string>

namespace MEM
{
struct MemoryPointer
{
    std::string segment_name;
    off_t offset;
};

class ProcessInterface
{
public:
    ProcessInterface() = delete;
    virtual ~ProcessInterface();

    [[nodiscard]] std::uintptr_t GetPID() const;

    virtual void UpdateMemoryMap() = 0;

protected:
    explicit ProcessInterface(const std::uintptr_t pid);

    virtual std::vector<MemoryPointer>
    ScanByValue(const void* value, const std::size_t data_size) = 0;

    virtual void*
    GetValue(const MemoryPointer& ptr, const std::size_t data_size) = 0;

    virtual void
    WriteValue(const MemoryPointer& ptr,
               const void* value,
               const std::size_t data_size) = 0;

private:
    const std::uintptr_t m_pid;
};
}

#endif // MEMPP_PROCESS_HPP
