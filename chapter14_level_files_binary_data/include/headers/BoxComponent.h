
#pragma once
#include "Component.h"
#include "Actor.h"
#include "Collision.h"

class BoxComponent : public Component {
public:
    BoxComponent(class Actor* owner, int updateOrder = 100);
    ~BoxComponent();  
    void OnUpdateWorldTransform() override;
    void SetObjectBox(const AABB& box) { mObjectBox = box; };
    const AABB& GetWorldBox() const { return mWorldBox; };
    void SetShouldRotate(bool shouldRotate) { mShouldRotate = shouldRotate; }

private:
    AABB mObjectBox; // Object space box
    AABB mWorldBox;  // World space box
    bool mShouldRotate; // Whether to rotate the box with the actor
};