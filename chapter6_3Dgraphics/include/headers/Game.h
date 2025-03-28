#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "VertexArray.h"
#include "Shader.h"
#include "Asteroid.h"

class Game {
    public: 
        Game();
        Game(SDL_Window* window, SDL_GLContext context);
        // Initialize the game
	    virtual bool Initialize(SDL_Window* window, SDL_GLContext context);
        // Shutdown the game
	    void Shutdown();
        void RunLoop();
        void ProcessInput(SDL_Event *event);
        void AddActor(class Actor* actor);
        void RemoveActor(class Actor* actor);
        void AddSprite(class SpriteComponent* sprite);
        void RemoveSprite(class SpriteComponent* sprite);
        void AddAsteroid(Asteroid* ast);
        void RemoveAsteroid(class Asteroid* ast);
        class Texture* GetTexture(const std::string& fileName);
        bool LoadShaders();
        std::vector<class Asteroid*>& GetAsteroids() { return mAsteroids; }
        
    protected:
        void UpdateGame(float detalTime);
        void GenerateOutput();
        virtual void LoadData();
        void UnloadData();
        void CreateSpriteVerts();
       

        SDL_Window* mWindow;
        SDL_GLContext mContext;
        Uint32 mTicksCount;
        
        bool mUpdatingActors;
        std::vector<class Actor*> mPendingActors;
        std::vector<class Actor*> mActors;
        std::vector<class SpriteComponent*> mSprites;

        class VertexArray* mSpriteVerts;
        class Shader* mSpriteShader;
        std::vector<class Asteroid*> mAsteroids;

        // Map of textures loaded
	    std::unordered_map<std::string, class Texture*> mTextures;
};