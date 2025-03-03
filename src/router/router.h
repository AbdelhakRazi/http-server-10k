#pragma once

#include <unordered_map>
#include <functional>
#include "http/http_request.h"
#include "http/http_response.h"

class Router {
    public:
        // key: method:uri
        static std::unordered_map<std::string, std::function<HttpResponse(const HttpRequest&)>> routes;
};