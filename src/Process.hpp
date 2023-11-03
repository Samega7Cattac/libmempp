#ifndef LIBMEMPP_PROCESS_HPP
#define LIBMEMPP_PROCESS_HPP

#ifdef __WIN32
#include "windows/"
#elif __unix__
#include "unix/Process.hpp"
#endif

#endif // LIBMEMPP_PROCESS_HPP