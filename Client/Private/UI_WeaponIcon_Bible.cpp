#include "stdafx.h"
#include "..\Public\UI_WeaponIcon_Bible.h"
#include "GameInstance.h"
#include "Player.h"

CUI_WeaponIcon_Bible::CUI_WeaponIcon_Bible(LPDIRECT3DDEVICE9 _GraphicDevice)
	: CUIObject(_GraphicDevice)
{
}

CUI_WeaponIcon_Bible::CUI_WeaponIcon_Bible(const CUI_WeaponIcon_Bible & rhs)
	: CUIObject(rhs)
{
}

HRESULT CUI_WeaponIcon_Bible::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_WeaponIcon_Bible::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = 30.0f;
	m_fSizeY = 30.0f;

	if (nullptr != pArg)
	{
		m_vPos = *(_float2*)pArg;
	}

	D3DXMatrixOrthoLH(&m_ProjMatrix, (float)g_iWinCX, (float)g_iWinCY, 0.f, 100.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	Safe_AddRef(pGameInstance);

	m_pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player"));

	Safe_Release(pGameInstance);

	return S_OK;
}

void CUI_WeaponIcon_Bible::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CUI_WeaponIcon_Bible::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


	m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);
}

HRESULT CUI_WeaponIcon_Bible::Render()
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

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_WeaponIcon_Bible::SetUp_Components()
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
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_WeaponIcon_Bible"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;;
}

HRESULT CUI_WeaponIcon_Bible::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/*m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);*/

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CUI_WeaponIcon_Bible::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CUI_WeaponIcon_Bible * CUI_WeaponIcon_Bible::Create(LPDIRECT3DDEVICE9 _GraphicDevice)
{
	CUI_WeaponIcon_Bible* pInstance = new CUI_WeaponIcon_Bible(_GraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Error : Failed Created UI_SelectItem"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_WeaponIcon_Bible::Clone(void * pArg)
{
	CUI_WeaponIcon_Bible* pInstance = new CUI_WeaponIcon_Bible(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Error : Failed Cloned UI_SelectItem"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_WeaponIcon_Bible::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}