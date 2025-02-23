#ifndef __HTTP_EXCEPTION__H__
#define __HTTP_EXCEPTION__H__

#include <exception>
#include <string>

class HttpException : public std::exception {
    std::string message;
    public:
        explicit HttpException(std::string message) : message{message} {};
        const char* what() const noexcept override {
        return message.c_str();
    }
};
#endif  //!__HTTP_EXCEPTION__H__