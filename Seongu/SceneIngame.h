#pragma once
#include "Scene.h"

class GameObject;


class SceneIngame : public Scene
{
public:
	SceneIngame();
	virtual ~SceneIngame();

	// Scene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update(float dTime) override;
	virtual void ShowDebug() override;
	virtual void Release() override;

	std::vector<std::shared_ptr<GameObject>> GetRenderObjects() const;
private:

	std::shared_ptr<GameObject> m_tempObj;          

};