target_include_directories(${PROJECT_NAME} PRIVATE ../lib/ChessEngine)

target_include_directories(${PROJECT_NAME} PRIVATE ../include)
set(SOURCES sdlSurfaces.c sdlTextures.c ../include/sdlSurfaces.h ../include/sdlTextures.h)

add_library(AppSources ${SOURCES})