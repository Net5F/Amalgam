cmake_minimum_required(VERSION 3.5)

message(STATUS "Configuring SDL_ttf")

find_package(Freetype REQUIRED)

# Add our static library target
add_library(SDL2_ttf-static STATIC
       ${PROJECT_SOURCE_DIR}/../Libraries/SDL_ttf/SDL_ttf.c
       ${PROJECT_SOURCE_DIR}/../Libraries/SDL_ttf/SDL_ttf.h
)
set_target_properties(SDL2_ttf-static PROPERTIES OUTPUT_NAME "SDL2_ttf")

target_include_directories(SDL2_ttf-static
    PUBLIC
        ${PROJECT_SOURCE_DIR}/../Libraries/SDL_ttf
)

target_link_libraries(SDL2_ttf-static
    PRIVATE
        SDL2-static
        Freetype::Freetype
)
