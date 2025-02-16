cmake_minimum_required(VERSION 3.10)
project(GameObjects VERSION 1.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(GameObjects Main.cpp 
                    src/Actor.cpp
                    src/BGSpriteComponent.cpp
                    src/Component.cpp
                    src/Game.cpp
                    src/GameMath.cpp
                    src/SpriteComponent.cpp
                    src/AnimSpriteComponent.cpp
                    src/MultiAnimSpriteComponent.cpp
                    src/TileMapGame.cpp
                    src/SpriteSheetActor.cpp
                    src/TileMapComponent.cpp
                    src/Ship.cpp
                )

target_include_directories(GameObjects PUBLIC
                          "${PROJECT_SOURCE_DIR}/include/headers"
                          "${PROJECT_SOURCE_DIR}/../SDL_image/include"
                          "${PROJECT_SOURCE_DIR}/../SDL/include"
                          )

find_library(SDL3_IMAGE_LOCAL SDL3_image "${PROJECT_SOURCE_DIR}/../SDL_image/build")
find_library(SDL3_LOCAL SDL3 "${PROJECT_SOURCE_DIR}/../SDL/build")

target_link_libraries(GameObjects PRIVATE ${SDL3_IMAGE_LOCAL} ${SDL3_LOCAL} )
