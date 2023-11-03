#include <iostream>
#include <vector>

#include "src/Process.hpp"

constexpr int PID = 8983;

int
main()
{
    MEM::Process proc(PID, MEM::Permission::READ_WRITE);

    std::cout << "PID: " << PID << std::endl;
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
        std::cout << "[" << ptr.segment_name << "] " << std::hex << ptr.offset << " : " << std::dec << proc.GetValue<int>(ptr);
    }

    return 0;
}
