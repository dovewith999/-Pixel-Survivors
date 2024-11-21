#include "stdafx.h"
#include "..\Public\UI_PlayerStats.h"
#include "GameInstance.h"
#include "Player.h"

CUI_PlayerStats::CUI_PlayerStats(LPDIRECT3DDEVICE9 _GraphicDevice)
	: CUIObject(_GraphicDevice)
{
}

CUI_PlayerStats::CUI_PlayerStats(const CUI_PlayerStats & rhs)
	: CUIObject(rhs)
{
}

HRESULT CUI_PlayerStats::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_PlayerStats::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = 300.0f;
	m_fSizeY = 400.0f;

	m_vPos.x = 150.f;
	m_vPos.y = 520.f;

	D3DXMatrixOrthoLH(&m_ProjMatrix, (float)g_iWinCX, (float)g_iWinCY, 0.f, 100.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	Safe_AddRef(pGameInstance);

	m_pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player"));

	Safe_Release(pGameInstance);

	if (FAILED(D3DXCreateSprite(m_pGraphic_Device, &m_pSprite)))
	{
		MSGBOX(L"m_pSprite Failed");
		return E_FAIL;
	}

	D3DXFONT_DESCW	tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 30;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"ANY");

	if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontInfo, &m_pFont)))
	{
		MSGBOX(L"m_pFont Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CUI_PlayerStats::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CUI_PlayerStats::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


	if (m_pPlayer->Get_LevelUp())
	{
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_PlayerStats::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_vPos.x - g_iWinCX * 0.5f, -m_vPos.y + g_iWinCY * 0.5f, 1.f));

	_float4x4		ViewMatrix;
	D3DXMatrixIdentity(&ViewMatrix);

	m_pTransformCom->Bind_OnGraphicDevice();
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_iTextureNum)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	TCHAR	szBuf[256]{};
	swprintf_s(szBuf, L"공격력 : %.1f\n데미지 감소 : %d\n이동속도 : %.1f\n체력 회복력 : %.1f\n쿨타임 : %.1f\n공격범위 : %.1f\n투사체 속도 : %.1f\n지속시간 : %.1f\n투사체 수 : %d\n아이템 획득 범위 : %.1f\n행운 : %.1f\n경험치 획득량 : %.1f"
	, m_fPower, m_iArmor, m_fMoveSpeed, m_fRecovery, m_fCoolTime, m_fArea, m_fSpeed, m_fDuration, m_iAmount, m_fMagnet, m_fLuck, m_fGrowth);

//private:
//	float	m_fPower = 1.f;			//공격력증가
//	int		m_iArmor = 0;			//대미지감소
//	float	m_fRecovery = 0.1f;		//체력회복력
//	float	m_fCoolTime = 1.f;		//쿨타임 감소
//	float	m_fArea = 1.f;			//공격범위 증가
//	float	m_fSpeed = 1.f;			//투사체 속도 증가
//	float	m_fDuration = 1.f;		//지속시간 증가
//	int		m_iAmount = 0;			//투사체 수 증가
//	float	m_fMoveSpeed = 0.f;		//이동속도증가
//	float	m_fMagnet = 1.f;		//아이템흭득범위
//	float	m_fLuck = 1.f;			//행운
//	float	m_fGrowth = 1.f;		//받는 경험치 증가

	D3DXMATRIX world;
	D3DXMatrixTranslation(&world, 10, 325, 0);

	m_pSprite->SetTransform(&world);
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);//2D

	m_pFont->DrawTextW(m_pSprite,
		szBuf,
		lstrlen(szBuf),
		NULL,
		0,
		D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pSprite->End();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_PlayerStats::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Renderer `*/
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SelectItem"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;;
}

HRESULT CUI_PlayerStats::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CUI_PlayerStats::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return S_OK;
}

CUI_PlayerStats * CUI_PlayerStats::Create(LPDIRECT3DDEVICE9 _GraphicDevice)
{
	CUI_PlayerStats* pInstance = new CUI_PlayerStats(_GraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Error : Failed Created UI_LevelUp"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_PlayerStats::Clone(void * pArg)
{
	CUI_PlayerStats* pInstance = new CUI_PlayerStats(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Error : Failed Cloned UI_LevelUp"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_PlayerStats::Free()
{
	__super::Free();
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}