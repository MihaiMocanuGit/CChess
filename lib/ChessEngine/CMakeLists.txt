# Optionally glob, but only for CMake 3.12 or later:
#file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${CChess_SOURCE_DIR}/include/ChessEngine/*.h")
cmake_minimum_required(VERSION 3.26)

set(CHESS_ENGINE_HEADER_LIST "chessStructure.h" "Structural/include/piece.h" "Structural/include/table.h" "Structural/include/team.h"
        "Structural/include/pieceRules.h" "Structural/include/kingMoveValidator.h"
        Structural/src/move.c)

# Make an automatic library - will be static or dynamic based on user setting
add_library(ChessEngine Structural/src/piece.c Structural/src/table.c Structural/src/team.c Structural/src/pieceRules.c Structural/src/kingMoveValidator.c
            ${CHESS_ENGINE_HEADER_LIST} )

# We need this directory, and users of our library will need it too
target_include_directories(ChessEngine PUBLIC Structural/include)

# All users of this library will need at least C17
target_compile_features(ChessEngine PUBLIC c_std_17)