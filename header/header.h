#pragma once

#include <string>

struct Header {
    std::string method;
    std::string endpoint;
    std::string version;
};