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
)

add_executable(simulator ${SOURCES})