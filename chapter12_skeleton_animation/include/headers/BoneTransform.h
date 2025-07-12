struct  BoneTransform
{
    /* data */
    Quaternion mRotation;
    Vector3 mTranslation;
    Matrix4 ToMatrix() const;
};
