#ifndef LIBMEMPP_PROCESS_HPP
#define LIBMEMPP_PROCESS_HPP

#ifdef _WIN32
#include "windows/Process.hpp"
#elif __unix__
#include "unix/Process.hpp"
#endif

#endif // LIBMEMPP_PROCESS_HPP