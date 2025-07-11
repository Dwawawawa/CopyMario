#pragma once


//class Button;
//class ObjectManager;

//////////////////////////////////////////
//! 나는 여기다가 
//! 뭘 추가할려고 하나?
//! 씬은 3개
//! 1. 시작씬
//! 2. 검은 화면 + 캐릭터 버섯 어떻게구현해야 하나// 게
//! 3. 인게임씬 맵 1-1
//! 4. 아웃트로 씬

enum class eSceneState
{
	INTRO,
	INGAME1_1,
	BLACK, 
	OUTTRO,
	COUNT
};


/// 모든 씬에 공통되는 씬의 상태 (더 있으면 추가바람)
// 별로 안쓸만해보임 FSM이긴 한데;;
enum class eNowSceneState
{
	NONE,
	GO_TO_NEXT_SCENE,
	GO_TO_PREV_SCENE,
	POPUP_SHOW,
};


// 이것은 추상 클래스(Abstract Class)
class Scene
{
public:
	Scene();
	virtual ~Scene();

	// 초기화
	virtual void Initialize();
	virtual void Update(float dTime) abstract;
	virtual void Draw() abstract;
	virtual void ShowDebug();
	virtual void Release();

protected:
	//ObjectManager* m_pObjectManager;
	eNowSceneState m_NowSceneState;		// 현재 씬의 상태

	bool m_IsGameEnd;

public:
	void SetIsGameEnd(bool val) { m_IsGameEnd = val; }
	void SetNowSceneState(eNowSceneState val) { m_NowSceneState = val; }
};