target_include_directories(${PROJECT_NAME} PRIVATE ../lib/ChessEngine)

target_include_directories(${PROJECT_NAME} PRIVATE ../include)
set(SOURCES objectSurfaces.c objectTextures.c ../include/objectSurfaces.h ../include/objectTextures.h
        screen.c
        ../include/screen.h
        mouseControl.c
        ../include/mouseControl.h
        vector.c
        ../include/vector.h
        UiComponent.c
        ../include/UiComponent.h

)

add_library(AppSources ${SOURCES})