#pragma once
#include "Scene.h"

class GameObject;

class SceneOutro : public Scene
{
public:
	SceneOutro();
	virtual ~SceneOutro();

	// Scene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update(float dTime) override;
	
	virtual void Release() override;

	
private:



};