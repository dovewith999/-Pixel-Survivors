#include "stdafx.h"
#include "..\Public\Monster.h"
#include "GameInstance.h"

#include "Player.h"
#include "../Public/Terrain.h"
#include "Player.h"
#include "Gun.h"
#include "Item_EXP.h"
#include "Item_Treasure.h"

#include <ctime>
#include <random>
#include <functional>

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject(pGraphic_Device)
{
}

CMonster::CMonster(const CMonster & rhs)
	: CLandObject(rhs)
	, m_iHp(rhs.m_iHp)
{

}

HRESULT CMonster::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT CMonster::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	// Target : Player, Tower, Monster
	m_pTarget = pGameInstance->Get_Target(D3DXVECTOR3(1.f, 1.f, 1.f), 200.f, LEVEL_GAMEPLAY, L"Layer_Player");

	m_pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player"));
	Safe_Release(pGameInstance);

	return S_OK;
}

void CMonster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (true == m_bFreeze)
	{
		m_fFreezeTime += 1.f * fTimeDelta;
		if (m_fFreezeTime >= m_fFreezeLimit)
		{
			m_bFreeze = false;
			m_fFreezeTime = 0.f;
		}
	}

	if (m_eState != STATE_DIE)
	{
		if (false == m_bFreeze && !m_pPlayer->Get_LevelUp())
		{
			--m_iAttackDelay;

			//Chase or Attack Target
			if (nullptr != m_pTarget)
			{
				CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pTarget->Find_Component(L"Com_Transform"));

				D3DXVECTOR3 TargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);

				if (m_pTransformCom)
					TargetPos.y = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
				else
					TargetPos.y = m_pAniTransformCom->Get_State(CAniTransform::ANI_TRANS_POSITION).y;

				_float fDistance = 0.f;
				if (m_pTransformCom)
					fDistance = D3DXVec3Length(&(TargetPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION)));
				else
					fDistance = D3DXVec3Length(&(TargetPos - m_pAniTransformCom->Get_State(CAniTransform::ANI_TRANS_POSITION)));
				// Attack

				if (3.f >= fDistance)
				{
					m_eState = STATE_ATTACK;
					m_eAniState = TEST_STATE_ATTACK;
					Attack_Target();
				}
				else // Chase
				{
					m_eAniState = TEST_STATE_IDLE;

					if (m_pTransformCom)
						m_pTransformCom->Chase(TargetPos, fTimeDelta, 0.1f);
					else
						m_pAniTransformCom->Chase(TargetPos, fTimeDelta, 0.1f);
				}
			}
		}
	}

	if (true == m_bAttacked)
	{
		m_fAttackTime += 5.f * fTimeDelta;
		if (m_fAttackTime >= m_fAttackLimit)
		{
			m_bAttacked = false;
			m_fAttackTime = 0.f;
		}
	}

	//공격 받았을때의 처리
	if (m_pBattleCom->Get_Attackted())
	{
		m_bAttacked = true;
		if (!m_strGameObjecttag.Compare(L"Animation"))
		{
			m_eAniState = TEST_STATE_STUN;
			m_pBattleCom->Process_HP(-1);
		}


		m_pBattleCom->Set_Attackted();
		if (m_pBattleCom->Get_HP() <= 0)
		{
			m_eState = STATE_DIE;
			Create_Item();
			/*if (nullptr != m_pHPBar)
				m_pHPBar->Set_Dead();*/
			if (!m_strGameObjecttag.Compare(L"Animation"))
			{
				Set_Dead();
			}
		}
		else
		{
			m_bHpShow = true;
		if(m_pHPBar)
			m_pHPBar->Set_Show(m_bHpShow);
		}
		if (nullptr != m_pHPBar)
			m_pHPBar->Set_HPBar_UI((float)m_pBattleCom->Get_HP() / (float)m_pBattleCom->Get_MaxHP());

	}
}

void CMonster::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	__super::Late_Tick(fTimeDelta);


}

HRESULT CMonster::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;



	return S_OK;
}

void CMonster::Attack_Target()
{
	if (nullptr == m_pTarget)
		return;


	ATTACK_COLLISION Attack_Collision;
	ZeroMemory(&Attack_Collision, sizeof(ATTACK_COLLISION));

	Attack_Collision.fAttack = (float)m_pBattleCom->Get_FinalAtk();

	if (m_pTransformCom)
		Attack_Collision.vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	else
		Attack_Collision.vPosition = m_pAniTransformCom->Get_State(CAniTransform::ANI_TRANS_POSITION);

	Attack_Collision.vSize = D3DXVECTOR3(1.f, 1.f, 1.f);

	if (m_pTransformCom)
		Attack_Collision.vAttackDir = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	else
		Attack_Collision.vAttackDir = m_pAniTransformCom->Get_State(CAniTransform::ANI_TRANS_LOOK);

	Attack_Collision.fAttacktedPower = 3.f;
	Attack_Collision.eAtkClass = ATK_ENEMY;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Attack"), TEXT("Prototype_GameObject_AttackCollision"), &Attack_Collision)))
		return;

	Safe_Release(pGameInstance);

	m_iAttackDelay = m_pBattleCom->Get_FinalAttackDelay();

	m_eAtkMotion = ATKMOTION_1;
}

void CMonster::AttackMotion1()
{
	/*if (m_fAttackAngle >= 360.f)
	{
		m_fAttackAngle = 0.f;
		m_fJumpPower = 0.f;
		m_fTimeAccumulate = 0.f;
		m_eAtkMotion = ATKMOTION_END;
		return;
	}
	if (m_fAttackAngle == 200.f)
	{
		m_fJumpPower = 1.f;
		m_fTimeAccumulate = 0.f;
	}
	if (m_pTransformCom)
		m_pTransformCom->TurnAngle(D3DXVECTOR3(0.f, 1.f, 0.f), 10.f);
	else
		m_pAniTransformCom->TurnAngle(D3DXVECTOR3(0.f, 1.f, 0.f), 10.f);

	m_fAttackAngle += 10.f;*/

}

// 0511
HRESULT CMonster::Create_Item()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	ITEM tItem{};

	std::random_device rd;

	// random_device 를 통해 난수 생성 엔진을 초기화 한다.
	std::mt19937 gen(rd());

	//mt19937 engine((unsigned int)time(NULL));         // MT19937 난수 엔진
	uniform_int_distribution<int> distribution(0, 50);   // 생성 범위
	auto generator = bind(distribution, gen);
	_int iTextureNum = generator();

	switch (m_eLevel)
	{
	case LEVEL_1:
		if(m_pTransformCom)
			tItem = { m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0 };
		else if(m_pAniTransformCom)
			tItem = { m_pAniTransformCom->Get_State(CAniTransform::ANI_TRANS_POSITION), 0 };

		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Item_EXP", L"Prototype_GameObject_Item_EXP", &tItem)))
			return E_FAIL;
		dynamic_cast<CItem_EXP*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_EXP"))->Set_Terrain(m_pTerrain);
		dynamic_cast<CItem_EXP*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_EXP"))->Set_Player(m_pPlayer);
		break;
	case LEVEL_2:
		if (m_pTransformCom)
			tItem = { m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1 };
		else if (m_pAniTransformCom)
			tItem = { m_pAniTransformCom->Get_State(CAniTransform::ANI_TRANS_POSITION), 1 };

		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Item_EXP", L"Prototype_GameObject_Item_EXP", &tItem)))
			return E_FAIL;
		dynamic_cast<CItem_EXP*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_EXP"))->Set_Terrain(m_pTerrain);
		dynamic_cast<CItem_EXP*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_EXP"))->Set_Player(m_pPlayer);
		break;
	case LEVEL_3:
		if (0 <= iTextureNum && 5 > iTextureNum)
		{
			if (m_pTransformCom)
				tItem = { m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0 };
			else if (m_pAniTransformCom)
				tItem = { m_pAniTransformCom->Get_State(CAniTransform::ANI_TRANS_POSITION), 0 };

			if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Item_Treasure", L"Prototype_GameObject_Item_Treasure", &tItem)))
				return E_FAIL;
			dynamic_cast<CItem_Treasure*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Treasure"))->Set_Terrain(m_pTerrain);
			dynamic_cast<CItem_Treasure*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Treasure"))->Set_Player(m_pPlayer);
		}
		else
		{
			if (m_pTransformCom)
				tItem = { m_pTransformCom->Get_State(CTransform::STATE_POSITION), 2 };
			else if (m_pAniTransformCom)
				tItem = { m_pAniTransformCom->Get_State(CAniTransform::ANI_TRANS_POSITION), 2 };

			if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Item_EXP", L"Prototype_GameObject_Item_EXP", &tItem)))
				return E_FAIL;
			dynamic_cast<CItem_EXP*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_EXP"))->Set_Terrain(m_pTerrain);
			dynamic_cast<CItem_EXP*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_EXP"))->Set_Player(m_pPlayer);
		}
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CMonster::Search_Target(const _tchar* pTargetTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	// Target : Player, Tower, Monster
	if (m_pTransformCom)
		m_pTarget = pGameInstance->Get_Target(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 200.f, LEVEL_GAMEPLAY, pTargetTag);
	else
		m_pTarget = pGameInstance->Get_Target(m_pAniTransformCom->Get_State(CAniTransform::ANI_TRANS_POSITION), 200.f, LEVEL_GAMEPLAY, pTargetTag);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CMonster::Set_MasterCube(CMasterCube* MasterCube)
{
	if (nullptr == MasterCube)
		return E_FAIL;

	m_MasterCube = MasterCube;
	if (m_pTransformCom)
		m_MasterCube->Set_BindTransform(m_pTransformCom);

	return S_OK;
}

HRESULT CMonster::Set_HPBar(CUI_Enemy_HPBar * HPBar)
{
	if (nullptr == HPBar)
		return E_FAIL;

	m_pHPBar = HPBar;

	return S_OK;
}

HRESULT CMonster::Set_BindingTransform_HPBar()
{
	if (nullptr == m_pHPBar)
		return E_FAIL;

	if (nullptr == m_pTransformCom)
		return E_FAIL;

	if (m_pTransformCom)
	{
		if(m_pHPBar)
		m_pHPBar->Set_Enemy_Transform(m_pTransformCom);
	}
	else
	{
		if(m_pHPBar)
		m_pHPBar->Set_Enemy_Transform(m_pAniTransformCom);
	}
	return S_OK;
}

HRESULT CMonster::Set_BindingTransform_HPBar(CAniTransform* _pAniTransform)
{
	if (nullptr == m_pHPBar)
		return E_FAIL;

	if (nullptr == m_pAniTransformCom)
		return E_FAIL;

	m_pHPBar->Set_Enemy_Transform(_pAniTransform);

	return S_OK;
}


void CMonster::Free()
{
	__super::Free();

	if (nullptr != m_pHPBar)
		m_pHPBar->Set_Dead();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pCollisionCom);
	Safe_Release(m_pBattleCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);

}
