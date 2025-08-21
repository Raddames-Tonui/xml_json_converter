#include "ToJsonController.h"
#include <sstream>


Json::Value ToJsonController::xmlNodeToJson(const pugi::xml_node &node)
{
    Json::Value j;

    for (auto &attr : node.attributes())
    {
        j["@" + std::string(attr.name())] = attr.value();
    }

    for (auto &child : node.children())
    {
        std::string name = child.name();

        if (!name.empty())
        {
            if (j.isMember(name))
            {
                if (!j[name].isArray())
                {
                    Json::Value tmp = j[name];
                    j[name] = Json::Value(Json::arrayValue);
                    j[name].append(tmp);
                }
                j[name].append(xmlNodeToJson(child)); 
            }
            else
            {
                j[name] = xmlNodeToJson(child);
            }
        }
        else if (child.type() == pugi::node_pcdata || child.type() == pugi::node_cdata)
        {
            // Handle text and CDATA content
            j["_text"] = child.value();
        }
    }

    // Leaf node case (no children, just text or CDATA)
    if (j.empty() && (node.type() == pugi::node_pcdata || node.type() == pugi::node_cdata))
    {
        return Json::Value(node.value());
    }

    return j;
}


void ToJsonController::toJson(const HttpRequestPtr &req,
                              std::function<void(const HttpResponsePtr &)> &&callback)
{
    std::string xmlContent(req->getBody());
    pugi::xml_document doc;

    // Step 1: Parse the XML input
    if (!doc.load_string(xmlContent.c_str()))
    {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        resp->setBody("Invalid XML");
        callback(resp);
        return;
    }

    // Optional query parameter: XPath expression
    std::string xpathExpr = req->getParameter("xpath");
    Json::Value j;

    if (!xpathExpr.empty())
    {
        // Step 2: Evaluate the XPath query
        pugi::xpath_node_set nodes = doc.select_nodes(xpathExpr.c_str());

        if (!nodes.empty())
        {
            if (nodes.size() > 1)
            {
                // Case A: Multiple XPath matches → return JSON array
                j = Json::Value(Json::arrayValue);

                for (auto &n : nodes)
                {
                    Json::Value obj;
                    // Always include the node name as the JSON key
                    obj[n.node().name()] = xmlNodeToJson(n.node());
                    j.append(obj);
                }
            }
            else
            {
                // Case B: Single XPath match → return object with node name
                pugi::xml_node n = nodes.begin()->node();
                j[n.name()] = xmlNodeToJson(n);
            }
        }
    }

    // Step 3: Default fallback → use the document’s root element
    if (j.isNull())
    {
        pugi::xml_node rootNode = doc.first_child();
        // Preserve the root element name (e.g., "library")
        j[rootNode.name()] = xmlNodeToJson(rootNode);
    }

    // Step 4: Serialize JSON back to string
    Json::StreamWriterBuilder writer;
    std::string jsonStr = Json::writeString(writer, j);

    // Step 5: Send response
    auto resp = HttpResponse::newHttpResponse();
    resp->setContentTypeCode(ContentType::CT_APPLICATION_JSON);
    resp->setBody(jsonStr);
    callback(resp);
}
