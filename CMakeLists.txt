cmake_minimum_required(VERSION 3.10)

project(SimulatorIOT VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

include_directories(Headers)

set(SOURCES
    Sources/main.cpp
    Sources/Server.cpp
    Sources/Sensor.cpp
    Sources/Scheduler.cpp
    Sources/TemperatureSensor.cpp
    Sources/HumiditySensor.cpp
    Sources/SoundSensor.cpp
    Sources/LightSensor.cpp
    Sources/HttpServer.cpp
)

add_executable(simulator ${SOURCES}
    Headers/LightSensor.h)

target_compile_definitions(simulator PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

if (WIN32)
    target_link_libraries(simulator PRIVATE ws2_32)
endif()