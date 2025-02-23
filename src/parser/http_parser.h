#ifndef __HTTP_PARSER__H__
#define __HTTP_PARSER__H__

#include "http/http_request.h"

HttpRequest parse_http_request(const char* buffer, ssize_t nb_bytes);
#endif  //!__HTTP_PARSER__H__
