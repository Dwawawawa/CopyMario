#include "00pch.h"
#include "SceneOutro.h"

SceneOutro::SceneOutro()
{

}

SceneOutro::~SceneOutro()
{

}

void SceneOutro::Initialize()
{
	m_tempObj = std::make_shared<GameObject>();
}

void SceneOutro::Update(float dTime)
{
	m_tempObj->patrol(300);
}


void SceneOutro::ShowDebug()
{

}

void SceneOutro::Release()
{

}

std::vector<std::shared_ptr<GameObject>> SceneOutro::GetRenderObjects() const
{

	std::vector<std::shared_ptr<GameObject>> objects = {};

	if (m_tempObj) {
		objects.push_back(m_tempObj);
	}

	return objects;
}
