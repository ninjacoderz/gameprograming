#pragma once
#include "Actor.h"
#include <vector>

class Tile: public Actor
{
    public:
        friend class Grid;
        enum TileState
        {
            EDefault,
            EPath,
            EStart,
            EBase
        };
        Tile(class Game* game);
        void SetTileState(TileState state);
        TileState GetTileState() const { return mTileState; }
        void ToggleSelect();
        const Tile* GetParent() const { return mParent; }

    private:
        std::vector<Tile*> mAdjacent;
        Tile* mParent;
        float f;
        float g;
        float h;
        bool mInOpenSet;
        bool mInClosedSet;
        bool mBlocked;

        void UpdateTexture();
        class SpriteComponent* mSprite;
        TileState mTileState;
        bool mSelected;
};