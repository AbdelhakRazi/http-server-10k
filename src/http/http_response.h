#ifndef __HTTP_RESPONSE__H__
#define __HTTP_RESPONSE__H__

#include <string>
#include <optional>

struct HttpResponse{
    std::string version;
    int status_code;
    std::string status_message;
    std::optional<std::string> body;
};

#endif  //!__HTTP_RESPONSE__H__