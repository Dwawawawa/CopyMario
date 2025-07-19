#pragma once
#include "Scene.h"

class GameObject;
class SSEngine;
class ObjectManager;

class SceneIntro : public Scene
{
public:
	SceneIntro();
	virtual ~SceneIntro();

	// Scene��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update(float dTime) override;
	virtual void ShowDebug() override;
	virtual void Release() override;

	virtual void Render(std::shared_ptr<SSEngine> Renderer) override;

private:

	
};