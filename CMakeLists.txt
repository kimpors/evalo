cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(evalo VERSION 1.0.0 LANGUAGES C)

add_executable(evalo src/main.c src/token.c src/parse.c)
target_include_directories(evalo PUBLIC include out)
target_link_libraries(evalo m)

include(CTest)
enable_testing()
