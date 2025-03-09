#include "AIComponent.h"
#include "Actor.h"
#include "AIState.h"
#include <SDL3/SDL_log.h>
AIComponent::AIComponent(Actor *owner):Component(owner),
mCurrentState(nullptr)
{

}
void AIComponent::Update(float deltaTime)
{

}

void AIComponent::ChangeState(const std::string& name)
{
    if(mCurrentState) {
        mCurrentState->OnExit();
    }
    auto inter = mStateMap.find(name);
    if(inter != mStateMap.end()) {
        mCurrentState = inter->second;
        mCurrentState->OnEnter();
    }
    else 
    {
        SDL_Log("Could not find AIState %s in state map", name.c_str());
		mCurrentState = nullptr;
    }
}

void AIComponent::RegisterState(AIState *state)
{
    mStateMap.emplace(state->GetName(), state);
}
