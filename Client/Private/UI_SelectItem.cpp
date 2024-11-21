#include "stdafx.h"
#include "..\Public\UI_SelectItem.h"
#include "GameInstance.h"
#include "Player.h"

CUI_SelectItem::CUI_SelectItem(LPDIRECT3DDEVICE9 _GraphicDevice)
	: CUIObject(_GraphicDevice)
{
}

CUI_SelectItem::CUI_SelectItem(const CUI_SelectItem & rhs)
	: CUIObject(rhs)
{
}

HRESULT CUI_SelectItem::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_SelectItem::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = 400.0f;
	m_fSizeY = 130.0f;

	if (nullptr != pArg)
	{
		m_vPos = *(_float2*)pArg;
	}

	D3DXMatrixOrthoLH(&m_ProjMatrix, (float)g_iWinCX, (float)g_iWinCY, 0.f, 100.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	Safe_AddRef(pGameInstance);

	m_pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player"));

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_IconBox", TEXT("Prototype_UIObject_IconBox"), _float2(m_vPos.x - (m_fSizeX / 2), m_vPos.y - (m_fSizeY / 2)))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

void CUI_SelectItem::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CUI_SelectItem::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


	if (m_pPlayer->Get_LevelUp())
	{
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_SelectItem::Render()
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

HRESULT CUI_SelectItem::SetUp_Components()
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

HRESULT CUI_SelectItem::SetUp_RenderState()
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

HRESULT CUI_SelectItem::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CUI_SelectItem * CUI_SelectItem::Create(LPDIRECT3DDEVICE9 _GraphicDevice)
{
	CUI_SelectItem* pInstance = new CUI_SelectItem(_GraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Error : Failed Created UI_SelectItem"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_SelectItem::Clone(void * pArg)
{
	CUI_SelectItem* pInstance = new CUI_SelectItem(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Error : Failed Cloned UI_SelectItem"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_SelectItem::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}