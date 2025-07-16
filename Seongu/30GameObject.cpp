#include "00pch.h"
#include "30GameObject.h"


GameObject::GameObject()
{
    x = 300, y = 300;          // ���� ��ġ
    width = 100, height = 100; // ũ��
    speed = 0.5f;         // �̵� �ӵ�
    direction = 1;       // �̵� ���� (1: ������, -1: ����)
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
