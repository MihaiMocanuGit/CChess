# Optionally glob, but only for CMake 3.12 or later:
#file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${CChess_SOURCE_DIR}/include/ChessEngine/*.h")
cmake_minimum_required(VERSION 3.26)

set(CHESS_ENGINE_HEADER_LIST "chessStructure.h" "include/piece.h" "include/table.h" "include/team.h"
        "include/pieceRules.h" "include/kingMoveValidator.h")

# Make an automatic library - will be static or dynamic based on user setting
add_library(ChessEngine src/piece.c src/table.c src/team.c src/pieceRules.c src/kingMoveValidator.c
            ${CHESS_ENGINE_HEADER_LIST} )

# We need this directory, and users of our library will need it too
target_include_directories(ChessEngine PUBLIC include)

# All users of this library will need at least C17
target_compile_features(ChessEngine PUBLIC c_std_17)