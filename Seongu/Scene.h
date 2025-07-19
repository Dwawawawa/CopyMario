#pragma once

class SSEngine;
class ObjectManager;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Initialize();
	virtual void Update(float dTime) abstract;

	virtual void ShowDebug();
	virtual void Release();

	virtual void Render(std::shared_ptr<SSEngine> Renderer);

protected:
	std::unique_ptr<ObjectManager> m_pObjectManager;
};