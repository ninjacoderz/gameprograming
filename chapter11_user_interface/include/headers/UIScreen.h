#include <string>
#include "InputSystem.h"

class UIScreen 
{
    public:
        UIScreen(class Game* game);
        virtual ~UIScreen();

        virtual void Update(float deltaTime);
        virtual void Draw(class Shader* shader);
        virtual void ProcessInput( const InputState& state);
        virtual void HandleKeyPress(int key);
        
        enum UIState{EActive, EClosing};

        void Close();

        UIState GetState() const { return mState; };

        void SetTitle ( const std::string& text, const Vector3& color = Color::White, int pointSize = 400 );

    protected:
        
        void DrawTexture( class Shader* shader, class Texture* texture, 
                            const Vector2& offset = Vector2::Zero, float scale = 1.0f);
        
        class Game* mGame;
        class Font* mFont;
        class Texture* mTitle;
        Vector2 mTitlePos;
        UIState mState;


};