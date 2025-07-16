#include "00pch.h"
#include "SceneIngame.h"

SceneIngame::SceneIngame()
{

}

SceneIngame::~SceneIngame()
{

}


void SceneIngame::Initialize()
{
	m_tempObj = std::make_shared<GameObject>();
}

void SceneIngame::Update(float dTime)
{
	m_tempObj->patrol(200);
}


void SceneIngame::ShowDebug()
{

}

void SceneIngame::Release()
{

}

std::vector<std::shared_ptr<GameObject>> SceneIngame::GetRenderObjects() const
{

	std::vector<std::shared_ptr<GameObject>> objects = {};

	if (m_tempObj) {
		objects.push_back(m_tempObj);
	}

	return objects;
}
