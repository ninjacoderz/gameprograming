#pragma once
#include "Component.h"
#include <unordered_map>
#include <string>
#include "AIState.h"

class AIComponent :public Component
{
private:
    // Maps name of state to AIState instance
	std::unordered_map<std::string, class AIState*> mStateMap;
	// Current state we're in
	class AIState* mCurrentState;
public:
    AIComponent(class Actor* owner);
	
	void Update(float deltaTime) override;
	void ChangeState(const std::string& name);
	void RegisterState(class AIState* state);
};     
