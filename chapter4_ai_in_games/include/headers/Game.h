#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "GameMath.h"
class Game {
    public: 
        Game();
        Game(SDL_Window* window, SDL_Renderer* renderer);
        // Initialize the game
	    virtual bool Initialize(SDL_Window* _window, SDL_Renderer* _renderer);
        // Shutdown the game
	    void Shutdown();
        void RunLoop();
        void ProcessInput(SDL_Event *event);
        void AddActor(class Actor* actor);
        void RemoveActor(class Actor* actor);
        SDL_Texture* GetTexture(const std::string& fileName);
        void AddSprite(class SpriteComponent* sprite);
        void RemoveSprite(class SpriteComponent* sprite);
        std::vector<class Enemy*>& GetEnemies() { return mEnemies; }
        class Grid* GetGrid() { return mGrid; }
        class Enemy* GetNearestEnemy(const Vector2& pos);

    protected:
        void UpdateGame(float detalTime);
        void GenerateOutput();
        virtual void LoadData();
        void UnloadData();

        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;
        Uint32 mTicksCount;
        
        bool mUpdatingActors;
        std::vector<class Actor*> mPendingActors;
        std::vector<class Actor*> mActors;

        // Map of textures loaded
	    std::unordered_map<std::string, SDL_Texture*> mTextures;

        // All the sprite components drawn
        std::vector<class SpriteComponent*> mSprites;
        std::vector<class Enemy*> mEnemies;

        class Grid* mGrid;
	    float mNextEnemy;

};