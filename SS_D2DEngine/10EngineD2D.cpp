#include "00pch.h"
#include "10EngineD2D.h"




void SSEngine::Initialize(HWND hwnd)
{
	m_hwnd = hwnd;

	CreateDeviceAndSwapChain(hwnd);
	CreateRenderTargets();
	CreateWriteResource();

	ComPtr<IWICImagingFactory> wicFactory;

	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&wicFactory));

	DX::ThrowIfFailed(hr);

	m_wicFactory = wicFactory;

}


void SSEngine::Uninitialize()
{
	ReleaseRenderTargets();

	m_wicFactory = nullptr;

	m_targetBitmap = nullptr;
	m_brush = nullptr;

	m_d2dContext = nullptr;
	m_d2dDevice = nullptr;

	m_swapChain = nullptr;
	m_d3dDevice = nullptr;
}

void SSEngine::Resize(UINT width, UINT height)
{
	if (nullptr == m_swapChain) return; // 초기화 전에 호출이 될 수 있음.
	ReleaseRenderTargets();

	// 스왑체인 크기 조정 후 렌더 타겟 재생성
	DX::ThrowIfFailed(m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0));

	CreateRenderTargets();
}

void SSEngine::DrawLine(float x1, float y1, float x2, float y2, const D2D1::ColorF& color)
{
	m_brush->SetColor(color);
	m_d2dContext->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), m_brush.Get());
}

void SSEngine::DrawCircle(float x, float y, float radius, const D2D1::ColorF& color)
{
	m_brush->SetColor(color);
	m_d2dContext->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), m_brush.Get());

}

void SSEngine::DrawRectangle(float left, float top, float right, float bottom, const D2D1::ColorF& color)
{
	m_brush->SetColor(color);
	m_d2dContext->DrawRectangle(D2D1::Rect(left, top, right, bottom), m_brush.Get());
}

void SSEngine::DrawBitmap(ID2D1Bitmap1* bitmap, D2D1_RECT_F dest)
{
	m_d2dContext->DrawBitmap(bitmap, dest);
}

void SSEngine::DrawBitmap(ID2D1Bitmap1* bitmap, D2D1_RECT_F destRect, D2D1_RECT_F srcRect, float opacity)
{
	m_d2dContext->DrawBitmap(
		bitmap,
		destRect,
		opacity,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		srcRect
	);
}

void SSEngine::DrawMessage(const wchar_t* text, float left, float top, float width, float height, const D2D1::ColorF& color)
{
	if (nullptr == m_textBrush)
	{
		m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(color), &m_textBrush);
	}

	m_textBrush->SetColor(color);
	D2D1_RECT_F layoutRect = D2D1::RectF(left, top, left + width, top + height);

	m_d2dContext->DrawTextW(
		text,
		static_cast<UINT32>(wcslen(text)),
		m_textFormat.Get(),
		layoutRect,
		m_textBrush.Get(),
		D2D1_DRAW_TEXT_OPTIONS_NONE,
		DWRITE_MEASURING_MODE_NATURAL);
}

void SSEngine::SetTransform(const D2D1_MATRIX_3X2_F tm)
{
	if (m_d2dContext) m_d2dContext->SetTransform(tm);
}


void SSEngine::RenderBegin()
{
	m_d2dContext->BeginDraw();
	m_d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::White)); // 배경을 흰색으로 초기화
}


void SSEngine::RenderEnd(bool bPresent)
{
	m_d2dContext->EndDraw();

	if (bPresent)
	{
		Present();
	}
}

////////////////////////////////////////
//! 결국 교수님의 형태가 좋았던 거임 ㅋㅋ
void SSEngine::Present()
{
	// 렌더링 작업이 끝나면 스왑체인에 프레임을 표시
	HRESULT hr = m_swapChain->Present(1, 0);

	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		Uninitialize();     // 디바이스가 제거되거나 리셋된 경우, 재초기화 필요
		Initialize(m_hwnd);
	}
	else
	{
		DX::ThrowIfFailed(hr);
	}
}

////////////////////////////////////////
//! 🎧 굴팝, 테켄 뮤직 그리고 염따의 살숨4 레츠고~!
//! 크레이트 디바이스 앤드 스왑체인
//! 사실 제대로 보진 않았는데, 그냥 그거라고 생각되긴 해. 
//! 
//! -> 그렇기 때문에 지피티에게 배워야겠다.
/*
왜 이렇게 복잡한가?
전통적인 방법 (기존 SSEngine):
Direct2D → HWND 직접 렌더링
현대적인 방법 (이 함수):
D3D11 Device → DXGI SwapChain → D2D Device → D2D Context
장점

성능 향상: 하드웨어 가속 최적화
유연성: 3D와 2D 그래픽을 자유롭게 조합
호환성: 최신 Windows 그래픽 스택 활용
확장성: 나중에 3D 렌더링 추가 용이

실제 사용 시나리오
cpp// 3D 배경 렌더링 (D3D11)
m_d3dContext->Draw(...);

// 2D UI 오버레이 (D2D)
m_d2dContext->DrawText(...);
m_d2dContext->DrawRectangle(...);
*/
void SSEngine::CreateDeviceAndSwapChain(HWND hwnd)
{
	////////////////////////////////////////
	//1. D3D11 디바이스 생성
	/*
		D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
		D3D11CreateDevice(
			nullptr,                            // 기본 그래픽 어댑터 사용
			D3D_DRIVER_TYPE_HARDWARE,           // 하드웨어 가속 사용
			nullptr,                            
			D3D11_CREATE_DEVICE_BGRA_SUPPORT,   // ⭐ BGRA 포맷 지원 (D2D 호환성)
			featureLevels,                      // DirectX 11.0 기능 레벨
			// ...
		);

		핵심: D3D11_CREATE_DEVICE_BGRA_SUPPORT 플래그로 Direct2D와의 호환성 확보
		BGRA 포맷은 Direct2D가 선호하는 픽셀 포맷
	*/
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	ComPtr<ID3D11Device> d3dDevice;
	ComPtr<ID3D11DeviceContext> d3dContext;

	HRESULT hr = D3D11CreateDevice(
		nullptr,                            //[in, optional]  IDXGIAdapter* pAdapter
		D3D_DRIVER_TYPE_HARDWARE,           //D3D_DRIVER_TYPE DriverType
		nullptr,                            //HMODULE         Software
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,   //UINT            Flags
		featureLevels,                      //[in, optional]  const D3D_FEATURE_LEVEL* pFeatureLevels
		ARRAYSIZE(featureLevels),           //UINT            FeatureLevels
		D3D11_SDK_VERSION,                  //UINT            SDKVersion
		&d3dDevice,                         //[out, optional] ID3D11Device** ppDevice
		nullptr,                            //[out, optional] D3D_FEATURE_LEVEL* pFeatureLevel
		&d3dContext);                       //[out, optional] ID3D11DeviceContext** ppImmediateContext

	DX::ThrowIfFailed(hr);

	////////////////////////////////////////
	// 2. DXGI 스왑체인 생성
	/*
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;  // BGRA 포맷
	scDesc.BufferCount = 2;                       // 더블 버퍼링
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 현대적 스왑 모드

	DXGI: DirectX Graphics Infrastructure - 그래픽 드라이버와의 저수준 인터페이스
	스왑체인: 화면에 그려질 버퍼들을 관리하는 객체
	FLIP_DISCARD: 성능 최적화된 현대적 스왑 모드
	*/
	ComPtr<IDXGIDevice> dxgiDevice;
	hr = d3dDevice.As(&dxgiDevice);

	DX::ThrowIfFailed(hr);

	ComPtr<IDXGIAdapter> dxgiAdapter;
	hr = dxgiDevice->GetAdapter(&dxgiAdapter);

	DX::ThrowIfFailed(hr);

	ComPtr<IDXGIFactory2> dxgiFactory;
	hr = dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

	DX::ThrowIfFailed(hr);

	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scDesc.SampleDesc.Count = 1;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = 2;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scDesc.Scaling = DXGI_SCALING_STRETCH;

	ComPtr<IDXGISwapChain1> swapChain;
	hr = dxgiFactory->CreateSwapChainForHwnd(
		d3dDevice.Get(), hwnd, &scDesc, nullptr, nullptr, &swapChain);

	DX::ThrowIfFailed(hr);

	////////////////////////////////////////
	// 3. ID2D1Factory4 생성
	/*
	D2D1_FACTORY_OPTIONS opts = {};
	#if defined(_DEBUG)
		opts.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION; // 디버그 정보 활성화
	#endif
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, ...);

	Factory: Direct2D 객체들을 생성하는 팩토리
	Single-threaded: 단일 스레드에서만 사용 (성능 최적화)
	*/
	D2D1_FACTORY_OPTIONS opts = {};
	ComPtr<ID2D1Factory8> d2dFactory;

#if defined(_DEBUG)
	opts.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory8),
		&opts,
		reinterpret_cast<void**>(d2dFactory.GetAddressOf()));

	DX::ThrowIfFailed(hr);

	////////////////////////////////////////
	// 4. ID2D1Device4 생성
	/*
	d2dFactory->CreateDevice(dxgiDevice.Get(), &baseDevice);
	baseDevice.As(&d2dDevice); // 최신 버전으로 캐스팅

	DXGI Device 연결: D3D11 디바이스와 D2D 디바이스를 연결
	상호 운용성: D3D11 텍스처를 D2D에서 직접 그릴 수 있게 됨
	*/
	ComPtr<ID2D1Device> baseDevice;
	hr = d2dFactory->CreateDevice(dxgiDevice.Get(), &baseDevice);

	DX::ThrowIfFailed(hr);

	ComPtr<ID2D1Device7> d2dDevice;
	hr = baseDevice.As(&d2dDevice);

	DX::ThrowIfFailed(hr);

	////////////////////////////////////////
	// 5. ID2D1DeviceContext7 생성
	/*
	d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dContext);

	Device Context: 실제 그리기 명령을 실행하는 객체
	Direct2D의 모든 그리기 작업은 이 컨텍스트를 통해 수행
	*/
	ComPtr<ID2D1DeviceContext7> d2dContext;//
	hr = d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dContext);

	DX::ThrowIfFailed(hr);

	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_swapChain = swapChain;

	m_d2dDevice = d2dDevice;
	m_d2dContext = d2dContext;
}

/*
# 왜 필요한가?
- ImGUI: Direct3D 11 렌더 타겟 뷰가 필요
- 3D 렌더링: 나중에 3D 요소 추가 시 필요
- 통합 렌더링: 같은 백버퍼에 D2D와 D3D11 동시 그리기

# 전체 구조도
스왑체인 백버퍼 (실제 메모리)
		 ↙              ↘
DXGI Surface    →    ID3D11Texture2D
		 ↓                    ↓
ID2D1Bitmap1          ID3D11RenderTargetView
		 ↓                    ↓
Direct2D 그리기        Direct3D 11 그리기
		 ↓                    ↓
	같은 화면에 최종 출력
# 실제 사용 시나리오
```cpp
// 렌더링 시작
m_d2dContext->BeginDraw();

// 3D 배경 렌더링 (D3D11)
m_d3dContext->OMSetRenderTargets(1, m_d3dRenderTV.GetAddressOf(), nullptr);
// ... 3D 그리기 ...

// 2D UI 오버레이 (D2D)
m_d2dContext->DrawText(...);
m_d2dContext->DrawRectangle(...);

// 렌더링 종료
m_d2dContext->EndDraw();
m_swapChain->Present(1, 0); // 화면에 표시
```
#핵심 포인트
- 메모리 공유: 같은 백버퍼를 D2D와 D3D11이 공유
- 성능 최적화: 복사 없이 직접 렌더링
- 확장성: ImGUI, 3D 렌더링 등 추가 기능 지원
- 통합 관리: 하나의 Present() 호출로 모든 그래픽 출력
이 함수를 통해 현대적인 멀티 레이어 그래픽 시스템의 기반이 완성됩니다.
*/
void SSEngine::CreateRenderTargets()
{
	// ㅁ1. 스왑체인 백버퍼 획득
	// 6. SwapChain 백버퍼 -> D2D Bitmap1 을 생성하여 렌더 타겟으로 설정
	/*
	백버퍼: 실제 화면에 표시될 이미지가 그려지는 메모리 영역
	인덱스 0: 첫 번째 백버퍼 (더블 버퍼링에서 현재 그리기 대상)
	DXGI Surface: D3D와 D2D가 공통으로 사용할 수 있는 표면
	*/
	ComPtr<IDXGISurface> dxgiSurface;
	HRESULT hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiSurface));

	DX::ThrowIfFailed(hr);

	// ㅁ2. Direct2D 비트맵 속성 설정
	/*
	각 옵션의 의미:
	D2D1_BITMAP_OPTIONS_TARGET: 렌더 타겟으로 사용 가능
	D2D1_BITMAP_OPTIONS_CANNOT_DRAW: 소스 이미지로는 사용 불가 (성능 최적화)
	DXGI_FORMAT_B8G8R8A8_UNORM: BGRA 32비트 포맷
	D2D1_ALPHA_MODE_PREMULTIPLIED: 알파 채널이 미리 곱해진 상태
	96.0f DPI: 표준 화면 DPI
	*/
	D2D1_BITMAP_PROPERTIES1 bitmapProps = {};
	bitmapProps.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bitmapProps.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bitmapProps.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	bitmapProps.dpiX = bitmapProps.dpiY = 96.0f;

	// ㅁ3. DXGI Surface → D2D Bitmap 변환
	/*
	핵심: DXGI Surface를 Direct2D가 이해할 수 있는 비트맵으로 래핑
	실제 메모리는 공유하되, Direct2D 인터페이스 제공
	*/
	ComPtr<ID2D1Bitmap1> targetBitmap;
	hr = m_d2dContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &bitmapProps, targetBitmap.GetAddressOf());

	DX::ThrowIfFailed(hr);

	// ㅁ4. Direct2D 렌더 타겟 설정
	/*
	Direct2D의 모든 그리기 명령이 이 비트맵(=백버퍼)에 그려짐
	이제 DrawLine(), DrawText() 등이 화면에 표시됨
	*/
	m_d2dContext->SetTarget(targetBitmap.Get());

	m_targetBitmap = targetBitmap;

	// ㅁ5. 기본 브러시 생성
	/*
	단색 브러시 생성 (재사용 가능)
	매번 브러시를 생성하지 않고 색상만 변경해서 사용
	*/
	hr = m_d2dContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Blue),
		&m_brush);

	DX::ThrowIfFailed(hr);
	
	// ㅁ6. Direct3D 11 렌더 타겟 뷰 생성
	// ImGUI 은 D3D11 렌더 타겟 뷰가 필요!!! 
	ComPtr<ID3D11Texture2D> backBuffer;
	hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

	// RenderTargetView 생성
	ComPtr<ID3D11RenderTargetView> mainRTV;
	hr = m_d3dDevice->CreateRenderTargetView(
		backBuffer.Get(),      // 텍스처
		nullptr,               // 기본 뷰 설명
		&mainRTV               // 출력 RTV 포인터
	);

	DX::ThrowIfFailed(hr);

	m_d3dRenderTV = mainRTV;
}

void SSEngine::CreateWriteResource()
{
	ComPtr<IDWriteFactory> writeFactory = nullptr;
	HRESULT hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(writeFactory.GetAddressOf()));

	DX::ThrowIfFailed(hr);

	writeFactory->CreateTextFormat(
		L"", // FontName    제어판-모든제어판-항목-글꼴-클릭 으로 글꼴이름 확인가능
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		15.0f,   // Font Size
		L"", //locale
		&m_textFormat);

	DX::ThrowIfFailed(hr);

	m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING); // 왼쪽 정렬
	m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR); // 위쪽 정렬
	m_textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP); // 줄바꿈 

}

void SSEngine::ReleaseRenderTargets()
{
	if (m_d2dContext)
	{
		m_d2dContext->SetTarget(nullptr);
	}

	m_d3dRenderTV.Reset();

	m_targetBitmap.Reset();
	m_brush.Reset();
	m_textBrush.Reset();
}


void SSEngine::CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1*& outBitmap)
{
	ComPtr<IWICBitmapDecoder>     decoder;
	ComPtr<IWICBitmapFrameDecode> frame;
	ComPtr<IWICFormatConverter>   converter;


	HRESULT hr = m_wicFactory->CreateDecoderFromFilename(
		path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);

	DX::ThrowIfFailed(hr);


	hr = decoder->GetFrame(0, &frame);

	DX::ThrowIfFailed(hr);

	hr = m_wicFactory->CreateFormatConverter(&converter);

	DX::ThrowIfFailed(hr);


	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
	);

	DX::ThrowIfFailed(hr);

	// Direct2D 비트맵 속성 (premultiplied alpha, B8G8R8A8_UNORM)
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	// ⑥ DeviceContext에서 WIC 비트맵으로부터 D2D1Bitmap1 생성
	hr = m_d2dContext->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, &outBitmap);
}