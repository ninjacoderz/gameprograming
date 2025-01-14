# To build SDL
    ## Build for dynamic lib
    git clone https://github.com/libsdl-org/SDL
    cd SDL
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    cmake --build . --config Release --parallel  
    sudo cmake --install . --config Release

    ## Build framework 
    ### macOS from the command line:

    Run: `cmake -S . -B build && cmake --build build && cmake --install build`

    ### macOS for universal architecture:

    Run: `sudo cmake -S . -B build -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" && sudo cmake --build build && sudo cmake --install build`

# To build main apps ( 1 of cmd bellow )
    gcc -o main.out main/main.c `pkg-config sdl3 --cflags --libs`
    gcc -o main.out main/main.c -F./build/ -lSDL3 -Wl,-rpath,./build
    gcc -o main.out main/main.c -F./build -framework SDL3 -Wl,-rpath,./build
    gcc -o main.out main.c -I../SDL/include -L../SDL/build -Wl,-rpath,../SDL/build -lSDL3

    g++ -o main main.cpp ./src/game.cpp -I../SDL/include -I./include/headers  -L../SDL/build -Wl,-rpath,../SDL/build -lSDL3

    gcc -o main main.cpp ./src/Game.cpp ./src/Component.cpp ./src/Math.cpp  ./src/Actor.cpp  -std=c++17  -stdlib=libc++  -lc++ -I../SDL/include -I../SDL_image/include/SDL3_image -I./include/headers  -L../SDL/build -Wl,-rpath,../SDL/build -lSDL3 -L../SDL_image/build -Wl,-rpath,../SDL_image/build -lSDL3_image 

    ## Build with dynamic 
    gcc -o main main.cpp ./src/Game.cpp ./src/Component.cpp ./src/GameMath.cpp  ./src/Actor.cpp  ./src/SpriteComponent.cpp ./src/BGSpriteComponent.cpp  -std=c++17  -stdlib=libc++  -lc++ -I./include/headers -I../SDL/include -I../SDL_image/include/SDL3_image -L../SDL_image/build -Wl,-rpath,../SDL_image/build -lSDL3_image -L../SDL/build -Wl,-rpath,../SDL/build -lSDL3 

    ## Build with framework

    gcc -o main main.cpp ./src/Game.cpp ./src/Component.cpp ./src/GameMath.cpp  ./src/Actor.cpp  ./src/SpriteComponent.cpp ./src/BGSpriteComponent.cpp  -std=c++17  -stdlib=libc++  -lc++ -I./include/headers -I../SDL/include -I../SDL_image/include/SDL3_image -L../SDL_image/build -Wl,-rpath,../SDL_image/build -lSDL3_image -F../SDL/build -Wl,-rpath,../SDL/build -framework SDL3

# To see the lib compiled with 
    otool -L main.out

# Use SDL_Delay(20); in SDL_AppIterate can reduce CPU usage   