#include "Process.hpp"

#include <string>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <fstream>

#include <iostream>

static const char* PROCESS_DIRECTORY = "/proc/";
static const char* MEMORY_FILE = "/mem";
static const char* MAPS_FILE = "/maps";

MEM::Process::Process(std::uintptr_t pid, Permission permissions) :
    ProcessInterface(pid),
    m_mem_fd(-1)
{
    std::string mem_file_path(PROCESS_DIRECTORY + std::to_string(pid) + MEMORY_FILE);

    m_mem_fd = open(mem_file_path.c_str(), permissions);
    if (m_mem_fd == -1)
    {
        std::perror("Error opening memory file");
        // TODO: throw something
    }

    std::string map_file_path(PROCESS_DIRECTORY + std::to_string(pid) + MAPS_FILE);
    std::ifstream map_fd(map_file_path);
    if (map_fd.is_open() == false)
    {
        std::perror("Error opening map file");
    }

    struct MemorySegment map;
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

MEM::Process::~Process()
{
    if (m_mem_fd != -1)
    {
        close(m_mem_fd);
    }
}
