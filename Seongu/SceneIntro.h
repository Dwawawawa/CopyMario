// SceneIntro.h
#pragma once
#include "Scene.h"

class GameObject;
class SSEngine;
class ObjectManager;

///////////// 
// ����� 
class PhysicsComponent;
class InputComponent;
class ColliderComponent;

class SceneIntro : public Scene
{
public:
    SceneIntro() = default;
    virtual ~SceneIntro() = default;

    void Initialize() override;
    void Update(float dTime) override;
private:
    PhysicsComponent*  debugPhysics   = nullptr ;
    InputComponent*    debugInput     = nullptr ;
    ColliderComponent* debugCollider  = nullptr ;
	
};