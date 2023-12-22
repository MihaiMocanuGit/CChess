cmake_minimum_required(VERSION 3.26)
project(CChess C)

#project template copied from: https://gitlab.com/CLIUtils/modern-cmake/-/tree/master/examples/extended-project
#                              https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_C_STANDARD 17)

    # Let's ensure -std=cxx instead of -std=gccxx
    set(CMAKE_C_EXTENSIONS OFF)

    # Let's nicely support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_C_FLAGS_DEBUG} -O0 -Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter
     -fsanitize=address,undefined")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address,undefined")

    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

    # Docs only available if this is the main app
    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()



endif ()

add_subdirectory(src)
add_subdirectory(apps)

