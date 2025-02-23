#ifndef __HTTP_RESPONSE__H__
#define __HTTP_RESPONSE__H__

#include <string>
#include <optional>
#include <sstream>
struct HttpResponse{
    std::string version;
    int status_code;
    std::string status_message;
    std::optional<std::string> optional_headers;
    std::optional<std::string> body;
};

inline std::string serialize_response(HttpResponse& response)
{
    std::ostringstream oss;
    oss << response.version << ' ' << response.status_code << ' ' << response.status_message << "\r\n";
    std::string optional_hdrs;
    if(response.optional_headers.has_value()) {
        oss << response.optional_headers.value() << "\r\n";
    }
    if(response.body.has_value()) {
        oss << "\r\n" << response.body.value();
    }
    
    return oss.str();;
}
#endif  //!__HTTP_RESPONSE__H__