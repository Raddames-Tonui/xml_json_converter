#include "ToXmlController.h"
#include <sstream>

void ToXmlController::jsonToXmlNode(const Json::Value &j, pugi::xml_node &node)
{
    if (j.isObject())
    {
        for (auto it = j.begin(); it != j.end(); ++it)
        {
            std::string key = it.key().asString();

            if (!key.empty() && key[0] == '@')
            {
                // JSON "@attr" → XML attribute
                node.append_attribute(key.substr(1).c_str())
                    .set_value(it->asCString());
            }
            else if (key == "_text")
            {
                // JSON "_text" → XML text content
                node.text().set(it->asCString());
            }
            else
            {
                // Handle nested objects or arrays
                if (it->isArray())
                {
                    for (const auto &el : *it)
                    {
                        pugi::xml_node child = node.append_child(key.c_str());
                        jsonToXmlNode(el, child); 
                    }
                }
                else
                {
                    pugi::xml_node child = node.append_child(key.c_str());
                    jsonToXmlNode(*it, child); 
                }
            }
        }
    }
    else if (j.isString() || j.isNumeric() || j.isBool())
    {
        // Plain value → text node
        node.text().set(j.asCString());
    }
}

void ToXmlController::toXml(const HttpRequestPtr &req,
                            std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto jsonStr = std::string(req->getBody());

    Json::CharReaderBuilder reader;
    Json::Value j;
    std::string errs;

    std::istringstream iss(jsonStr);
    if (!Json::parseFromStream(reader, iss, &j, &errs))
    {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        resp->setBody("Invalid JSON");
        callback(resp);
        return;
    }

    // Create an XML doc
    pugi::xml_document doc;
    pugi::xml_node root;

    // If JSON has exactly one root key, use that as the element name
    if (j.isObject() && j.size() == 1)
    {
        std::string rootName = j.begin().name();
        root = doc.append_child(rootName.c_str());
        jsonToXmlNode(*j.begin(), root);
    }
    else
    {
        root = doc.append_child("root");
        jsonToXmlNode(j, root);
    }

    // Serialize to string
    std::ostringstream oss;
    doc.save(oss, "  ");

    auto resp = HttpResponse::newHttpResponse();
    resp->setContentTypeCode(ContentType::CT_TEXT_XML);
    resp->setBody(oss.str());
    callback(resp);
}
