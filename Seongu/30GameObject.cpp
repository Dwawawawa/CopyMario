#include "00pch.h"
#include "30GameObject.h"


GameObject::GameObject()
{
    x = 300, y = 300;          // 현재 위치
    width = 100, height = 100; // 크기
    speed = 0.5f;         // 이동 속도
    direction = 1;       // 이동 방향 (1: 오른쪽, -1: 왼쪽)
}

GameObject::~GameObject()
{
}

void GameObject::patrol(float y)
{
    this->y = y;

    if (x > 800) 
        direction = -1;
    else if(x < 0)
        direction = 1;

    x = x + speed * direction;
}
