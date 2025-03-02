cmake_minimum_required(VERSION 3.31)

set(CMAKE_C_COMPILER "clang")
project(SimpleDB
    VERSION 0.1
    LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>")
add_compile_options(-Wall -Wextra -g -D_CRT_SECURE_NO_WARNINGS)

enable_testing()

add_executable(devdb main.c filemgr.c block.c page.c)

add_subdirectory(test)

add_custom_command(
    OUTPUT "${CMAKE_SOURCE_DIR}/compile_commands.json"
    WORKING_DIR ${CMAKE_BINARY_DIR}
    DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
    COMMAND copy compile_commands.json \"${CMAKE_SOURCE_DIR}/compile_commands.json\"
)

add_custom_target(lsp
    DEPENDS "${CMAKE_SOURCE_DIR}/compile_commands.json")

