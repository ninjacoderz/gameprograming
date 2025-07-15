#pragma once
#include "MeshComponent.h"
#include "MatrixPalette.h"

class SkeletalMeshComponent : public MeshComponent
{
public:
    SkeletalMeshComponent(class Actor* owner);
    // Draw this mesh component
    void Draw(class Shader* shader) override;

    void Update(float deltaTime) override;

    // Setters
	void SetSkeleton(class Skeleton* sk) { mSkeleton = sk; }

	// Play an animation. Returns the length of the animation
	float PlayAnimation(class Animation* anim, float playRate = 1.0f);

protected:
    void ComputeMatrixPalette();

    class Animation* mAnimation;

    const Skeleton* mSkeleton;
    MatrixPalette mPalette;

    float mAnimPlayRate;
	float mAnimTime;
};
