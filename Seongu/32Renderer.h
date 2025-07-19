// 32Renderer.h
#pragma once
#include "31Component.h"

class Transform;
class SSEngine;

class Renderer : public Component
{
public:
    Renderer() : m_width(100), m_height(100), m_color(255, 255, 255) {}

    void Initialize() override;
    void Render(std::shared_ptr<SSEngine> renderer) override;

    void SetSize(float width, float height) { m_width = width; m_height = height; }
    void SetColor(int r, int g, int b) { m_color = { r, g, b }; }

    float GetWidth() const { return m_width; }
    float GetHeight() const { return m_height; }

private:
    Transform* m_transform = nullptr;
    float m_width, m_height;
 
    struct Color 
    { 
        int r, g, b; 
        Color(int r = 255, int g = 255, int b = 255) 
            : r(r), g(g), b(b) {} 
    
        // 이것도 필요없는거 아닌가? 뭐든 D2D1::ColorF로 사용하면 될 텐데
        // D2D1::ColorF로 변환하는 함수
        D2D1::ColorF ToD2DColorF(float a = 1.0f) const
        {
            return D2D1::ColorF(
                r / 255.0f,
                g / 255.0f,
                b / 255.0f,
                a
            );
        }
    } m_color;


};