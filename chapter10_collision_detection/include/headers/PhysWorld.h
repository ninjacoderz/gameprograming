#pragma once
#include <vector>
#include <functional>
#include "GameMath.h"
#include "Collision.h"

class PhysWorld
{
public:
	PhysWorld(class Game* game);

    struct  CollisionInfo
    {
        Vector3 mPoint;          // Point of collision
        Vector3 mNormal;         // Normal at collision
        class BoxComponent* mBox; // Component collided with
        class Actor* mActor;      // Owning actor of component
    };
    bool SegmentCast(const LineSegment& l, CollisionInfo& outColl);
    // Tests collisions using naive pairwise
	void TestPairwise(std::function<void(class Actor*, class Actor*)> f);
	// Test collisions using sweep and prune
	void TestSweepAndPrune(std::function<void(class Actor*, class Actor*)> f);

	// Add/remove box components from world
	void AddBox(class BoxComponent* box);
	void RemoveBox(class BoxComponent* box);
private:
    class Game* mGame;
    std::vector<class BoxComponent*> mBoxes;
};