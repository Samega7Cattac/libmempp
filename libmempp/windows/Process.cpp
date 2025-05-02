#include "Process.hpp"

#include <iostream>

using namespace MEM;

Process::Process(std::uintptr_t pid, Permission permissions) :
    ProcessInterface(pid)
{
    EnablePrivilege(SE_DEBUG_NAME);

    m_process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | permissions, FALSE, pid);
    if (m_process_handle == nullptr)
    {
        std::cerr << "Failed to open process. Error code: " << GetLastError() << std::endl;
    }

    UpdateMemoryMap();
}

Process::~Process()
{
    if (m_process_handle != nullptr)
    {
        CloseHandle(m_process_handle);
    }
}

void
Process::UpdateMemoryMap()
{
    LPVOID lpAddress = nullptr;
    MEMORY_BASIC_INFORMATION memInfo;
    while (VirtualQueryEx(m_process_handle, lpAddress, &memInfo, sizeof(memInfo)) == sizeof(memInfo))
    {
        // Print information about the memory region
        std::cout << "Base Address: " << memInfo.BaseAddress << std::endl;
        std::cout << "Region Size: " << memInfo.RegionSize << std::endl;
        std::cout << "State: " << memInfo.State << std::endl;
        std::cout << "Protect: " << memInfo.Protect << std::endl;
        std::cout << "Type: " << memInfo.Type << std::endl;
        std::cout << std::endl;

        // Move to the next memory region
        lpAddress = (LPVOID)((uintptr_t)memInfo.BaseAddress + memInfo.RegionSize);
    }
}

std::vector<MemoryPointer>
Process::ScanByValue(const void* value, const std::size_t data_size)
{
    std::vector<MemoryPointer> ptrs;
//    for (const MemoryRegion& map : m_maps)
//    {
//        for (off_t pos = map.start; pos < map.end; ++pos)
//        {
//            void* data;
//            if (pread(m_mem_fd, &data, data_size, pos) == -1)
//            {
//                std::perror("Error reading memory");
//            }
//            if (memcmp(value, data, data_size) == 0)
//            {
//                ptrs.emplace_back(map.path, pos);
//            }
//        }
//    }
    return ptrs;
}

void*
Process::GetValue(const MemoryPointer& ptr, const std::size_t data_size)
{
//    if (lseek(m_mem_fd, ptr.offset, SEEK_SET) != ptr.offset)
//    {
//        std::perror("Error pointing to address");
//    }

    void* value;
//    if (read(m_mem_fd, &value, data_size) == -1)
//    {
//        std::perror("Error reading value to memory");
//    }
    return value;
}

void
Process::WriteValue(const MemoryPointer& ptr,
                    const void* value,
                    std::size_t data_size)
{
//    if (lseek(m_mem_fd, ptr.offset, SEEK_SET) != ptr.offset)
//    {
//        std::perror("Error pointing to address");
//    }
//
//    if (write(m_mem_fd, &value, sizeof(T)) == -1)
//    {
//        std::perror("Error writing value to memory");
//    }
}

void
Process::EnablePrivilege(LPCTSTR privilegeName)
{
    HANDLE hToken;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken) == false) {
        std::cerr << "OpenProcessToken failed. Error code: " << GetLastError() << std::endl;
    }

    TOKEN_PRIVILEGES tp;
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (LookupPrivilegeValue(nullptr, privilegeName, &tp.Privileges[0].Luid) == false) {
        std::cerr << "LookupPrivilegeValue failed. Error code: " << GetLastError() << std::endl;
        CloseHandle(hToken);
    }

    if (AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL) == false) {
        std::cerr << "AdjustTokenPrivileges failed. Error code: " << GetLastError() << std::endl;
        CloseHandle(hToken);
    }

    CloseHandle(hToken);
}
