#include "http_parser.h"
#include <iostream>
namespace {
    bool body_separator(const char* buffer, int cursor) {
        if(buffer[cursor] == '\r' &&
        buffer[cursor + 1] == '\n' &&
        buffer[cursor + 2] == '\r' &&
        buffer[cursor + 3] == '\n') return true;
        return false;
}
}

HttpRequest parse_http_request(const char* buffer, int nb_bytes) {
    // HTTP request: header + empty line + body (optional in case of put and POST)
    int cursor = 0;
    while(!body_separator(buffer, cursor)) {
        cursor++;
        if(cursor > nb_bytes) {
          return {"", ""};
        }
    }
    std::string header{std::string(buffer, cursor)};
    int endpoint_cursor = cursor + 4;
    while(buffer[cursor] != '\0') {
        cursor++;
        if(cursor > nb_bytes) return {create_header(std::move(header), nb_bytes), ""};
    }
    std::string body{std::string(buffer, endpoint_cursor, cursor - endpoint_cursor)}; // risque d'accéder à un espace mémoire qui existe pas.
    return {create_header(std::move(header), nb_bytes), create_payload(std::move(body))};
}

Header create_header(std::string&& header, int nb_bytes) {
    // extract header fields
    int cursor = 0;
    while(header[cursor] != ' ') {
      cursor++;
    }
    if(cursor > nb_bytes) return Header{"", "", ""};
    std::string method{header.substr(0, cursor)};
    int endpoint_cursor = ++cursor;
    while(header[cursor] != ' ') {
      cursor++;
    }
    if(cursor > nb_bytes) return Header{method, "", ""};
    std::string endpoint{std::string(header, endpoint_cursor, cursor - endpoint_cursor)};
    while(header[cursor] != '/') {
      cursor++;
    }
    endpoint_cursor = cursor + 1;
    while(header[cursor] != '\r' && header[cursor + 1] != '\n') {
      cursor++;
    }
    if(cursor > nb_bytes) return Header{header, endpoint, ""};
    std::string version{std::string(header, endpoint_cursor, cursor - endpoint_cursor)};
    return {method, endpoint, version};
}
Body create_payload(std::string&& payload) {
    return {payload};
}