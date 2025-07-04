#include "HUD.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "PhysWorld.h"
#include "FPSActor.h"
#include <algorithm>

HUD::HUD(Game* game)
	:UIScreen(game)
{
    Renderer* r = mGame->GetRenderer();

}

HUD::~HUD()
{

}

void HUD::Update(float deltaTime)
{
    UIScreen::Update(deltaTime);
}

void HUD::Draw(Shader* shader) {

}

