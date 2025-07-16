#pragma once
class GameObject
{
public:
    GameObject();
    ~GameObject();

    void patrol(float y);

    // 위치 관련 get 함수
    float GetX() const { return x; }
    float GetY() const { return y; }

    // 크기 관련 get 함수
    float GetWidth() const { return width; }
    float GetHeight() const { return height; }

    // 이동 관련 get 함수
    float GetSpeed() const { return speed; }
    int GetDirection() const { return direction; }

private:
    float x, y;          // 현재 위치
    float width, height; // 크기
    float speed;         // 이동 속도
    int direction;       // 이동 방향 (1: 오른쪽, -1: 왼쪽)

    // 기타 렌더링에 필요한 정보
    //Color color;
    //Texture* texture;
};