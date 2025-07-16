#pragma once
#include "Scene.h"

class GameObject;

class SceneOutro : public Scene
{
public:
	SceneOutro();
	virtual ~SceneOutro();

	// Scene��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update(float dTime) override;
	virtual void ShowDebug() override;
	virtual void Release() override;

	std::vector<std::shared_ptr<GameObject>> GetRenderObjects() const;
private:

	std::shared_ptr<GameObject> m_tempObj;

};