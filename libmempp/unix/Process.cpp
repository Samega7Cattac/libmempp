#include "Process.hpp"

#include <string>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <fstream>

#include <iostream>

using namespace MEM;

static const char* PROCESS_DIRECTORY = "/proc/";
static const char* MEMORY_FILE = "/mem";
static const char* MAPS_FILE = "/maps";

[[maybe_unused]]static const uint64_t THREAD_CREATION_THRESHOLD = 100000;

Process::Process(const std::uintptr_t pid, const Permission permissions) :
    ProcessInterface(pid),
    m_mem_fd(-1)
{
    const std::string mem_file_path(PROCESS_DIRECTORY + std::to_string(pid) + MEMORY_FILE);

    m_mem_fd = open(mem_file_path.c_str(), permissions);
    if (m_mem_fd == -1)
    {
        std::perror("Error opening memory file");
        // TODO: throw something
    }

    UpdateMemoryMap();
}

Process::~Process()
{
    if (m_mem_fd != -1)
    {
        close(m_mem_fd);
    }
}

void
Process::UpdateMemoryMap()
{
    std::string map_file_path(PROCESS_DIRECTORY + std::to_string(GetPID()) + MAPS_FILE);
    std::ifstream map_fd(map_file_path);
    if (map_fd.is_open() == false)
    {
        std::perror("Error opening map file");
    }

    struct MemoryRegion map;
    char perm[5];
    char path[255];
    std::string line;
    while(std::getline(map_fd, line))
    {
        memset(&perm, 0, 5);
        memset(&path, 0, 255);
        sscanf(line.c_str(), "%lx-%lx %s %*x %*x:%*x %*d %s", &map.start, &map.end, &perm, &path);
        if (perm[0] != 'r' && perm[1] != 'w')
        {
            continue;
        }
        map.permissions = perm;
        map.path = path;
        m_maps.emplace_back(map);
    }
}

std::vector<MEM::MemoryPointer>
Process::ScanByValue(const void* value, const std::size_t data_size)
{
    std::vector<MemoryPointer> ptrs;
    for (const MemoryRegion& map : m_maps)
    {
        for (off_t pos = map.start; pos < map.end; ++pos)
        {
            void* data;
            if (pread(m_mem_fd, &data, data_size, pos) == -1)
            {
                std::perror("Error reading memory");
            }
            if (memcmp(value, &data, data_size) == 0)
            {
                ptrs.emplace_back(map.path, pos);
            }
        }
    }
    return ptrs;
}

void*
Process::GetValue(const MemoryPointer& ptr, const std::size_t data_size)
{
    if (lseek(m_mem_fd, ptr.offset, SEEK_SET) != ptr.offset)
    {
        std::perror("Error pointing to address");
    }

    void* value;
    if (read(m_mem_fd, &value, data_size) == -1)
    {
        std::perror("Error reading value to memory");
    }
    return value;
}

void
Process::WriteValue(const MemoryPointer& ptr,
                    const void* value,
                    const std::size_t data_size)
{
    if (lseek(m_mem_fd, ptr.offset, SEEK_SET) != ptr.offset)
    {
        std::perror("Error pointing to address");
    }

    if (write(m_mem_fd, value, data_size) == -1)
    {
        std::perror("Error writing value to memory");
    }
}
