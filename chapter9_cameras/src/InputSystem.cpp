#include "InputSystem.h"
#include <SDL3/SDL.h>

bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const
{
    return mCurrState[keyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const
{
    if (mPrevState[keyCode] == 0)
    {
        if (mCurrState[keyCode] == 0)
        {
            return ENone;
        }
        else
        {
            return EPressed;
        }
    }
    else // Prev state must be 1
    {
        if (mCurrState[keyCode] == 0)
        {
            return EReleased;
        }
        else
        {
            return EHeld;
        }
    }
}

bool InputSystem::Initialize()
{
    // Keyboard
    // Assign current state pointer
    mState.Keyboard.mCurrState = SDL_GetKeyboardState(NULL);
    // Clear previous state memory
    memset(mState.Keyboard.mPrevState, 0,
      SDL_SCANCODE_COUNT);

   return true;
}

void InputSystem::PrepareForUpdate()
{
    // Copy current state to previous
    // Keyboard
    memcpy(mState.Keyboard.mPrevState,
      mState.Keyboard.mCurrState,
      SDL_SCANCODE_COUNT);

    // Mouse
    mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
    mState.Mouse.mScrollWheel = Vector2::Zero;

}

void InputSystem::SetRelativeMouseMode( SDL_Window *window, bool value)
{
    bool set = value ? true : false;
    SDL_SetWindowRelativeMouseMode(window, set);

    mState.Mouse.mIsRelative = value;
}

void InputSystem::Update() {
    // Mouse
    float x = 0, y = 0;
    if (mState.Mouse.mIsRelative)
	{
		mState.Mouse.mCurrButtons = 
			SDL_GetRelativeMouseState(&x, &y);
	}
	else
	{
		mState.Mouse.mCurrButtons = 
			SDL_GetMouseState(&x, &y);
	}

    mState.Mouse.mMousePos.x = x;
    mState.Mouse.mMousePos.y = y;
}

void InputSystem::ProcessEvent(SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_EVENT_MOUSE_WHEEL:
        mState.Mouse.mScrollWheel = Vector2( event.wheel.x, event.wheel.y);
        break;
    default:
        break;
    }
}

void InputSystem::Shutdown()
{
}