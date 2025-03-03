#ifndef __TRACE__H__
#define __TRACE__H__

#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>

inline const char* getFileName(const char* path) {
    const char* file = std::strrchr(path, '/');
    if (!file) file = std::strrchr(path, '\\');  // Windows paths
    return file ? file + 1 : path;
}

namespace {
     void format_str(const char* file_name, int line, const char *level, const char *fmt...) // macro will expand to: trace::format_str, which is not accessible so that's why we can't put it in separate
    {
        std::ofstream ofs("log.txt", std::ofstream::out | std::ofstream::app);
        std::time_t t = std::time(nullptr);
        char time_buf[100];
        std::strftime(time_buf, sizeof time_buf, "%D %T ", std::gmtime(&t));
        std::va_list args1;
        va_start(args1, fmt);
        std::vector<char> buf(1 + std::vsnprintf(nullptr, 0, fmt, args1));
        va_end(args1); // first usage for args1
        // second usage
        va_start(args1, fmt);
        std::vsnprintf(buf.data(), buf.size(), fmt, args1); // format va list to a buffer of strings
        va_end(args1);
        std::cout << level << file_name << "." << line << " " << time_buf << buf.data() << "\n";
        ofs << level << file_name << "." << line << " " << time_buf << buf.data() << "\n";
        ofs.close();
    }
}
// pass file and line explicitly, otherwise it would take the file as trace.h
namespace trace {
#ifdef DEBUG
    #define TRACE_DEBUG(...) format_str(getFileName(__FILE__), __LINE__, "[DEBUG] ", __VA_ARGS__)
#else
    #define TRACE_DEBUG(...) // No-op when DEBUG is not defined
#endif // DEBUG

#define TRACE_INFO(...) format_str(getFileName(__FILE__), __LINE__,  "[INFO] ",  __VA_ARGS__)
#define TRACE_ERROR(...) format_str(getFileName(__FILE__), __LINE__, "[ERROR] ", __VA_ARGS__)
    // vsprintf has been depcrecated due to security issues.
    // use vsnprintf
} // namespace trace
#endif //!__TRACE__H__
