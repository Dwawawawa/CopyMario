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
    auto scl = m_transform->GetScale();


    // SSEngine을 사용해서 실제 렌더링
    renderer->DrawRectangle(pos.x, pos.y, pos.x + scl.x, pos.y + scl.y, m_color.ToD2DColorF());
    // 또는 텍스처가 있다면
    // renderer->DrawTexture(m_texture, pos.x, pos.y, m_width, m_height);

}

void Renderer::Release()
{
    m_transform = nullptr;
    Component::Release();
}
