#include <drogon/HttpController.h>   // Drogon framework header for HTTP controllers
#include <pugixml.hpp>               // pugixml: C++ XML processing library
#include <jsoncons/json.hpp>         // jsoncons: JSON parsing library
#include <jsoncons_ext/jsonpath/jsonpath.hpp> // jsonpath support for jsoncons
#include <fstream>
#include <sstream>

using namespace drogon;
namespace jc = jsoncons;            // alias for jsoncons namespace
namespace jp = jsoncons::jsonpath;  // alias for jsonpath utilities
using json = jc::json;              // alias: we'll just write 'json' instead of jc::json

// ------------------- CONTROLLER -------------------
class ToXmlController : public HttpController<ToXmlController>
{
public:
    METHOD_LIST_BEGIN
        // Register an HTTP POST route: /convert/toxml
        // Calls convertJsonToXml whenever that route is hit
        ADD_METHOD_TO(ToXmlController::convertJsonToXml, "/convert/toxml", Post);
    METHOD_LIST_END

    // ------------------- MAIN HANDLER -------------------
    void convertJsonToXml(const HttpRequestPtr &req,
                          std::function<void(const HttpResponsePtr &)> &&callback)
    {
        try
        {
            // ------------------- PARSE JSON -------------------
            // Take the raw request body (string of JSON)
            auto body = req->getBody();
            // Parse it into a jsoncons::json object
            json j = json::parse(body);

            // ------------------- CREATE XML DOCUMENT -------------------
            pugi::xml_document doc;              // root XML document
            auto student = doc.append_child("Student");  // add <Student> root node

            // ------------------- JSONPATH EXTRACTION -------------------
            // 1. Extract "id" with JSONPath: $.Student.id
            // "$" means root of JSON
            // ".Student" → go into object "Student"
            // ".id" → get the value of key "id"
            json id = jp::json_query(j, "$.Student.id");
            if (!id.empty())   // json_query returns an array of matches
                // Add XML attribute: <Student id="...">
                student.append_attribute("id") = id[0].as<std::string>().c_str();

            // 2. Extract "fullname" → then split into fname/lname
            json fullname = jp::json_query(j, "$.Student.fullname");
            if (!fullname.empty())
            {
                // 1. Take the JSONPath result (fullname[0]) and convert to std::string
                std::string name = fullname[0].as<std::string>();
                // e.g., name = "John Doe"

                // 2. Create an input string stream from that string
                std::istringstream iss(name);
                // This allows us to "stream" the string word by word, like reading from a file.

                // 3. Create two string variables to hold first and last name
                std::string fname, lname;

                // 4. Extract two words from the stream
                iss >> fname >> lname;
                // This uses the >> operator to split by whitespace.
                // Example: "John Doe" → fname = "John", lname = "Doe"
                // Example: "Jane Mary Smith" → fname = "Jane", lname = "Mary" (rest is ignored!)
                // Example: "Madonna" → fname = "Madonna", lname = "" (second extraction fails)

                // 5. Add these as child nodes to the XML <Student> node
                student.append_child("fname").text() = fname.c_str(); // <fname>John</fname>
                student.append_child("lname").text() = lname.c_str(); // <lname>Doe</lname>
            }


            // 3. Extract age
            // Notice JSONPath: "$.Student.age_dob.age"
            // Meaning: go into Student → object age_dob → key age
            json age = jp::json_query(j, "$.Student.age_dob.age");
            if (!age.empty())
                student.append_child("Age").text() = age[0].as<std::string>().c_str();

            // 4. Extract dob
            json dob = jp::json_query(j, "$.Student.age_dob.dob");
            if (!dob.empty())
                student.append_child("DOB").text() = dob[0].as<std::string>().c_str();

            // ------------------- PROFESSION (nested object) -------------------
            // Create <Profession> child
            auto professionNode = student.append_child("Profession");

            // Extract title
            json title = jp::json_query(j, "$.Student.Profession.title");
            if (!title.empty())
                professionNode.append_child("title").text() = title[0].as<std::string>().c_str();

            // Extract experience
            json experience = jp::json_query(j, "$.Student.Profession.experience");
            if (!experience.empty())
                professionNode.append_child("experience").text() = experience[0].as<std::string>().c_str();

            // ------------------- CONVERT XML TO STRING -------------------
            std::ostringstream oss;
            doc.save(oss, "  ");   // save XML into stringstream, pretty-printed with indent = "  "

            // ------------------- RETURN RESPONSE -------------------
            auto resp = HttpResponse::newHttpResponse();
            resp->setContentTypeCode(CT_APPLICATION_XML); // content type = XML
            resp->setBody(oss.str());                     // XML string body
            callback(resp);
        }
        catch (const std::exception &e)
        {
            // ------------------- ERROR HANDLING -------------------
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k400BadRequest);  // HTTP 400 = Bad Request
            resp->setContentTypeCode(CT_TEXT_PLAIN);
            resp->setBody(std::string("Invalid JSON: ") + e.what()); // send error details
            callback(resp);
        }
    }
};




    🔑 Key Points to Understand

    fullname[0].as<std::string>()

    JSONPath queries return an array of results.

    Even if only one match, you access it with [0].

    .as<std::string>() converts the JSON value to a std::string.

    Why std::istringstream is used

    It lets you parse words from a string as if you were reading from a file or console.

    The >> operator extracts tokens separated by whitespace.

    Naive split limitation

    This code assumes fullname = exactly "First Last".

    If fullname has more than 2 words ("John Michael Doe") → last name becomes "Michael", and "Doe" is ignored.

    If fullname has only 1 word ("Cher") → last name will be empty.

    Result in XML

    Input JSON:

    { "Student": { "fullname": "John Doe" } }


    Output XML:

    <Student>
    <fname>John</fname>
    <lname>Doe</lname>
    </Student>