#pragma once
class GameObject
{
public:
    GameObject();
    ~GameObject();

    void patrol(float y);

    // ��ġ ���� get �Լ�
    float GetX() const { return x; }
    float GetY() const { return y; }

    // ũ�� ���� get �Լ�
    float GetWidth() const { return width; }
    float GetHeight() const { return height; }

    // �̵� ���� get �Լ�
    float GetSpeed() const { return speed; }
    int GetDirection() const { return direction; }

private:
    float x, y;          // ���� ��ġ
    float width, height; // ũ��
    float speed;         // �̵� �ӵ�
    int direction;       // �̵� ���� (1: ������, -1: ����)

    // ��Ÿ �������� �ʿ��� ����
    //Color color;
    //Texture* texture;
};