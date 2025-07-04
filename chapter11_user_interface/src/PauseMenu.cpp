#include "PauseMenu.h"
#include "Game.h"
#include "DialogBox.h"
#include <SDL3/SDL.h>
#include "Renderer.h"

PauseMenu::PauseMenu(Game* game)
    :UIScreen(game)
{
    SDL_Log("Init Pause Menu");
    mGame->SetState(Game::EPaused);
    SetRelativeMouseMode(mGame->GetRenderer()->getWindow(), false);
    SetTitle("PauseTitle");

    AddButton("ResumeButton", [this]() {
		Close();
	});

    AddButton("QuitButton", [this]() { 
		new DialogBox(mGame, "QuitText",
			[this]() {
				mGame->SetState(Game::EQuit);
		});
	});
}

PauseMenu::~PauseMenu()
{
	SetRelativeMouseMode(mGame->GetRenderer()->getWindow(), true);
	mGame->SetState(Game::EGameplay);
}

void PauseMenu::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	
	if (key == SDLK_ESCAPE)
	{
		Close();
	}
}
