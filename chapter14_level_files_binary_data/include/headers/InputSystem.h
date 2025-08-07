
#pragma once
#include <SDL3/SDL.h>
#include "GameMath.h"


enum ButtonState
{
   ENone,
   EPressed,
   EReleased,
   EHeld
};

class KeyboardState
{
public:
   // Friend so InputSystem can easily update it
   friend class InputSystem;
   // Get just the boolean true/false value of key
   bool GetKeyValue(SDL_Scancode keyCode) const;
   // Get a state based on current and previous frame
   ButtonState GetKeyState(SDL_Scancode keyCode) const;
private:
   const bool* mCurrState; // Pointer to Uint8 for SDL_GetKeyboardState
   Uint8 mPrevState[SDL_SCANCODE_COUNT];
};

class MouseState
{   
    public:
        friend class InputSystem;

        // For mouse position
        const Vector2& GetPosition() const { return mMousePos; }
        const Vector2& GetScrollWheel() const { return mScrollWheel; }
        bool IsRelative() const { return mIsRelative; }

        // For buttons
        bool GetButtonValue(int button) const;
        ButtonState GetButtonState(int button) const;

    private:
        // Store current mouse position
        Vector2 mMousePos;
        // Motion of scroll wheel
        Vector2 mScrollWheel;
        // Store button data
        Uint32 mCurrButtons;
        Uint32 mPrevButtons;
        // Are we in relative mouse mode
        bool mIsRelative;
};

struct InputState
{
	KeyboardState Keyboard;
    MouseState Mouse;
};

class InputSystem
{
    public: 
        bool Initialize();
        void Shutdown();
        void PrepareForUpdate();
        void Update();
        void SetRelativeMouseMode(SDL_Window *window, bool value);
        const InputState& GetState() const { return mState; }
        void ProcessEvent(union SDL_Event& event);
    private:
        InputState mState;
};