#include <drogon/drogon.h>

int main() {
    drogon::app().registerHandler(
        "/",                                         
        [](const drogon::HttpRequestPtr &req,
           std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setContentTypeCode(drogon::CT_TEXT_HTML);
            resp->setBody("<h1>Hello, World!</h1>");
            callback(resp); 
        },
        {drogon::Get} 
    );

    drogon::app().loadConfigFile("config.json");
    std::cout<< "drogon started!" << std::endl;
    drogon::app().run();

    return 0;
}
