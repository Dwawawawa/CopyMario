#pragma once
#include "Scene.h"

class GameObject;


class SceneIngame : public Scene
{
public:
	SceneIngame();
	virtual ~SceneIngame();

	// Scene��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update(float dTime) override;
	virtual void Release() override;


private:


};