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
// ���̾ ��� �ø� ���ΰ� ����ϱ� : 2d������ z ��ǥ
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