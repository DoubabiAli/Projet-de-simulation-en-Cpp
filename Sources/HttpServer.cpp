#include "../Headers/HttpServer.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace {

#ifdef _WIN32
using Socket = SOCKET;
const Socket invalidSocket = INVALID_SOCKET;
#else
using Socket = int;
const Socket invalidSocket = -1;
#endif

void closeSocket(Socket socket) {
#ifdef _WIN32
    closesocket(socket);
#else
    close(socket);
#endif
}

std::string jsonEscape(const std::string& value) {
    std::string escaped;
    for (char character : value) {
        if (character == '\\' || character == '"') {
            escaped += '\\';
        }
        escaped += character;
    }
    return escaped;
}

std::string latestMeasurement(const std::filesystem::path& dataDirectory,
                              const std::string& sensorType) {
    const auto filePath = dataDirectory / (sensorType + ".csv");
    std::ifstream file(filePath);
    if (!file) {
        return "{\"value\":null,\"date\":null,\"error\":\"file not found\"}";
    }

    std::string line;
    std::string latest;
    while (std::getline(file, line)) {
        if (!line.empty() && line.find("Date;Heure;") != 0) {
            latest = line;
        }
    }

    if (latest.empty()) {
        return "{\"value\":null,\"date\":null}";
    }

    std::stringstream row(latest);
    std::string date;
    std::string time;
    std::string sensorId;
    std::string type;
    std::string value;
    std::getline(row, date, ';');
    std::getline(row, time, ';');
    std::getline(row, sensorId, ';');
    std::getline(row, type, ';');
    std::getline(row, value, ';');

    return "{\"value\":\"" + jsonEscape(value) + "\",\"date\":\""
        + jsonEscape(date + " " + time) + "\"}";
}

std::string sensorsJson(const std::filesystem::path& dataDirectory) {
    return "{\"temperature\":" + latestMeasurement(dataDirectory, "Temperature")
        + ",\"humidity\":" + latestMeasurement(dataDirectory, "Humidity")
        + ",\"light\":" + latestMeasurement(dataDirectory, "Light")
        + ",\"sound\":" + latestMeasurement(dataDirectory, "Sound") + "}";
}

std::string contentType(const std::filesystem::path& path) {
    if (path.extension() == ".html") return "text/html; charset=utf-8";
    if (path.extension() == ".js") return "application/javascript; charset=utf-8";
    if (path.extension() == ".css") return "text/css; charset=utf-8";
    return "text/plain; charset=utf-8";
}

std::string httpResponse(const std::string& status,
                         const std::string& type,
                         const std::string& body) {
    return "HTTP/1.1 " + status + "\r\nContent-Type: " + type
        + "\r\nContent-Length: " + std::to_string(body.size())
        + "\r\nConnection: close\r\n\r\n" + body;
}

std::string handleRequest(const std::string& request,
                          const std::filesystem::path& frontendDirectory,
                          const std::filesystem::path& dataDirectory) {
    const auto requestEnd = request.find("\r\n");
    const std::string requestLine = request.substr(0, requestEnd);
    std::stringstream requestStream(requestLine);
    std::string method;
    std::string path;
    requestStream >> method >> path;

    if (method != "GET") {
        return httpResponse("405 Method Not Allowed", "text/plain", "Method not allowed");
    }

    if (path == "/api/sensors") {
        return httpResponse("200 OK", "application/json; charset=utf-8",
                            sensorsJson(dataDirectory));
    }

    if (path.find("..") != std::string::npos) {
        return httpResponse("400 Bad Request", "text/plain", "Invalid path");
    }

    const std::string relativePath = path == "/" ? "index.html" : path.substr(1);
    const auto filePath = frontendDirectory / relativePath;
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        return httpResponse("404 Not Found", "text/plain", "Not found");
    }

    std::ostringstream body;
    body << file.rdbuf();
    return httpResponse("200 OK", contentType(filePath), body.str());
}

} // namespace

HttpServer::HttpServer(const std::string& frontendDirectory,
                       const std::string& dataDirectory,
                       int port)
    : frontendDirectory(frontendDirectory), dataDirectory(dataDirectory), port(port) {}

void HttpServer::run() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("Impossible d'initialiser Winsock.");
    }
#endif

    const Socket serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == invalidSocket) {
        throw std::runtime_error("Impossible de creer le socket HTTP.");
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(static_cast<unsigned short>(port));

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0
        || listen(serverSocket, 10) < 0) {
        closeSocket(serverSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        throw std::runtime_error("Impossible de demarrer le serveur HTTP.");
    }

    std::cout << "Dashboard disponible sur http://localhost:" << port << std::endl;

    while (true) {
        const Socket clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == invalidSocket) {
            continue;
        }

        char buffer[8192] = {};
        const int received = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (received > 0) {
            const std::string response = handleRequest(
                std::string(buffer, received), frontendDirectory, dataDirectory);
            send(clientSocket, response.c_str(), static_cast<int>(response.size()), 0);
        }
        closeSocket(clientSocket);
    }
}
