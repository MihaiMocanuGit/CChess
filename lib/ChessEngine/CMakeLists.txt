# Optionally glob, but only for CMake 3.12 or later:
#file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${CChess_SOURCE_DIR}/include/ChessEngine/*.h")

set(CHESS_ENGINE_INCLUDE_ROOT "${CChess_SOURCE_DIR}/include/ChessEngine")
set(CHESS_ENGINE_HEADER_LIST "${CHESS_ENGINE_INCLUDE_ROOT}/piece.h" "${CHESS_ENGINE_INCLUDE_ROOT}/table.h" "${CHESS_ENGINE_INCLUDE_ROOT}/team.h")

# Make an automatic library - will be static or dynamic based on user setting
add_library(ChessEngine src/piece.c src/table.c src/team.c ${CHESS_ENGINE_HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(ChessEngine PUBLIC ../include/ChessEngine)

# This depends on (header only) boost
#target_link_libraries(modern_library PRIVATE Boost::boost)

# All users of this library will need at least C17
target_compile_features(ChessEngine PUBLIC c_std_17)