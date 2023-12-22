# Optionally glob, but only for CMake 3.12 or later:
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${CChess_SOURCE_DIR}/include/project/*.h")
set(HEADER_LIST "${CChess_SOURCE_DIR}/include/project/testingStructure.h")

# Make an automatic library - will be static or dynamic based on user setting
add_library(CChessGame testingStructure.c ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(CChessGame PUBLIC ../include)

# This depends on (header only) boost
#target_link_libraries(modern_library PRIVATE Boost::boost)

# All users of this library will need at least C17
target_compile_features(CChessGame PUBLIC c_std_17)

# IDEs should put the headers in a nice place
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST})
