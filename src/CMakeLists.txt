# Optionally glob, but only for CMake 3.12 or later:
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${CChess_SOURCE_DIR}/include/ChessEngine/*.h")
set(PROJECT_INCLUDE_ROOT "${CChess_SOURCE_DIR}/include/ChessEngine")
set(HEADER_LIST "${PROJECT_INCLUDE_ROOT}/piece.h" "${PROJECT_INCLUDE_ROOT}/table.h" "${PROJECT_INCLUDE_ROOT}/team.h")

# Make an automatic library - will be static or dynamic based on user setting
add_library(CChessGame ChessEngine/piece.c ChessEngine/table.c ChessEngine/team.c ${HEADER_LIST})

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
