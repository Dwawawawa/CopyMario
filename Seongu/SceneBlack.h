#pragma once
#include "Scene.h"

class GameObject;

class SceneBlack : public Scene
{
public:
	SceneBlack();
	virtual ~SceneBlack();

	// Scene��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update(float dTime) override;
	virtual void Release() override;

	
private:

	

};