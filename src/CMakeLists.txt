target_include_directories(${PROJECT_NAME} PRIVATE ../lib/ChessEngine)

target_include_directories(${PROJECT_NAME} PRIVATE ../include)
set(SOURCES objectSurfaces.c objectTextures.c ../include/objectSurfaces.h ../include/objectTextures.h
        Screen.c
        ../include/Screen.h)

add_library(AppSources ${SOURCES})