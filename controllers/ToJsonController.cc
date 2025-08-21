#include <drogon/HttpController.h>
#include <pugixml.hpp>
#include <json/json.h>   // Drogon uses this internally

using namespace drogon;

class ToJsonController : public drogon::HttpController<ToJsonController> {
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(ToJsonController::xmlToJson, "/convert/tojson", Post);
    METHOD_LIST_END

    void xmlToJson(const HttpRequestPtr &req,
                   std::function<void(const HttpResponsePtr &)> &&callback)
    {
        std::string xmlStr(req->getBody());  // FIXED

        pugi::xml_document doc;
        if (!doc.load_string(xmlStr.c_str()))
        {
            Json::Value err;
            err["error"] = "Invalid XML";
            auto resp = HttpResponse::newHttpJsonResponse(err);
            callback(resp);
            return;
        }

        auto studentNode = doc.select_node("/Student");
        if (!studentNode)
        {
            Json::Value err;
            err["error"] = "No Student node found";
            auto resp = HttpResponse::newHttpJsonResponse(err);
            callback(resp);
            return;
        }

        // auto id    = studentNode.node().attribute("id").value();
        auto id = doc.select_node("/Student/@id").attribute().value();
        auto fname = doc.select_node("/Student/fname").node().child_value();
        auto lname = doc.select_node("/Student/lname").node().child_value();
        auto age   = doc.select_node("/Student/Age").node().child_value();
        auto dob   = doc.select_node("/Student/DOB").node().child_value();
        // auto profession = doc.select_node("/Student/Profession").node().child_value();
        auto title = doc.select_node("/Student/Profession/title").node().child_value();
        auto experience = doc.select_node("/Student/Profession/experience").node().child_value();


        Json::Value out;
        out["Student"]["student_id"] = id;
        out["Student"]["fullname"] = std::string(fname) + " " + lname;
        out["Student"]["age_dob"]["age"] = age;
        out["Student"]["age_dob"]["dob"] = dob;
        // out["Student"]["Profession"] = profession;
        out["Student"]["Profession"]["title"] = title;
        out["Student"]["Profession"]["experience"] = experience;
        
        auto resp = HttpResponse::newHttpJsonResponse(out);
        callback(resp);
    }
};
