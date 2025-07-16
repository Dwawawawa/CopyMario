#pragma once
//class Button;
//class ObjectManager;




// 이것은 추상 클래스(Abstract Class)
class Scene
{
public:
	Scene();
	virtual ~Scene();

	// 초기화
	virtual void Initialize();
	virtual void Update(float dTime) abstract;

	virtual void ShowDebug();
	virtual void Release();


	virtual std::vector<std::shared_ptr<GameObject>> GetRenderObjects() const abstract;
protected:
	//ObjectManager* m_pObjectManager;
	

	

	std::shared_ptr<GameObject> m_tempObj = nullptr;

public:
	
};