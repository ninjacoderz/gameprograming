#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>
#include <vector>

class Game {
    public: 
        Game(SDL_Window* window, SDL_Renderer* renderer);
        // Initialize the game
	    bool Initialize(SDL_Window* _window, SDL_Renderer* _renderer);
        // Shutdown the game
	    void Shutdown();
        void RunLoop();
        void ProcessInput(SDL_Event *event);
        void AddActor(class Actor* actor);
        void RemoveActor(class Actor* actor);
        SDL_Texture* GetTexture(const std::string& fileName);
        void AddSprite(class SpriteComponent* sprite);
        void RemoveSprite(class SpriteComponent* sprite);
    private:
        void UpdateGame(float detalTime);
        void GenerateOutput();
        void LoadData();
        void UnloadData();

        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;
        SDL_GLContext mContext;
        Uint32 mTicksCount;
        
        bool mUpdatingActors;
        std::vector<class Actor*> mPendingActors;
        std::vector<class Actor*> mActors;

        // Map of textures loaded
	    std::unordered_map<std::string, SDL_Texture*> mTextures;

        // All the sprite components drawn
        std::vector<class SpriteComponent*> mSprites;

};