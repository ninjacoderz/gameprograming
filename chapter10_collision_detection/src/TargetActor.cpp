#include "TargetActor.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"
#include "GameMath.h"

TargetActor::TargetActor(Game* game):Actor(game)
{
    SetRotation(Quaternion(Vector3::UnitZ, GameMath::Pi));
    MeshComponent* mc = new MeshComponent(this);
    Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Target.gpmesh");
    mc->SetMesh(mesh);

    BoxComponent* bc = new BoxComponent(this);
	bc->SetObjectBox(mesh->GetBox());
    
};