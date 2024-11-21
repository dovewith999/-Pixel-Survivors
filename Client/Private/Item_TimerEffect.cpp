#include "stdafx.h"
#include "..\Public\Item_TimerEffect.h"
#include "GameInstance.h"
#include "Player.h"
#include "Monster.h"

CItem_TimerEffect::CItem_TimerEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CItem(pGraphic_Device)
{
}

CItem_TimerEffect::CItem_TimerEffect(const CItem_TimerEffect & rhs)
	: CItem(rhs)
{
}

HRESULT CItem_TimerEffect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem_TimerEffect::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
	{
		MSGBOX(L"Failed : SetUp_Components");
		return E_FAIL;
	}

	if (nullptr != pArg)
	{
		m_tItemInit = *(ITEM*)pArg;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tItemInit.vPosition);
	}

	m_pTransformCom->TurnAngle(D3DXVECTOR3(1.f, 0.f, 0.f), 85.f);
	m_pTransformCom->Set_Scale(_float3(3.f, 3.f, 3.f));

	return S_OK;
}

void CItem_TimerEffect::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fDeadTime -= fTimeDelta;
	if (m_fDeadTime <= 0)
	{
		Set_Dead();
	}

	_float3 vPos = m_pPlayer->Get_Position();
	vPos.y += 0.5f;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	if (m_bGlow)
	{
		m_fGlow += 0.9f * fTimeDelta;
		if (1.f <= m_fGlow)
			m_bGlow = false;
	}
	else
	{
		m_fGlow -= 0.9f * fTimeDelta;
		if (0.5f >= m_fGlow)
			m_bGlow = true;
	}
}

void CItem_TimerEffect::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	__super::Late_Tick(fTimeDelta);

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_UI, this);

}

HRESULT CItem_TimerEffect::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	//Set_Billboard();

	//if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
	//	return E_FAIL;

	//if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(_uint(m_fFrame))))
	//	return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	_float4x4	WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	D3DXMatrixTranspose(&WorldMatrix, &WorldMatrix);

	_float4x4	ViewMat;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMat);

	_float4x4	ProjMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);

	m_pShaderCom->SetUp_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
	m_pShaderCom->SetUp_RawValue("g_ViewMatrix", D3DXMatrixTranspose(&ViewMat, &ViewMat), sizeof(_float4x4));
	m_pShaderCom->SetUp_RawValue("g_ProjMatrix", D3DXMatrixTranspose(&ProjMatrix, &ProjMatrix), sizeof(_float4x4));
	m_pShaderCom->SetUp_Texture("g_Texture", m_pTextureCom->Get_Texture(m_fFrame));

	_float vR = 0.f;
	_float vG = 0.f;
	_float vB = 255.f;

	m_pShaderCom->SetUp_RawValue("g_fColorR", &vR, sizeof(_float));
	m_pShaderCom->SetUp_RawValue("g_fColorG", &vG, sizeof(_float));
	m_pShaderCom->SetUp_RawValue("g_fColorB", &vB, sizeof(_float));
	m_pShaderCom->SetUp_RawValue("g_fAlpha", &m_fGlow, sizeof(_float));

	m_pShaderCom->Begin_Shader(2);

	m_pVIBufferCom->Render();

	m_pShaderCom->End_Shader();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

void CItem_TimerEffect::Set_Billboard()
{
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

}

HRESULT CItem_TimerEffect::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	{
		MSGBOX(L"Failed : Add_Components Com_Transform");
		return E_FAIL;
	}

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	{
		MSGBOX(L"Failed : Add_Components Com_Renderer");
		return E_FAIL;
	}

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item_Timer_Effect"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
	{
		MSGBOX(L"Failed : Add_Components Com_Texture");
		return E_FAIL;
	}

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
	{
		MSGBOX(L"Failed : Add_Components Com_VIBuffer");
		return E_FAIL;
	}

	/* For.Com_Collision */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
	{
		MSGBOX(L"Failed : Add_Components Com_Collision");
		return E_FAIL;
	}

	/* For.Com_Collider */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, _float3(1.f, 1.f, 0.5f))))
	{
		MSGBOX(L"Failed : Add_Components Prototype_Component_Collider");
		return E_FAIL;
	}

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Rect"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem_TimerEffect::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CItem_TimerEffect::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CItem_TimerEffect * CItem_TimerEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem_TimerEffect*	pInstance = new CItem_TimerEffect(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CItem_TimerEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem_TimerEffect::Clone(void * pArg)
{
	CItem_TimerEffect*	pInstance = new CItem_TimerEffect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CItem_TimerEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_TimerEffect::Free()
{
	__super::Free();

	//for (auto& iter : m_ItemExpList)
	//	Safe_Release(*iter);
	//m_ItemExpList.clear();

	Safe_Release(m_pShaderCom);
}
