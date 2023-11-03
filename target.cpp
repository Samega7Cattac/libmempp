#include <iostream>

#include <unistd.h>

int
main()
{
    int target = 120;
    std::cout << "pid: " << getpid() << std::endl;
    std::cout << "ptr:" << std::hex << &target << std::dec << " : " << target << std::endl;
    getchar();
    std::cout << "ptr:" << std::hex << &target << std::dec << " : " << target << std::endl;
    return target;
}