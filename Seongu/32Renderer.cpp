// 32Renderer.cpp
#include "00pch.h"
#include "32Renderer.h"
#include "30GameObject.h"
#include "32Transform.h"

void Renderer::Initialize()
{
    m_transform = GetOwner()->GetComponent<Transform>();
}

void Renderer::Render(std::shared_ptr<SSEngine> renderer)
{
    if (!m_transform || !IsActive()) return;

    auto pos = m_transform->GetPosition();
    
    // SSEngine�� ����ؼ� ���� ������
    renderer->DrawRectangle(pos.x, pos.y, m_width, m_height, m_color.ToD2DColorF());
    // �Ǵ� �ؽ�ó�� �ִٸ�
    // renderer->DrawTexture(m_texture, pos.x, pos.y, m_width, m_height);

}