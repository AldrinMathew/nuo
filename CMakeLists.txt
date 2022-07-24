cmake_minimum_required(VERSION 3.16.3)

PROJECT(nuo
        VERSION "0.3.0"
        DESCRIPTION "Nuo Library for modern C++"
        HOMEPAGE_URL "https://github.com/aldrinmathew/nuo")

if(WIN32)
        add_compile_definitions(PLATFORM_IS_WINDOWS=true)
        add_compile_definitions(PLATFORM_IS_UNIX=false)
        #
else()
        add_compile_definitions(PLATFORM_IS_WINDOWS=false)
        add_compile_definitions(PLATFORM_IS_UNIX=true)
        SET(CMAKE_CXX_COMPILER /usr/lib/llvm-14/bin/clang++)
endif()

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
        src/json.cpp
        src/json_parser.cpp)

add_subdirectory(test)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_include_directories(${PROJECT_NAME} PRIVATE include/)

if(WIN32)
        install(TARGETS ${PROJECT_NAME} DESTINATION C:\\msys64\\clang64\\lib)
        install(DIRECTORY include/nuo DESTINATION C:\\msys64\\clang64\\include)
else()
        install(TARGETS ${PROJECT_NAME} DESTINATION lib)
        install(DIRECTORY include/nuo DESTINATION include)
endif()