#pragma once


//class Button;
//class ObjectManager;

//////////////////////////////////////////
//! ���� ����ٰ� 
//! �� �߰��ҷ��� �ϳ�?
//! ���� 3��
//! 1. ���۾�
//! 2. ���� ȭ�� + ĳ���� ���� ��Ա����ؾ� �ϳ�// ��
//! 3. �ΰ��Ӿ� �� 1-1
//! 4. �ƿ�Ʈ�� ��

enum class eSceneState
{
	INTRO,
	INGAME1_1,
	BLACK, 
	OUTTRO,
	COUNT
};


/// ��� ���� ����Ǵ� ���� ���� (�� ������ �߰��ٶ�)
// ���� �Ⱦ����غ��� FSM�̱� �ѵ�;;
enum class eNowSceneState
{
	NONE,
	GO_TO_NEXT_SCENE,
	GO_TO_PREV_SCENE,
	POPUP_SHOW,
};


// �̰��� �߻� Ŭ����(Abstract Class)
class Scene
{
public:
	Scene();
	virtual ~Scene();

	// �ʱ�ȭ
	virtual void Initialize();
	virtual void Update(float dTime) abstract;
	virtual void Draw() abstract;
	virtual void ShowDebug();
	virtual void Release();

protected:
	//ObjectManager* m_pObjectManager;
	eNowSceneState m_NowSceneState;		// ���� ���� ����

	bool m_IsGameEnd;

public:
	void SetIsGameEnd(bool val) { m_IsGameEnd = val; }
	void SetNowSceneState(eNowSceneState val) { m_NowSceneState = val; }
};