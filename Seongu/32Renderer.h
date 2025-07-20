// 32Renderer.h
#pragma once
#include "31Component.h"

class Transform;
class SSEngine;

class Renderer : public Component
{
public:
    Renderer() : m_width(100), m_height(100), m_color(255, 255, 255) , m_layer(0){}

    void Initialize() override;
    void Render(std::shared_ptr<SSEngine> renderer) override;
    void Release() override;

    void SetSize(float width, float height) { m_width = width; m_height = height; }
    void SetColor(int r, int g, int b) { m_color = { r, g, b }; }
    void SetColor(const D2D1::ColorF& d2dColor) {
        m_color = {
            static_cast<int>(d2dColor.r * 255.0f),
            static_cast<int>(d2dColor.g * 255.0f),
            static_cast<int>(d2dColor.b * 255.0f)
        };
    }

    float GetWidth() const { return m_width; }
    float GetHeight() const { return m_height; }

    void SetLayer(int layer) { m_layer = layer; }
    int GetLayer() const { return m_layer; }

private:
    Transform* m_transform = nullptr;
    float m_width, m_height;
 
    struct Color 
    { 
        int r, g, b; 
        Color(int r = 255, int g = 255, int b = 255) 
            : r(r), g(g), b(b) {} 
    
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

    // 렌더링 순서  (높을수록 앞에 그려짐)
    int m_layer;
};