#include "parser/http_parser.h"
#include "trace/trace.h"
#include "exceptions/http_exception.h"
#include "http/http_request.h"

#include <iostream>
#include <unordered_set>
namespace {
    bool body_separator(const char* buffer, int cursor) {
        return (buffer[cursor] == '\r' && buffer[cursor + 1] == '\n' &&
        buffer[cursor + 2] == '\r' && buffer[cursor + 3] == '\n');
    }
    bool line_separator(const char* buffer, int cursor) {
        return (buffer[cursor] == '\r' && buffer[cursor + 1] == '\n');
    }
    std::unordered_set<std::string> HTTP_METHODS{"GET", "POST", "PUT", "DELETE"};
    std::unordered_set<std::string> HTTP_VERSIONS{"HTTP/1.0", "HTTP/1.1", "HTTP/2"};
}

HttpRequest parse_http_request(const char* buffer, ssize_t nb_bytes) {
    int iterator = 0;
    std::string method;
    // for error prone, we check against nb_bytes read
    while(buffer[iterator] != ' ' && iterator < nb_bytes) {
      method.push_back(buffer[iterator]);
      iterator++;
    }
    if(HTTP_METHODS.find(method) == HTTP_METHODS.end()) throw HttpException{"Invalid HTTP method"};
    iterator++;
    std::string uri;
    while(buffer[iterator] != ' ' && iterator < nb_bytes) {
      uri.push_back(buffer[iterator]);
      iterator++;
    }
    iterator++;
    std::string version;
    while(!line_separator(buffer, iterator) && iterator < nb_bytes) {
      version.push_back(buffer[iterator]);
      iterator++;
    }
    TRACE_DEBUG("version %s", version.c_str());
    if(HTTP_VERSIONS.find(version) == HTTP_VERSIONS.end()) throw HttpException{"Invalid HTTP Version"};
    iterator += 2;
    std::optional<std::string> optional_headers;
    std::string hdrs;
    while(!body_separator(buffer, iterator)) {
      hdrs.push_back(buffer[iterator]);
      iterator++;
    }
    if(!hdrs.empty()) optional_headers = hdrs;
    iterator += 4;
    buffer = buffer + iterator;
    std::optional<std::string> body = std::string{buffer, static_cast<size_t>(nb_bytes)};
    return {method, uri, version, optional_headers, body};
}