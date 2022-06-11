cmake_minimum_required(VERSION 3.16.3)

PROJECT(nuo
        VERSION "0.1.0"
        DESCRIPTION "Nuo Library for modern C++"
        HOMEPAGE_URL "https://github.com/aldrinmathew/nuo")

add_library(${PROJECT_NAME}
                src/exception.cpp)

add_subdirectory(test)

target_include_directories(${PROJECT_NAME} PRIVATE include/)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(DIRECTORY include/nuo DESTINATION include)
install(DIRECTORY include/nuo DESTINATION include)