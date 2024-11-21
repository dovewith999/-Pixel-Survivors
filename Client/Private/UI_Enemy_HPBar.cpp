#include "stdafx.h"
#include "..\Public\UI_Enemy_HPBar.h"
#include "GameInstance.h"

CUI_Enemy_HPBar::CUI_Enemy_HPBar(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject(pGraphic_Device)
{

}

CUI_Enemy_HPBar::CUI_Enemy_HPBar(const CUI_Enemy_HPBar & rhs)
	: CUIObject(rhs)
{

}

HRESULT CUI_Enemy_HPBar::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;



	return S_OK;
}


HRESULT CUI_Enemy_HPBar::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_fDistance = *(float*)pArg;
	}

	m_pTransformCom->Set_Scale(D3DXVECTOR3(1.f, 0.1f, 0.1f));

	return S_OK;
}

void CUI_Enemy_HPBar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);




}

void CUI_Enemy_HPBar::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	__super::Late_Tick(fTimeDelta);

	if(m_bShow)
		m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHABLEND, this);

	
}

HRESULT CUI_Enemy_HPBar::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (!m_bShow)
		return S_OK;

	_float3 vEnemyPos = m_pEnemyTransform->Get_State(CTransform::STATE_POSITION);
	vEnemyPos.y += m_fDistance;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vEnemyPos);

	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

	if (nullptr == m_pEnemyTransform)
		return E_FAIL;


	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Enemy_HPBar::Add_Element(CUI_Enemy_HPElement* Element)
{
	if (nullptr == Element)
		return E_FAIL;

	m_pHPElement = Element;
	m_pHPElement->Set_Enemy_Transform(m_pTransformCom);

	return S_OK;
}

HRESULT CUI_Enemy_HPBar::Set_HPBar_UI(float iHP)
{
	if (nullptr == m_pHPElement)
		return E_FAIL;

	if (m_bDead)
		return E_FAIL;

	m_pHPElement->Set_HP_UI(iHP);

	return S_OK;
}

void CUI_Enemy_HPBar::Set_Enemy_Transform(CTransform * pEnemyTransform)
{
	if (nullptr == pEnemyTransform)
		return;

	m_pEnemyTransform = pEnemyTransform;
}



void CUI_Enemy_HPBar::Set_Enemy_Transform(CAniTransform * pEnemyTransform)
{
	if (nullptr == pEnemyTransform)
		return;

	m_pEnenmyAniTransform = pEnemyTransform;
}



HRESULT CUI_Enemy_HPBar::SetUp_Components()
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
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_EnemyHPBar"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Enemy_HPBar::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Enemy_HPBar::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	

	return S_OK;
}

CUI_Enemy_HPBar * CUI_Enemy_HPBar::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Enemy_HPBar*	pInstance = new CUI_Enemy_HPBar(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Enemy_HPBar::Clone(void * pArg)
{
	CUI_Enemy_HPBar*	pInstance = new CUI_Enemy_HPBar(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CUI_Enemy_HPBar::Free()
{
	__super::Free();

	if (m_bDead)
		m_pHPElement->Set_Dead();
	

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
