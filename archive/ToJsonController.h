#pragma once
#include <drogon/HttpController.h>
#include <pugixml.hpp>
#include <json/json.h>   

using namespace drogon;

class ToJsonController : public HttpController<ToJsonController>
{
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(ToJsonController::toJson, "/convert/tojson", Post);
    METHOD_LIST_END

    void toJson(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);

private:
    // recursively convert an XML node to JSON
    Json::Value xmlNodeToJson(const pugi::xml_node &node);
};
