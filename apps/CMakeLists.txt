add_executable(CChess CChess.c)
target_compile_features(CChess PRIVATE c_std_17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(CChess PRIVATE  ${SDL2_LIBRARIES})
