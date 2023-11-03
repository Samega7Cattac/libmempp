#ifndef MEMPP_PROCESS_HPP
#define MEMPP_PROCESS_HPP

#include <cstdint>
#include <vector>

namespace MEM
{
class ProcessInterface
{
public:
    ProcessInterface() = delete;
    virtual ~ProcessInterface();

    std::uintptr_t GetPID() const;

protected:
    explicit ProcessInterface(std::uintptr_t pid);

private:
    std::uintptr_t m_pid;
};
}

#endif // MEMPP_PROCESS_HPP