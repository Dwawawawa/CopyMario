#pragma once
#include "Scene.h"

class GameObject;


class SceneIntro : public Scene
{
public:
	SceneIntro();
	virtual ~SceneIntro();

	// Scene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update(float dTime) override;
	virtual void ShowDebug() override;
	virtual void Release() override;

	std::vector<std::shared_ptr<GameObject>> GetRenderObjects() const;
private:

	
};