add_executable(CChess CChess.c)
target_compile_features(CChess PRIVATE c_std_17)

target_link_libraries(CChess PRIVATE CChessGame)
