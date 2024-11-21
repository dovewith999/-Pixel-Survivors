#include "stdafx.h"
#include "..\Public\UI_Start.h"
#include "GameInstance.h"

CUI_Start::CUI_Start(LPDIRECT3DDEVICE9 _GraphicDevice)
	: CUIObject(_GraphicDevice)
{
}

CUI_Start::CUI_Start(const CUI_Start & rhs)
	: CUIObject(rhs)
{
}

HRESULT CUI_Start::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Start::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fSizeX = 600.0f;
	m_fSizeY = 100.0f;

	m_vPos.x = 640.f;
	m_vPos.y = 600.f;

	D3DXMatrixOrthoLH(&m_ProjMatrix, (float)g_iWinCX, (float)g_iWinCY, 0.f, 100.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));

	return S_OK;
}

void CUI_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (600.f == m_vPos.y)
	{
		m_bDown = false;
		m_bUp = true;
	}

	if (570.f == m_vPos.y)
	{
		m_bUp = false;
		m_bDown = true;
	}

	if (m_bUp)
	{
		--m_vPos.y;
	}

	else if (m_bDown)
	{
		++m_vPos.y;
	}
}

void CUI_Start::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	__super::Late_Tick(fTimeDelta);


	m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);
}

HRESULT CUI_Start::Render()
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

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Start::SetUp_Components()
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
	if (FAILED(__super::Add_Components(LEVEL_LOGO, TEXT("Prototype_Component_Texture_UI_Start"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Start::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/*m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);*/

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CUI_Start::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CUI_Start * CUI_Start::Create(LPDIRECT3DDEVICE9 _GraphicDevice)
{
	CUI_Start* pInstance = new CUI_Start(_GraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Error : Failed Created UI_Logo Prototype"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Start::Clone(void * pArg)
{
	CUI_Start* pInstance = new CUI_Start(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Error : Failed Cloned UI_Logo Copy"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Start::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}