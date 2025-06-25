//
// Created by Binh Nguyen Thanh on 24/05/2025.
//

#include "Component.h"
#include "GameMath.h"

class CameraComponent : public Component {
public:
    CameraComponent(class Actor* owner, int updateOrder = 200);
protected:
    void SetViewMatrix(const Matrix4& viewMatrix) const;
};

