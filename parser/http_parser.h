#pragma once

#include "../request/http_request.h"
#include "../header/header.h"
#include "../body/body.h"

namespace {
    bool body_separator(const char* buffer, int cursor);
}

HttpRequest parse_http_request(const char* buffer, int nb_bytes);
Header create_header(std::string&& header, int nb_bytes);
Body create_payload(std::string&& payload);