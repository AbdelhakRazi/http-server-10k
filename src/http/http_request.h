#ifndef __HTTP_REQUEST__H__
#define __HTTP_REQUEST__H__

#include <string>
#include <optional>
struct HttpRequest{
    std::string method;
    std::string uri;
    std::string version;
    std::optional<std::string> optional_headers;
    std::optional<std::string> body;
};

#endif  //!__HTTP_REQUEST__H__
