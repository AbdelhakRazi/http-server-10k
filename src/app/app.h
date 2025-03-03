#pragma once

#include <functional>
#include <string>
#include "http/http_request.h"
#include "http/http_response.h"

class App {
    public: 
        void get(std::string uri, std::function<HttpResponse(const HttpRequest&)> handler);
        void post(std::string uri, std::function<HttpResponse(const HttpRequest&)> handler);
        void put(std::string uri, std::function<HttpResponse(const HttpRequest&)> handler);
        void deletee(std::string uri, std::function<HttpResponse(const HttpRequest&)> handler);
};