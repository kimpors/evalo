cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(evalo VERSION 1.0.0 LANGUAGES C)
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_BUILD_TYPE Debug)
set(C_STANDARD 23)

add_executable(${PROJECT_NAME} src/main.c src/token.c src/parse.c src/arg.c src/format.c)

if (CMAKE_SYSTEM MATCHES Windows)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
endif()

if (CMAKE_SYSTEM MATCHES Linux)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wuninitialized)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC include)

if (CMAKE_SYSTEM MATCHES Linux)
target_link_libraries(${PROJECT_NAME} m)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

include(CTest)
enable_testing()

add_executable(test_arg test/test_arg.c src/arg.c)
target_include_directories(test_arg PUBLIC include)

add_test(NAME Args1 COMMAND test_arg)
add_test(NAME Args2 COMMAND test_arg "--help")
add_test(NAME Args3 COMMAND test_arg "12 + 22")
add_test(NAME Args4 COMMAND test_arg "12+22")
add_test(NAME Args5 COMMAND test_arg "-p" "12+22" "12")
add_test(NAME Args6 COMMAND test_arg "-p" "12" "12+22")
add_test(NAME Args7 COMMAND test_arg "-ecph" "12" "12+22")

add_executable(test_eval test/test_eval.c src/token.c src/parse.c)
target_include_directories(test_eval PUBLIC include)

if (CMAKE_SYSTEM MATCHES Linux)
target_link_libraries(test_eval m)
endif()

add_test(NAME Eval1 COMMAND test_eval "2 + 2" "4")
add_test(NAME Eval2 COMMAND test_eval "1.2 + 1.3" "2.5")

add_test(NAME Eval3 COMMAND test_eval "2 - 2" "0")
add_test(NAME Eval4 COMMAND test_eval "1.2 - 1.3" "-0.1")

add_test(NAME Eval5 COMMAND test_eval "2 * 2" "4")
add_test(NAME Eval6 COMMAND test_eval "1.2 * 1.3" "1.56")

add_test(NAME Eval7 COMMAND test_eval "2 / 2" "1")
add_test(NAME Eval8 COMMAND test_eval "1.2 / 1.2" "1")

add_test(NAME Eval9 COMMAND test_eval "2^3" "8")
add_test(NAME Eval10 COMMAND test_eval "1.2^1.3" "1.27")

add_test(NAME Eval11 COMMAND test_eval "1/0" "inf")
add_test(NAME Eval12 COMMAND test_eval "12 + 22" "34")
add_test(NAME Eval13 COMMAND test_eval "12+22" "34")
add_test(NAME Eval14 COMMAND test_eval "2 * (2 + 4) * 2" "24")
add_test(NAME Eval15 COMMAND test_eval "2*(2+4)*2" "24")
add_test(NAME Eval16 COMMAND test_eval "2 * ((120 / 2) + (3 ^ 3)) * 2" "348")
add_test(NAME Eval17 COMMAND test_eval "2*((120/2)+(3^3))*2" "348")
