#pragma once

class GameObject;
class SSEngine;


class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	
	void Initialize();

	void Update(float dTime);
	void Render(std::shared_ptr<SSEngine> Renderer);

private:
	std::list<GameObject*> ObjectList;

};