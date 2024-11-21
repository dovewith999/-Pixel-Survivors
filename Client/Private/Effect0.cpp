#include "stdafx.h"
#include "..\Public\Effect0.h"
#include "GameInstance.h"

#include "Player.h"


CEffect0::CEffect0(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CEffect(pGraphic_Device)
{
}

CEffect0::CEffect0(const CEffect0 & rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect0::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect0::NativeConstruct(void * pArg)
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
		D3DXVECTOR3 vPos = *(D3DXVECTOR3*)pArg;
		vPos.y -= 1.f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

		m_pTransformCom->TurnAngle(D3DXVECTOR3(1.f, 0.f, 0.f), 90.f);
	}

	return S_OK;
}

void CEffect0::Tick(_float fTimeDelta)
{
	m_fFrame += 12 * fTimeDelta;

	
	m_vCollisionScale += D3DXVECTOR3(fTimeDelta, fTimeDelta, fTimeDelta);

	m_pTransformCom->Set_Scale(m_vCollisionScale);
	

	

	if (m_fFrame >= 10)
	{
		Set_Dead();
	}
}

void CEffect0::Late_Tick(_float fTimeDelta)
{
	

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CEffect0::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

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

void CEffect0::CreateAttackCollision()
{
	
}

HRESULT CEffect0::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));


	TransformDesc.fSpeedPerSec = 6.f;
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
	

	m_pTransformCom->Set_Scale(m_vCollisionScale);

	

	

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect0"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
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

	

	return S_OK;
}

HRESULT CEffect0::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CEffect0::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CEffect0 * CEffect0::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect0*	pInstance = new CEffect0(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect0::Clone(void * pArg)
{
	CEffect0*	pInstance = new CEffect0(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect0::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
