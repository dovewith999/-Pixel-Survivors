#include "stdafx.h"
#include "..\Public\Item_Timer.h"
#include "GameInstance.h"
#include "Player.h"
#include "Monster.h"

#include "Item_TimerEffect.h"

CItem_Timer::CItem_Timer(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CItem(pGraphic_Device)
{
}

CItem_Timer::CItem_Timer(const CItem_Timer & rhs)
	: CItem(rhs)
{
}

HRESULT CItem_Timer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem_Timer::NativeConstruct(void * pArg)
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

	return S_OK;
}

void CItem_Timer::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bActive)
	{
		Active_Item();
		Set_Dead();
	}

	//if (1 <= m_iCollisionCnt)
	//{
	//	_float3 vCurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//	_float3 m_vDistance = vCurrentPos - m_tItemInit.vPosition;

	//	if (m_fRange < D3DXVec3Length(&m_vDistance))
	//		m_bReturn = true;

	//	if (true == m_bReturn)
	//	{
	//		m_vDirection = vCurrentPos - m_pPlayer->Get_Position();
	//		m_pTransformCom->Go_Direction(m_vDirection * -1.f, 10.f, fTimeDelta);
	//	}
	//	else
	//	{
	//		m_vDirection.x = fabs(vCurrentPos.x - m_pPlayer->Get_Position().x);
	//		m_vDirection.y = fabs(vCurrentPos.y - m_pPlayer->Get_Position().y);
	//		m_vDirection.z = fabs(vCurrentPos.z - m_pPlayer->Get_Position().z);

	//		m_pTransformCom->Go_Direction(m_vDirection, 5.f, fTimeDelta);

	//		m_iCollisionCnt = 1;
	//	}
	//}

}

void CItem_Timer::Late_Tick(_float fTimeDelta)
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

HRESULT CItem_Timer::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	Set_Billboard();

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(0)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

void CItem_Timer::Active_Item()
{
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return;

	Safe_AddRef(pGameInstance);

	m_MonsterList = *pGameInstance->Get_Layer(LEVEL_GAMEPLAY, L"Layer_Enemy");

	ITEM tItem = { m_pPlayer->Get_Position(), 0 };

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Item_Timer_Effect", L"Prototype_GameObject_Item_Timer_Effect", &tItem)))
		return;

	dynamic_cast<CItem_TimerEffect*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Timer_Effect"))->Set_Terrain(m_pTerrain);
	dynamic_cast<CItem_TimerEffect*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Timer_Effect"))->Set_Player(dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player")));

	Safe_Release(pGameInstance);

	if (0 >= m_MonsterList.size())
		return;

	for (auto& iter : m_MonsterList)
	{
		dynamic_cast<CMonster*>(iter)->Set_Freeze();
	}

}

void CItem_Timer::Set_Billboard()
{
	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

}

HRESULT CItem_Timer::SetUp_Components()
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
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item_Timer"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
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

HRESULT CItem_Timer::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CItem_Timer::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CItem_Timer * CItem_Timer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItem_Timer*	pInstance = new CItem_Timer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CItem_Timer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem_Timer::Clone(void * pArg)
{
	CItem_Timer*	pInstance = new CItem_Timer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CItem_Timer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Timer::Free()
{
	__super::Free();

	//for (auto& iter : m_ItemExpList)
	//	Safe_Release(*iter);
	//m_ItemExpList.clear();
}
