#include "00pch.h"
#include "SceneIntro.h"
#include "30GameObject.h"


SceneIntro::SceneIntro()
{

}

SceneIntro::~SceneIntro()
{

}

void SceneIntro::Initialize()
{
	m_tempObj = std::make_shared<GameObject>();
}

void SceneIntro::Update(float dTime)
{
	m_tempObj->patrol(100);
}


void SceneIntro::ShowDebug()
{

}

void SceneIntro::Release()
{

}

std::vector<std::shared_ptr<GameObject>> SceneIntro::GetRenderObjects() const
{

	std::vector<std::shared_ptr<GameObject>> objects = {};

	if (m_tempObj) {
		objects.push_back(m_tempObj);
	}

	return objects;
}
