#include "00pch.h"
#include "30ObjectManager.h"
#include "30GameObject.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::Initialize()
{

}

void ObjectManager::Update(float dTime)
{
	for (auto obj : ObjectList)
	{
		obj->patrol(300);
	}
}

/////
// 레이어를 어떻게 올릴 것인가 고민하기 : 2d에서의 z 좌표
//bool IsLessThan(const GameObject* a, const Object* b)
//{
//	return a->GetLayer() < b->GetLayer();
//}

void ObjectManager::Render(std::shared_ptr<SSEngine> Renderer)
{
	
	
	//ObjectList.sort();
	for (auto obj : ObjectList)
	{
		
	}

}