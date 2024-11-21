#include "stdafx.h"
#include "..\Public\UI_Enemy_HPElement.h"
#include "GameInstance.h"

CUI_Enemy_HPElement::CUI_Enemy_HPElement(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject(pGraphic_Device)
{

}

CUI_Enemy_HPElement::CUI_Enemy_HPElement(const CUI_Enemy_HPElement & rhs)
	: CUIObject(rhs)
{

}

HRESULT CUI_Enemy_HPElement::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;



	return S_OK;
}


HRESULT CUI_Enemy_HPElement::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		//m_vPos = *(_float2*)pArg;
	}


	m_pTransformCom->Set_Scale(D3DXVECTOR3(1.f, 0.09f, 0.09f));

	return S_OK;
}

void CUI_Enemy_HPElement::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	

}

void CUI_Enemy_HPElement::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	__super::Late_Tick(fTimeDelta);

	if(m_bShow)
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT CUI_Enemy_HPElement::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (!m_bShow)
		return S_OK;

	if (nullptr == m_pHPBarTransform)
		return E_FAIL;

	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

	_float3 vEnemyPos = m_pHPBarTransform->Get_State(CTransform::STATE_POSITION);
	vEnemyPos -= m_pTransformCom->Get_State(CTransform::STATE_RIGHT) * (1 - m_iHP) * 0.5f;
	vEnemyPos -= m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 0.001f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vEnemyPos);

	m_pTransformCom->Set_Scale(D3DXVECTOR3((m_iHP-0.02f), 0.08f, 0.08f));

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(_uint(0))))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

void CUI_Enemy_HPElement::Set_HP_UI(float iHP)
{
	if(!m_bDead)
		m_iHP = iHP;
}



HRESULT CUI_Enemy_HPElement::SetUp_Components()
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
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_EnemyHPElement"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Enemy_HPElement::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CUI_Enemy_HPElement::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CUI_Enemy_HPElement * CUI_Enemy_HPElement::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Enemy_HPElement*	pInstance = new CUI_Enemy_HPElement(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Enemy_HPElement::Clone(void * pArg)
{
	CUI_Enemy_HPElement*	pInstance = new CUI_Enemy_HPElement(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CUI_Enemy_HPElement::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
