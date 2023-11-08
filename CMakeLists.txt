cmake_minimum_required(VERSION 3.1)

project(astra-test)

set(CXX_STANDARD 17)

set(LIBS_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(SERVER_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/server)
set(CLIENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/client)

if (APPLE)
    add_definitions(-D_DARWIN)
    message(STATUS DARWIN)
elseif (UNIX)
    add_definitions(-D_LINUX)
    message(STATUS UNIX)
elseif(WIN32)
    add_definitions(-DWIN32)
    message(STATUS WIN32)
endif ()


add_subdirectory(server)
add_subdirectory(lib)