#include "stdafx.h"
#include "..\Public\Item_HP.h"
#include "GameInstance.h"
#include "Player.h"


CItem_HP::CItem_HP(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CItem(pGraphic_Device)
{
}

CItem_HP::CItem_HP(const CItem_HP & rhs)
	: CItem(rhs)
{
}

HRESULT CItem_HP::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem_HP::NativeConstruct(void * pArg)
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

	//wsprintf(m_szRenderTag, m_szRenderComTag, m_iDrawID);

	return S_OK;
}

void CItem_HP::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bActive)
	{
		++m_iCollisionCnt;
		if (2 <= m_iCollisionCnt)
		{
			CBattle* pBattle = dynamic_cast<CBattle*>(m_pPlayer->Find_Component(L"Com_Battle"));
			_uint iHp = pBattle->Get_HP();
			pBattle->Set_HP(m_iHP + iHp);
			Set_Dead();
		}
		else
			m_bActive = false;
	}

	if (1 <= m_iCollisionCnt)
	{
		_float3 vCurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float3 m_vDistance = vCurrentPos - m_tItemInit.vPosition;

		if (m_fRange < D3DXVec3Length(&m_vDistance))
			m_bReturn = true;

		if (true == m_bReturn)
		{
			m_vDirection = vCurrentPos - m_pPlayer->Get_Position();
			m_pTransformCom->Go_Direction(m_vDirection * -1.f, 10.f, fTimeDelta);
		}
		else
		{
			m_vDirection.x = fabs(vCurrentPos.x - m_pPlayer->Get_Position().x);
			m_vDirection.y = fabs(vCurrentPos.y - m_pPlayer->Get_Position().y);
			m_vDirection.z = fabs(vCurrentPos.z - m_pPlayer->Get_Position().z);

			m_pTransformCom->Go_Direction(m_vDirection, 5.f, fTimeDelta);

			m_iCollisionCnt = 1;
		}
	}

}

void CItem_HP::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	if (2 <= m_iCollisionCnt)
		return;

	__super::Late_Tick(fTimeDelta);

	m_iTerrainIndex = m_pTerrain->Get_TerrainRange(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_iTerrainIndexX, m_iTerrainIndexY);
	__super::SetUp_OnTerrain(m_pTransformCom, LEVEL_GAMEPLAY, TEXT("Layer_Terrain"));

	m_pColliderCom->Set_Position(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pCollisionCom->Add_Collisions(m_iTerrainIndex, L"Item", this);

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT CItem_HP::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	Set_Billboard();

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	//if (RENDERSET_TRUE == m_eRenderSet)
	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

void CItem_HP::Set_Billboard()
{
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

}

HRESULT CItem_HP::SetUp_Components()
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
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item_HP"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
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

	return S_OK;
}

HRESULT CItem_HP::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CItem_HP::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CItem_HP * CItem_HP::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem_HP*	pInstance = new CItem_HP(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem_HP::Clone(void * pArg)
{
	CItem_HP*	pInstance = new CItem_HP(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_HP::Free()
{
	__super::Free();
}
