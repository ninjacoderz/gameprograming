
#include <fstream>
#include <sstream>
#include <iostream>
#include "GameMath.h"
#include "SpriteComponent.h"

class TileMapComponent: public SpriteComponent {
    public:
        TileMapComponent(class Actor* owner, int drawOrder = 10);
        void Draw(SDL_Renderer* renderer) override;
        void SetTexture(SDL_Texture* texture) override { mTileMapTexture = texture; }
        void LoadTileMap_CSV(std::string fileName);
        void SetcreenSite(Vector2 &size) { mScreenSize = size; }
    private: 
        SDL_Texture* mTileMapTexture;
        std::vector<std::vector<int>> csvMap;
        Vector2 mScreenSize;
};