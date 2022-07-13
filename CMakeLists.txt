cmake_minimum_required(VERSION 3.16.3)

PROJECT(nuo
        VERSION "0.2.0"
        DESCRIPTION "Nuo Library for modern C++"
        HOMEPAGE_URL "https://github.com/aldrinmathew/nuo")

SET(CMAKE_CXX_COMPILER /usr/lib/llvm-14/bin/clang++)

if(CMAKE_BUILD_TYPE STREQUAL Release)
        SET(IS_RELEASE ON)
else()
        SET(IS_RELEASE OFF)
endif()

if(IS_RELEASE)
        SET(CMAKE_CXX_FLAGS "-std=c++20 -fexceptions")
else()
        SET(CMAKE_CXX_FLAGS "-g -std=c++20 -fexceptions -Wall")
endif()

add_library(${PROJECT_NAME}
        src/exception.cpp
        src/json.cpp)

add_subdirectory(test)

target_include_directories(${PROJECT_NAME} PRIVATE include/)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(DIRECTORY include/nuo DESTINATION include)
install(DIRECTORY include/nuo DESTINATION include)