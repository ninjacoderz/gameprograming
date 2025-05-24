cmake_minimum_required(VERSION 3.10)
project(GameObjects VERSION 1.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(GameObjects Main.cpp 
    src/Actor.cpp
    src/Component.cpp
    src/GameMath.cpp
    src/VertexArray.cpp
    src/Texture.cpp
    src/Shader.cpp
    src/MoveComponent.cpp
    src/CircleComponent.cpp
    src/Random.cpp
    src/MeshComponent.cpp
    src/Renderer.cpp
    src/Mesh.cpp
    src/Game.cpp
    src/PlaneActor.cpp
        src/SpriteComponent.cpp
    src/AudioComponent.cpp
    src/AudioSystem.cpp
    src/SoundEvent.cpp
    src/InputSystem.cpp
    src/FPSActor.cpp
    src/FPSCamera.cpp
    src/CameraComponent.cpp
)

target_include_directories(GameObjects PUBLIC
                            "${PROJECT_SOURCE_DIR}/include/headers"
                            "${PROJECT_SOURCE_DIR}/../SDL_image/include"
                            "${PROJECT_SOURCE_DIR}/../SDL/include"
                            "${PROJECT_SOURCE_DIR}/../SOIL/include"
                            "${PROJECT_SOURCE_DIR}/../rapidjson/include"
                            "${PROJECT_SOURCE_DIR}/../glew/include"
                            "${PROJECT_SOURCE_DIR}/../FMODProgrammersAPI/api/studio/inc"
                            "${PROJECT_SOURCE_DIR}/../FMODProgrammersAPI/api/core/inc"
                            "/usr/local/include"
                          )

if (APPLE)
    find_library(COREFOUNDATION CoreFoundation)
endif()

find_library(SDL3_IMAGE_LOCAL SDL3_image "${PROJECT_SOURCE_DIR}/../SDL_image/build")
find_library(SDL3_LOCAL SDL3 "${PROJECT_SOURCE_DIR}/../SDL/build")
find_library(SOIL_LOCAL SOIL "${PROJECT_SOURCE_DIR}/../SOIL/build")
find_library(FMOD_LOCAL FMOD "${PROJECT_SOURCE_DIR}/../FMODProgrammersAPI/api/core/lib")
find_library(FMOD_STUDIO_LOCAL FMODStudio "${PROJECT_SOURCE_DIR}/../FMODProgrammersAPI/api/studio/lib")
find_library(OPENGL OpenGL)
find_library(GLEW Glew)

target_link_libraries(GameObjects PRIVATE  
                    ${SDL3_LOCAL} 
                    ${SDL3_IMAGE_LOCAL} 
                    ${OPENGL} 
                    ${GLEW} 
                    ${SOIL_LOCAL} 
                    ${COREFOUNDATION} 
                    ${FMOD_LOCAL} 
                    ${FMOD_STUDIO_LOCAL}
                )
