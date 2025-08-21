#include <drogon/HttpController.h>
#include <pugixml.hpp>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonpath/jsonpath.hpp>
#include <fstream>
#include <sstream>

using namespace drogon;
namespace jc = jsoncons;
namespace jp = jsoncons::jsonpath;
using json = jc::json;

class ToXmlController : public HttpController<ToXmlController>
{
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(ToXmlController::convertJsonToXml, "/convert/toxml", Post);
    METHOD_LIST_END

    void convertJsonToXml(const HttpRequestPtr &req,
                          std::function<void(const HttpResponsePtr &)> &&callback)
    {
        try
        {
            // Load JSON from request
            auto body = req->getBody();
            json j = json::parse(body);

            // Build XML
            pugi::xml_document doc;
            auto student = doc.append_child("Student");

            // id -> attribute
            json id = jp::json_query(j, "$.Student.id");
            if (!id.empty())
                student.append_attribute("id") = id[0].as<std::string>().c_str();

            // fullname -> split fname / lname
            json fullname = jp::json_query(j, "$.Student.fullname");
            if (!fullname.empty())
            {
                std::string name = fullname[0].as<std::string>();
                std::istringstream iss(name);
                std::string fname, lname;
                iss >> fname >> lname;
                student.append_child("fname").text() = fname.c_str();
                student.append_child("lname").text() = lname.c_str();
            }

            // age
            json age = jp::json_query(j, "$.Student.age_dob.age");
            if (!age.empty())
                student.append_child("Age").text() = age[0].as<std::string>().c_str();

            // dob
            json dob = jp::json_query(j, "$.Student.age_dob.dob");
            if (!dob.empty())
                student.append_child("DOB").text() = dob[0].as<std::string>().c_str();

            // profession
            auto professionNode = student.append_child("Profession");

            json title = jp::json_query(j, "$.Student.Profession.title");
            if (!title.empty())
                professionNode.append_child("title").text() = title[0].as<std::string>().c_str();

            json experience = jp::json_query(j, "$.Student.Profession.experience");
            if (!experience.empty())
                professionNode.append_child("experience").text() = experience[0].as<std::string>().c_str();

            // Convert XML doc to string
            std::ostringstream oss;
            doc.save(oss, "  ");

            auto resp = HttpResponse::newHttpResponse();
            resp->setContentTypeCode(CT_APPLICATION_XML);
            resp->setBody(oss.str());
            callback(resp);
        }
        catch (const std::exception &e)
        {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k400BadRequest);
            resp->setContentTypeCode(CT_TEXT_PLAIN);
            resp->setBody(std::string("Invalid JSON: ") + e.what());
            callback(resp);
        }
    }
};
