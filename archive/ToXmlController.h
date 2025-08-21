#pragma once
#include <drogon/HttpController.h>
#include <pugixml.hpp>
#include <json/json.h>

using namespace drogon;

class ToXmlController : public HttpController<ToXmlController>
{
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(ToXmlController::toXml, "/convert/toxml", Post);
    METHOD_LIST_END

    void toXml(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback);

private:
    //  recursively convert JSON to XML
    void jsonToXmlNode(const Json::Value &j, pugi::xml_node &node);
};
