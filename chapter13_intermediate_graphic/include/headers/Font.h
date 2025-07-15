
#pragma once
#include <string>
#include <unordered_map>
#include <SDL_ttf.h>
#include "GameMath.h"

class Font {
    public:
        Font(class Game* game);
        ~Font();
        bool Load(const std::string& fileName);
        void Unload();
        class Texture* RenderText( const std::string& text,
            const Vector3& color = Color::White,
            int pointSize = 30);
    private:   
        std::unordered_map<int, TTF_Font*> mFontData;
        class Game* mGame;
};