#ifndef __HTTP_PARSER__H__
#define __HTTP_PARSER__H__

#include "http/http_request.h"
#include "http/header.h"
#include "http/body.h"

namespace {
    bool body_separator(const char* buffer, int cursor);
}

HttpRequest parse_http_request(const char* buffer, int nb_bytes);
Header create_header(std::string&& header, int nb_bytes);
Body create_payload(std::string&& payload);

#endif  //!__HTTP_PARSER__H__
