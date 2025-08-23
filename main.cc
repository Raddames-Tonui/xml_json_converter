#include <drogon/drogon.h>

int main() {
    drogon::app().registerHandler(
        "/",                                         
        [](const drogon::HttpRequestPtr &req,
           std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
            auto resp = drogon::HttpResponse::newHttpResponse();
            // resp->setContentTypeCode(drogon::CT_TEXT_HTML);
            // resp->setBody("<h1>Hello, World!</h1>");

            resp->setStatusCode(drogon::k200OK);
            resp->setContentTypeCode(drogon::CT_APPLICATION_JSON);
            resp->setBody(R"({"message": "XML <-> JSON Converter started!"})");
            callback(resp); 
        },
        {drogon::Get} 
    );

    drogon::app().loadConfigFile("config.json");
    std::cout<< "drogon XML<->JSON started!" << std::endl;
    drogon::app().run();

    return 0;
}
