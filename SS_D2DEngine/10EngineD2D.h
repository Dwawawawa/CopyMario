#pragma once
#include <wrl/client.h>
#include <d2d1_3.h>
#include <d2d1helper.h>
#include <d3d11_1.h>
#include <dxgi1_2.h>
#include <dwrite.h>
#include <wincodec.h>
#include <string>

#pragma comment(lib, "D2D1.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")

using namespace Microsoft::WRL;

class SSEngine
{
public:
	SSEngine() = default;
	~SSEngine() { Uninitialize(); }

    // 기존 SSEngine 메서드를 새로운 이름으로 매핑
    void Initialize(HWND hwnd);
    void Uninitialize();
    void Resize(UINT width, UINT height);

    // 기존 그리기 메서드들
    void DrawLine(float x1, float y1, float x2, float y2, const D2D1::ColorF& color);
    void DrawCircle(float x, float y, float radius, const D2D1::ColorF& color);
    void DrawRectangle(float left, float top, float right, float bottom, const D2D1::ColorF& color);

    // 새로운 기능들
    void DrawBitmap(ID2D1Bitmap1* bitmap, D2D1_RECT_F dest);
    void DrawBitmap(ID2D1Bitmap1* bitmap, D2D1_RECT_F destRect, D2D1_RECT_F srcRect, float opacity = 1.0f);
    void DrawMessage(const wchar_t* text, float left, float top, float width, float height, const D2D1::ColorF& color);

    void SetTransform(const D2D1_MATRIX_3X2_F tm);

    // 기존 BeginRender/EndRender를 새로운 이름으로
    void RenderBegin();
    void RenderEnd(bool bPresent = true);
    void Present();

    void CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1*& outBitmap);

    // D3D 리소스 접근자
    ID3D11Device* GetD3DDevice() const { return m_d3dDevice.Get(); }
    ID3D11DeviceContext* GetD3DContext() const { return m_d3dContext.Get(); }
    ID3D11RenderTargetView* GetD3DRenderTargetView() const { return m_d3dRenderTV.Get(); }

private:
    // 초기화 도우미 메서드들
    void CreateDeviceAndSwapChain(HWND hwnd);
    void CreateRenderTargets();
    void CreateWriteResource();
    void ReleaseRenderTargets();


private:
	HWND m_hwnd = nullptr;

    // D3D11 리소스들
    ComPtr<ID3D11Device>           m_d3dDevice;
    ComPtr<IDXGISwapChain1>        m_swapChain;
    ComPtr<ID3D11DeviceContext>    m_d3dContext;
    ComPtr<ID3D11RenderTargetView> m_d3dRenderTV; // D3D render target view

    // D2D 리소스들
    ComPtr<ID2D1Device7>           m_d2dDevice;
    ComPtr<ID2D1DeviceContext7>    m_d2dContext;
    ComPtr<ID2D1Bitmap1>           m_targetBitmap; // D2D render target bitmap

    // 브러쉬들
    ComPtr<ID2D1SolidColorBrush>   m_brush;        // 기존 m_pNowBrush
    ComPtr<ID2D1SolidColorBrush>   m_textBrush;

    // 텍스트 관련 (기존 DWrite 관련)
    ComPtr<IDWriteFactory>         m_dwriteFactory; // 기존 m_pDWriteFactory
    ComPtr<IDWriteTextFormat>      m_textFormat;    // 기존 m_pTextFormat

    // 이미지 관련
    ComPtr<IWICImagingFactory>     m_wicFactory;    // 기존 주석처리된 m_pImageFactory




};

