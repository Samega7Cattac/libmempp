#include <iostream>
#include <vector>

#include "../libmempp/Process.hpp"

int
main(int argc, char* argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    std::uintptr_t pid = atoll(argv[1]);

    MEM::Process proc(pid, MEM::Permission::READ_WRITE);

    std::cout << "PID: " << pid << std::endl;
    std::vector<MEM::MemoryPointer> ptrs = proc.ScanByValue<int>(120);

    std::vector<MEM::MemoryPointer> stack_ptrs;

    for (const MEM::MemoryPointer& ptr : ptrs)
    {
        std::cout << "[" << ptr.segment_name << "] " << std::hex << ptr.offset;
        if (ptr.segment_name == "[stack]")
        {
            stack_ptrs.emplace_back(ptr);
            std::cout << std::dec << " : " << proc.GetValue<int>(ptr);
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << "WRITING" << std::endl;

    for (const MEM::MemoryPointer& ptr : stack_ptrs)
    {
        proc.WriteValue<int>(ptr, 0);
        std::cout << ptr.segment_name << " "
            << std::hex << ptr.offset << " : "
            << std::dec << proc.GetValue<int>(ptr) << std::endl;
    }

    return 0;
}
