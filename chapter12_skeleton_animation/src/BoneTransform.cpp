#include "BoneTransform.h"

Matrix4 BoneTransform::ToMatrix() const 
{
    Matrix4 root = Matrix4::CreateFromQuaternion(mRotation);
    Matrix4 trans = Matrix4::CreateTranslation(mTranslation);
    return root * trans;
}

BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float f) 
{
    BoneTransform retVal;
    retVal.mRotation = Quaternion::Slerp(a.mRotation, b.mRotation, f);
    retVal.mTranslation = Vector3::Lerp(a.mTranslation, b.mTranslation, f);
    return retVal;
}