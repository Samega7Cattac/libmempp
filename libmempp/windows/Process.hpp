#ifndef MEMPP_WINDOWS_PROCESS_HPP
#define MEMPP_WINDOWS_PROCESS_HPP

#include "../ProcessInterface.hpp"

// STD headers
#include <string>

// WIN32 API headers
#include <windows.h>

namespace MEM
{

enum Permission
{
    READ = PROCESS_VM_READ,
    WRITE = PROCESS_VM_WRITE,
    READ_WRITE = PROCESS_VM_READ | PROCESS_VM_WRITE
};

class Process : public ProcessInterface
{
public:
    Process(std::uintptr_t pid, Permission permissions);

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
        return *reinterpret_cast<T*>(data);
    }

    template<typename T>
    void WriteValue(const MemoryPointer& ptr, const T& value)
    {
        WriteValue(ptr, &value, sizeof(T));
    }

protected:
    std::vector<MemoryPointer>
    ScanByValue(const void* value, std::size_t data_size) override;

    void*
    GetValue(const MemoryPointer& ptr, std::size_t data_size) override;

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

    HANDLE m_process_handle;

    static void EnablePrivilege(LPCTSTR privilegeName);
};
}

#endif // MEMPP_WINDOWS_PROCESS_HPP
