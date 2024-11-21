#pragma once

#include "Client_Defines.h"
#include "Base.h"


BEGIN(Engine)
class CCollision;
class CRenderer;
class CGameInstance;
END

 
BEGIN(Client)

class CMainApp final : public CBase
{
public:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT NativeConstruct();
	void Tick(float fTimeDelta);
	void Late_Tick(); // CH 추가
	HRESULT Render();

private:
	CGameInstance*			m_pGameInstance = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	CRenderer*				m_pRenderer = nullptr;
	CCollision*				m_pCollision = nullptr; // CH 추가

#ifdef _DEBUG
private:
	_tchar			m_szFPS[MAX_PATH] = TEXT("");
	_uint			m_iNumDraw = 0;
	_float			m_fTimeAcc = 0.f;
#endif // _DEBUG

private:
	HRESULT SetUp_RenderState(); /* 렌더상태를 설정한다(필모드, 컬모드, 알파블렌드) */
	HRESULT SetUp_SamplerState(); /* 샘플러로부터 샘플링해오는 방식에 대한 설정. */

	HRESULT Start_Level(LEVEL eLevelID);
	HRESULT Ready_Prototype_Component();
	HRESULT Ready_Prototype_GameObject();
	HRESULT Ready_Layer_Logo();


#ifdef _DEBUG
private:
	HRESULT Compute_FPS();
#endif // _DEBUG

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END