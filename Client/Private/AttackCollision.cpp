#include "stdafx.h"
#include "AttackCollision.h"
#include "GameInstance.h"
#include "../Public/MyTerrain.h"

CAttackCollision::CAttackCollision(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject(pGraphic_Device)
{

}

CAttackCollision::CAttackCollision(const CAttackCollision & rhs)
	: CLandObject(rhs)
{

}

HRESULT CAttackCollision::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT CAttackCollision::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		ATTACK_COLLISION AtkColInfo = *(ATTACK_COLLISION*)pArg;
		m_AtkClass = AtkColInfo.eAtkClass;
		m_vSize = AtkColInfo.vSize;

		if (FAILED(SetUp_Components(AtkColInfo)))
			return E_FAIL;

		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(1.7f, 0.f, 0.5f));

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, AtkColInfo.vPosition);

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();

		Set_Terrain( dynamic_cast<CTerrain*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Terrain")));
	}

	return S_OK;
}

void CAttackCollision::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


}

void CAttackCollision::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	__super::Late_Tick(fTimeDelta);


	// CH 추가
	// 충돌 처리를 위한 Collider Position Set
	m_pColliderCom->Set_Position(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_iTerrainIndex = m_pTerrain->Get_TerrainRange(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_iTerrainIndexX, m_iTerrainIndexY);


	_tchar* tString;

	if (m_AtkClass == ATK_PLAYER)
		tString = L"PlayerAttack";
	else/* if (m_AtkClass == ATK_ENEMY)*/
		tString = L"EnemyAttack";


	
	m_pCollisionCom->Add_Collisions(m_iTerrainIndex, tString, this);

	
		if (m_iTerrainIndex % 20 > 0)
		{
			m_pCollisionCom->Add_Collisions(m_iTerrainIndex - 1, tString, this);
			if (m_iTerrainIndex > 20)
			{
				m_pCollisionCom->Add_Collisions(m_iTerrainIndex - 21, tString, this);
			}
			if (m_iTerrainIndex < 380)
			{
				m_pCollisionCom->Add_Collisions(m_iTerrainIndex + 19, tString, this);
			}
		}
		if (m_iTerrainIndex > 20)
		{
			m_pCollisionCom->Add_Collisions(m_iTerrainIndex - 20, tString, this);
		}
		if (m_iTerrainIndex % 20 < 19)
		{
			m_pCollisionCom->Add_Collisions(m_iTerrainIndex + 1, tString, this);
			if (m_iTerrainIndex > 20)
			{
				m_pCollisionCom->Add_Collisions(m_iTerrainIndex - 19, tString, this);
			}
			if (m_iTerrainIndex < 380)
			{
				m_pCollisionCom->Add_Collisions(m_iTerrainIndex + 21, tString, this);
			}
		}
		if (m_iTerrainIndex < 380)
		{
			m_pCollisionCom->Add_Collisions(m_iTerrainIndex + 20, tString, this);
		}

	m_bDead = true;
}

HRESULT CAttackCollision::Render()
{

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	//m_pColliderCom->Render(); // CH 추가
							  //m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAttackCollision::SetUp_Components(ATTACK_COLLISION AtkColInfo)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer `*/
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// CH 추가
	/* For.Com_Collider */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, AtkColInfo.vSize)))
	{
		MSGBOX(L"Failed : Add_Components Prototype_Component_Collider");
		return E_FAIL;
	}

	/* For.Com_Collision */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
	{
		MSGBOX(L"Failed : Add_Components Com_Collision");
		return E_FAIL;
	}


	CBattle::BATTLEDATA		BattleData;
	ZeroMemory(&BattleData, sizeof(CBattle::BATTLEDATA));

	BattleData.iAtk = (_uint)AtkColInfo.fAttack;
	BattleData.iDeffence = 0;
	BattleData.iMaxHP = 0;
	BattleData.iHP = 0;
	BattleData.vAttackDir = AtkColInfo.vAttackDir;
	BattleData.fAttacktedPower = AtkColInfo.fAttacktedPower;

	/* For.Com_Battle */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Battle"), TEXT("Com_Battle"), (CComponent**)&m_pBattleCom, &BattleData)))
	{
		MSGBOX(L"Failed : Add_Components Com_Collision");
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CAttackCollision::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); // CH 추가

	return S_OK;
}

HRESULT CAttackCollision::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); // CH 추가

	return S_OK;
}

CAttackCollision * CAttackCollision::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAttackCollision*	pInstance = new CAttackCollision(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAttackCollision::Clone(void * pArg)
{
	CAttackCollision*	pInstance = new CAttackCollision(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CAttackCollision::Free()
{
	__super::Free();

	Safe_Release(m_pCollisionCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pBattleCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
}
