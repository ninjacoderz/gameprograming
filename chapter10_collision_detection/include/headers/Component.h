#pragma once
#include <SDL3/SDL.h>
#include "InputSystem.h"
class Component
{
public:
	// Constructor
	// (the lower the update order, the earlier the component updates)
	Component(class Actor* owner, int updateOrder = 100);
	// Destructor
	virtual ~Component();
	// Update this component by delta time
	virtual void Update(float deltaTime) {};
	class Actor* GetOwner() { return mOwner; }
	
	int GetUpdateOrder() const { return mUpdateOrder; }
	virtual void OnUpdateWorldTransform(){}
	virtual void ProcessInput(const InputState& state) {};

protected:
	// Owning actor
	class Actor* mOwner;
	// Update order of component
	int mUpdateOrder;
};
