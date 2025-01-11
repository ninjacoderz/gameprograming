To build SDL
    git clone https://github.com/libsdl-org/SDL
    cd SDL
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    cmake --build . --config Release --parallel  
    sudo cmake --install . --config Release

To build main apps ( 1 of cmd bellow )
    gcc -o main.out main/main.c `pkg-config sdl3 --cflags --libs`
    gcc -o main.out main/main.c -F./build/ -lSDL3 -Wl,-rpath,./build
    gcc -o main.out main/main.c -F./build -framework SDL3 -Wl,-rpath,./build
    gcc -o main.out main.c -I../SDL/include -L../SDL/build -Wl,-rpath,../SDL/build -lSDL3

    g++ -o main main.cpp ./src/game.cpp -I../SDL/include -I./include/headers  -L../SDL/build -Wl,-rpath,../SDL/build -lSDL3
    
    g++ -o main main.cpp ./src/game.cpp -I../SDL/include -I./include/headers  -L../SDL/build -Wl,-rpath,../SDL/build -lSDL3 -I/usr/local/Cellar/glew/2.2.0_1/include/GL/ -L/usr/local/Cellar/glew/2.2.0_1/lib  -lGLEW  -framework OpenGL
    
To see the lib compiled with 
    otool -L main.out

Use SDL_Delay(20); in SDL_AppIterate can reduce CPU usage   

To find out the linked lib run the command 
    pkg-config --libs --static glew

To find out what linked with execute 
    otool -L main