target_include_directories(${PROJECT_NAME} PRIVATE ../lib/ChessEngine)

target_include_directories(${PROJECT_NAME} PRIVATE ../include)
set(SOURCES objectSurfaces.c objectTextures.c ../include/objectSurfaces.h ../include/objectTextures.h
        screen.c
        ../include/screen.h)

add_library(AppSources ${SOURCES})