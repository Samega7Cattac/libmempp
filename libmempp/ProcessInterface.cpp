#include "ProcessInterface.hpp"

using namespace MEM;

ProcessInterface::~ProcessInterface()
{

}

std::uintptr_t
ProcessInterface::GetPID() const
{
    return m_pid;
}

ProcessInterface::ProcessInterface(const std::uintptr_t pid) :
    m_pid(pid)
{

}
