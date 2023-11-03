#include "ProcessInterface.hpp"

MEM::ProcessInterface::~ProcessInterface()
{

}

std::uintptr_t
MEM::ProcessInterface::GetPID() const
{
    return m_pid;
}

MEM::ProcessInterface::ProcessInterface(std::uintptr_t pid) :
    m_pid(pid)
{

}
