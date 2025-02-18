
#include "Actor.h"
#include "GameMath.h"
class Asteroid: public Actor 
{
    public:
        Asteroid(class Game* game);
        ~Asteroid();
        void SetPosition(const Vector2& pos) override;
};