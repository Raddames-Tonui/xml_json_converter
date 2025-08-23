#pragma once

#include <drogon/HttpController.h>

// Cross-platform export/import macro
#ifdef _WIN32
  #ifdef BUILD_DLL
    #define API __declspec(dllexport)   
  #else
    #define API __declspec(dllimport)  
  #endif
#else
  #define API                          
#endif

class API ToJsonController : public drogon::HttpController<ToJsonController>
{
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(ToJsonController::xmlToJson, "/convert/tojson", Post);
    METHOD_LIST_END

    void xmlToJson(const drogon::HttpRequestPtr &req,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};

class API ToXmlController : public drogon::HttpController<ToXmlController>
{
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(ToXmlController::convertJsonToXml, "/convert/toxml", Post);
    METHOD_LIST_END

    void convertJsonToXml(const drogon::HttpRequestPtr &req,
                          std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};
