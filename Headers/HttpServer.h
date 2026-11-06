#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <string>

class HttpServer {
public:
    HttpServer(const std::string& frontendDirectory,
               const std::string& dataDirectory,
               int port = 8080);

    void run();

private:
    std::string frontendDirectory;
    std::string dataDirectory;
    int port;
};

#endif
