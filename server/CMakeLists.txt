cmake_minimum_required(VERSION 3.1)

project(server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wfloat-conversion -pedantic -pthread")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(server main.cpp server.cpp server.h block.h block.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${LIBS_FOLDER})
target_link_libraries(server PRIVATE clsocket)