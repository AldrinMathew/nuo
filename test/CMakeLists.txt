add_executable(nuotest main.cpp)

target_include_directories(nuotest PRIVATE ../include/)
target_link_directories(nuotest PRIVATE ../build/)
target_link_libraries(nuotest PRIVATE nuo)