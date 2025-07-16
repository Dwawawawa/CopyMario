#include "00pch.h"
#include "SceneBlack.h"

SceneBlack::SceneBlack()
{
}

SceneBlack::~SceneBlack()
{
}

void SceneBlack::Initialize()
{
	m_tempObj = std::make_shared<GameObject>();
}

void SceneBlack::Update(float dTime)
{
	m_tempObj->patrol(400);
}


void SceneBlack::ShowDebug()
{

}

void SceneBlack::Release()
{

}

std::vector<std::shared_ptr<GameObject>> SceneBlack::GetRenderObjects() const
{

	std::vector<std::shared_ptr<GameObject>> objects = {};

	if (m_tempObj) {
		objects.push_back(m_tempObj);
	}

	return objects;
}
