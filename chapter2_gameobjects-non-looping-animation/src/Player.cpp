#include "Player.h"
#include "Game.h"

Player::Player(Game *game): Actor(game)
,mRightSpeed(0.0f)
,mDownSpeed(0.0f)
{
    AnimSpriteComponent* asc = new AnimSpriteComponent(this);
    std::vector<SDL_Texture*> idle = {
        game->GetTexture("Assets/Character06.png")
    };

    std::vector<SDL_Texture*> walk = {
            game->GetTexture("Assets/Character01.png"),
            game->GetTexture("Assets/Character02.png"),
            game->GetTexture("Assets/Character03.png"),
            game->GetTexture("Assets/Character04.png"),
            game->GetTexture("Assets/Character05.png"),
            game->GetTexture("Assets/Character06.png")
    };

    std::vector<SDL_Texture*> jump = {
            game->GetTexture("Assets/Character07.png"),
            game->GetTexture("Assets/Character08.png"),
            game->GetTexture("Assets/Character09.png"),
            game->GetTexture("Assets/Character10.png"),
            game->GetTexture("Assets/Character11.png"),
            game->GetTexture("Assets/Character12.png"),
            game->GetTexture("Assets/Character13.png"),
            game->GetTexture("Assets/Character14.png"),
            game->GetTexture("Assets/Character15.png")
    };

    std::vector<SDL_Texture*> punch = {
            game->GetTexture("Assets/Character16.png"),
            game->GetTexture("Assets/Character17.png"),
            game->GetTexture("Assets/Character18.png")
    };

    asc->SetAnimTextures(IDLE, idle);
    asc->SetAnimTextures(WALK, walk);
    asc->SetAnimTextures(JUMP, jump);
    asc->SetAnimTextures(PUNCH, punch);

    mAnim = asc;
}

void Player::UpdateActor(float deltaTime) {
    Actor::UpdateActor(deltaTime);
    // Update position based on speeds and delta time
    Vector2 pos = GetPosition();
    pos.x += mRightSpeed * deltaTime;
    pos.y += mDownSpeed * deltaTime;
    // Restrict position to left half of screen
    if(pos.x < 25.0f){
        pos.x = 25.0f;
    }
    else if(pos.x > 500.0f){
        pos.x = 500.0f;
    }
    if(pos.y < 25.0f){
        pos.y = 25.0f;
    }
    else if(pos.y > 743.0f){
        pos.y = 743.0f;
    }
    SetPosition(pos);
}

void Player::ProcessKeyboard(const SDL_Scancode state )
{
	mRightSpeed = 0.0f;
	mDownSpeed = 0.0f;
	// right/left
	bool continueAnim = false;
    if(state == SDL_SCANCODE_D ){
        mRightSpeed += 250.0f;
        mAnim->SetCurrAnim(WALK);
        continueAnim = true;
    }
    if(state == SDL_SCANCODE_A){
        mRightSpeed -= 250.0f;
        mAnim->SetCurrAnim(WALK);
        continueAnim = true;
    }
    if(state == SDL_SCANCODE_S){
        mDownSpeed += 300.0f;
        mAnim->SetCurrAnim(WALK);
        continueAnim = true;
    }
    if(state == SDL_SCANCODE_W){
        mDownSpeed -= 300.0f;
        mAnim->SetCurrAnim(WALK);
        continueAnim = true;
    }
    if(state == SDL_SCANCODE_SPACE){
        mAnim->SetCurrAnim(JUMP);
        mAnim->SetAnimOnce();
        continueAnim = true;
    }
    if(state == SDL_SCANCODE_F){
        mAnim->SetCurrAnim(PUNCH);
        mAnim->SetAnimOnce();
        continueAnim = true;
    }
    if(!continueAnim) {
        mAnim->SetCurrAnim(IDLE);
    }
}