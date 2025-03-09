#include "Tile.h"
#include "SpriteComponent.h"
#include "Game.h"

Tile::Tile(class Game* game)
:Actor(game)
,mParent(nullptr)
,f(0.0f)
,g(0.0f)
,h(0.0f)
,mBlocked(false)
,mSprite(nullptr)
,mTileState(EDefault)
,mSelected(false)
{
    mSprite = new SpriteComponent(this);
    UpdateTexture();
}

void Tile::SetTileState(TileState state)
{
	mTileState = state;
	UpdateTexture();
}

void Tile::ToggleSelect()
{
	mSelected = !mSelected;
	UpdateTexture();
}

void Tile::UpdateTexture()
{
	std::string texture;
	switch (mTileState)
	{
		case EStart:
			texture = "Assets/TileTan.png";
			break;
		case EBase:
			texture = "Assets/TileGreen.png";
			break;
		case EPath:
			if (mSelected)
				texture = "Assets/TileGreySelected.png";
			else
				texture = "Assets/TileGrey.png";
			break;
		case EDefault:
		default:
			if (mSelected)
				texture = "Assets/TileBrownSelected.png";
			else
				texture = "Assets/TileBrown.png";
			break;
	}
	mSprite->SetTexture(GetGame()->GetTexture(texture));
}