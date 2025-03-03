#include "app/app.h"
#include "router/router.h"
#include "trace/trace.h"

std::unordered_map<std::string, std::function<HttpResponse(const HttpRequest&)>> Router::routes{};
void App::get(std::string uri, std::function<HttpResponse(const HttpRequest&)> handler)
{
    TRACE_INFO("called");
    Router::routes["GET:"+uri] = handler;
}

void App::post(std::string uri, std::function<HttpResponse(const HttpRequest &)> handler)
{
    Router::routes["POST:"+uri] = handler;
}

void App::put(std::string uri, std::function<HttpResponse(const HttpRequest &)> handler)
{
    Router::routes["PUT:"+uri] = handler;
}

void App::deletee(std::string uri, std::function<HttpResponse(const HttpRequest &request)> handler)
{
    Router::routes["DELETE:"+uri] = handler;
}
