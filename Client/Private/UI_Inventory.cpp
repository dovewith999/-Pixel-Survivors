#include "stdafx.h"
#include "..\Public\UI_Inventory.h"
#include "GameInstance.h"
#include "Player.h"
#include "UI_WeaponIcon.h"

CUI_Inventory::CUI_Inventory(LPDIRECT3DDEVICE9 _GraphicDevice)
	: CUIObject(_GraphicDevice)
{
}

CUI_Inventory::CUI_Inventory(const CUI_Inventory & rhs)
	: CUIObject(rhs)
{
}

HRESULT CUI_Inventory::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	
	return S_OK;
}

HRESULT CUI_Inventory::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_vecWeaponIcon.reserve(6);

	m_fSizeX = 200.0f;
	m_fSizeY = 70.0f;

	if (nullptr != pArg)
	{
		m_vPos = *(_float2*)pArg;
	}

	m_iInventoryNum = 0;

	D3DXMatrixOrthoLH(&m_ProjMatrix, (float)g_iWinCX, (float)g_iWinCY, 0.f, 100.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	Safe_AddRef(pGameInstance);

	m_pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player"));

	Safe_Release(pGameInstance);

	PushBack_WeaponIcon(PLASMA);

	return S_OK;
}

void CUI_Inventory::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);
}

void CUI_Inventory::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	__super::Late_Tick(fTimeDelta);


	m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);

}

HRESULT CUI_Inventory::Render()
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

void CUI_Inventory::PushBack_WeaponIcon(WEAPONLIST _WeaponType)
{
	if (5 <= m_iInventoryNum)
	{
		return;
	}

	if (GARLIC == _WeaponType)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_WeaponIcon_Garlic", TEXT("Prototype_UIObject_WeaponIcon_Garlic"), &_float2(34.f * m_iInventoryNum + 17.f, 49.f))))
			return;
		m_vecWeaponIcon.push_back(_WeaponType);
		++m_iInventoryNum;
		Safe_Release(pGameInstance);
	}

	else if (PLASMA == _WeaponType)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_WeaponIcon_Plasma", TEXT("Prototype_UIObject_WeaponIcon_Plasma"), &_float2(34.f * m_iInventoryNum + 17.f, 49.f))))
			return;
		m_vecWeaponIcon.push_back(_WeaponType);
		++m_iInventoryNum;
		Safe_Release(pGameInstance);
	}

	else if (LIGHTNING == _WeaponType)
	{
		
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_WeaponIcon_LightningRing", TEXT("Prototype_UIObject_WeaponIcon_LightningRing"), &_float2(34.f * m_iInventoryNum + 17.f, 49.f))))
			return;
		m_vecWeaponIcon.push_back(_WeaponType);
		++m_iInventoryNum;
		Safe_Release(pGameInstance);
	}

	else if (BIBLE == _WeaponType)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_WeaponIcon_Bible", TEXT("Prototype_UIObject_WeaponIcon_Bible"), &_float2(34.f * m_iInventoryNum + 17.f, 49.f))))
			return;
		m_vecWeaponIcon.push_back(_WeaponType);
		++m_iInventoryNum;
		Safe_Release(pGameInstance);
	}
}

HRESULT CUI_Inventory::SetUp_Components()
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
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Inventory"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Inventory::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	return S_OK;
}

HRESULT CUI_Inventory::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return S_OK;
}

CUI_Inventory * CUI_Inventory::Create(LPDIRECT3DDEVICE9 _GraphicDevice)
{
	CUI_Inventory* pInstance = new CUI_Inventory(_GraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Error : Failed Created UI_Inventory Prototype"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Inventory::Clone(void * pArg)
{
	CUI_Inventory* pInstance = new CUI_Inventory(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Error : Failed Cloned UI_Inventory Copy"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Inventory::Free()
{
	__super::Free();

	m_vecWeaponIcon.clear();

	Safe_Release(m_pWeaponIcon);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
