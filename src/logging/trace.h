#include <iostream>
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>

#define DEBUG_LOG(const char* fmt) // fmt is const char*
std::ofstream ofs("log.txt", std::ofstream::out | std::ofstream::app);
std::time_t t = std::time(nullptr);
char time_buf[100];
std::strftime(time_buf, sizeof time_buf, "%D %T", std::gmtime(&t));
std::va_list args1;
va_start(args1, fmt);
std::vector<char> buf(1 + std::vsnprintf(nullptr, 0, fmt, args1));
va_end(args1); // first usage for args1
// second usage
va_start(args1, fmt);
std::vsnprintf(buf.data(), buf.size(), fmt, args1); // format va list to a buffer of strings
va_end(args1);
std::cout << time_buf << " [debug]: " << buf.data() << "\n";
ofs << time_buf << " [debug]: " << buf.data() << "\n";
ofs.close();