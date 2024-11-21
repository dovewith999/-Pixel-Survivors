#include "..\Public\Graphic_Device.h"
#include "ZFrustum.h"

IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
	: m_p3D(nullptr) 
	, m_pDevice(nullptr)
	, m_pSprite(nullptr)
	, m_pFont(nullptr)
{

}

HRESULT CGraphic_Device::InitDevice(HWND hWnd, GRAPHIC_DEVICE_DESC::MODE eWinMode, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppOut)
{
	m_p3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9			DeviceCaps;
	// 3D렌더링에 필요한 기본적인 연산을 지원하는지 조사하여 각종 정보를 보관하기 위한 구조체
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// FAILED 매크로 :hresult 반환타입이 음수일 경우 true
	// GetDeviceCaps : 장치 성능을 조사
	// D3DADAPTER_DEFAULT :정보를 얻으려는 기본 그래픽 카드를 의미
	// D3DDEVTYPE_HAL : HAL(hardware abstraction layer)에 저장되어 있는 장치의 필수 정보를 얻어오기 위해 넣어주는 인자값
	if (FAILED(m_p3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		MSGBOX(L"GetDeviceCaps Failed"); // mfc에서만 사용가능한 메세지 박스 함수
		return E_FAIL;
	}

	DWORD	vp = 0;		// 버텍스 프로세싱 : 정점 연산 + 조명 처리

	//m_pDevice->SetTransform(D3DTS_WORLD, 행렬.);
	//m_pDevice->SetTransform(D3DTS_VIEW, 행렬.);
	//m_pDevice->SetTransform(D3DTS_PROJECTION, 행렬.);

	//D3DLIGHT9		LightDesc;
	//m_pDevice->SetLight(0, LightDesc);


	// HAL 을 통해 조사한 장치가 하드웨어 버텍스 프로세싱을 지원하는지 판단
	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;		// 그래픽 카드

	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;		// 직접 연산(CPU)

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp, hWnd, eWinMode, iWinCX, iWinCY);

	if (FAILED(m_p3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_pDevice)))
	{
		MSGBOX(L"CreateDevice Failed"); // mfc에서만 사용가능한 메세지 박스 함수
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		MSGBOX(L"m_pSprite Failed");
		return E_FAIL;
	}

	D3DXFONT_DESCW	tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	//lstrcpy(tFontInfo.FaceName, L"궁서");
	
	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		MSGBOX(L"m_pFont Failed");
		return E_FAIL;
	}

	*ppOut = m_pDevice;

	Safe_AddRef(m_pDevice);

	ZFrustum_Manager* pZFrustum_Manager = ZFrustum_Manager::Get_Instance();
	Safe_AddRef(pZFrustum_Manager);
	pZFrustum_Manager->Set_Graphic_Device(m_pDevice);
	Safe_Release(pZFrustum_Manager);
	return S_OK;
}

void CGraphic_Device::SetParameters(D3DPRESENT_PARAMETERS & d3dpp, HWND hWnd, GRAPHIC_DEVICE_DESC::MODE eWinMode, _uint iWinCX, _uint iWinCY)
{
	d3dpp.BackBufferWidth = iWinCX;
	d3dpp.BackBufferHeight = iWinCY;

	// 후면 버퍼의 픽셀 포맷 , 32비트 포맷 사용
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	
	// 후면 버퍼의 개수
	d3dpp.BackBufferCount = 1;

	// 픽셀이 모여 발생하는 계단현상을 뭉개서 부드럽게 연출하는 기법(안티 엘리어싱)을 지원하는지 여부를 묻는 인자값
	// 자연스럽게 연출은 되지만 성능저하가 발생하기 때문에 사용하지 않을 예정
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// 복사가 아닌 교체 방식을 통해 그리기와 지우기를 하게끔 만들어준다.
	
	// D3DSWAPEFFECT_DISCARD : 스왑 체인 방식
	// D3DSWAPEFFECT_FLIP : 버퍼 하나를 뒤집으면서 사용하는 방식
	// D3DSWAPEFFECT_COPY : 더블 버퍼링과 유사하게 복사하여 사용하는 방식

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;	// 장치를 사용할 윈도우 핸들 설정
	
	d3dpp.Windowed = eWinMode; // TRUE인 경우 창모드, FALSE 전체 화면 모드

	// 스텐실 버퍼 : 픽셀의 특정 부위를 잘라내기위해 비교대상군을 생성한 버퍼.
	// 깊이 버퍼(Z버퍼) : 물체의 깊이 값을 저장하여 원근감을 표현하기 위한 버퍼
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// 전체화면 시에 사용하는 인자값
	// 창 모드 일 경우 장치가 아닌 운영체제가 재생률을 관리함
	// 전체 화면으로 변경 시 모니터 재생율에 대한 설정
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	
	// 재생율과 시연의 간격 설정
	// D3DPRESENT_INTERVAL_IMMEDIATE : 즉시 시연
	// D3DPRESENT_INTERVAL_DEFAULT : 적절한 간격을 DIRECTX 알아서 결정, 보통 모니터 재생율을 따라감
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void CGraphic_Device::Render_Begin(void)
{
	// 렌더링 과정 : 후면 버퍼를 먼저 비운다(지운다)-> 후면 버퍼에 그린다-> 후면버퍼에 연결된 서피스를 전면버퍼에 연결된 서피스와 교체하여 출력한다.

	// 1인자 : 지우고자 하는 렉트의 개수
	// 2인자 : 지우고자 하는 렉트 배열의 이름(nullptr인 경우 전체 화면 영역에 해당하는 렉터라는 의미)

	m_pDevice->Clear(0,
		nullptr,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(255, 0, 0, 255),	// 백버퍼 색상
		1.f, // z버퍼의 초기화 값
		0);	 // 스텐실 버퍼의 초기화 값

	//여기서부터 후면버퍼에 그리기 시점을 알린다.
	m_pDevice->BeginScene();

	// 2D 이미지를 그릴 수 있도록 장치에게 알림, 현재 렌더링 옵션은 매개변수로 넣어줌
	// 알파테스트가 유효한 상태에서 알파블렌딩을 사용하겠다는 옵션
	

}

void CGraphic_Device::Render_End(HWND hWnd)
{
	
	m_pDevice->EndScene();

	// 후면버퍼에 연결되어 있는 서피스와 전면 버퍼에 연결된 서피스를 교환하는 과정
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
	// 1,2인자 : 스왑체인 기법이 D3DSWAPEFFECT_COPY가 아닌 이상 NULL
	// 3인자 : 출력 대상 윈도우 핸들, 기본 nullptr인 경우 d3dpp.hDeviceWindow을 참조
	// 4인자 :  스왑체인 기법이 D3DSWAPEFFECT_COPY가 아닌 이상 NULL

}

void CGraphic_Device::Free()
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pDevice);
	Safe_Release(m_p3D);
}
