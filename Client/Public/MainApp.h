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
	void Late_Tick(); // CH �߰�
	HRESULT Render();

private:
	CGameInstance*			m_pGameInstance = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	CRenderer*				m_pRenderer = nullptr;
	CCollision*				m_pCollision = nullptr; // CH �߰�

#ifdef _DEBUG
private:
	_tchar			m_szFPS[MAX_PATH] = TEXT("");
	_uint			m_iNumDraw = 0;
	_float			m_fTimeAcc = 0.f;
#endif // _DEBUG

private:
	HRESULT SetUp_RenderState(); /* �������¸� �����Ѵ�(�ʸ��, �ø��, ���ĺ���) */
	HRESULT SetUp_SamplerState(); /* ���÷��κ��� ���ø��ؿ��� ��Ŀ� ���� ����. */

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