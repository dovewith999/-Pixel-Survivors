#include "stdafx.h"
#include "..\Public\Player.h"
#include "GameInstance.h"
#include "../Public/MyTerrain.h"
#include "Camera_Player.h"
#include "Monster.h"
#include "UI_IconBox.h"
#include "UI_Inventory.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLandObject(pGraphic_Device)
{

}

CPlayer::CPlayer(const CPlayer & rhs)
	: CLandObject(rhs)
{

}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT CPlayer::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(30.f, 0.f, 30.f));

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	m_pCamera_Player = dynamic_cast<CCamera_Player*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Camera")); 
	Safe_Release(pGameInstance);

	//무기 실험용
	//m_Active.emplace(PLASMA, 1);
	//m_Active.emplace(BIBLE, 1);
	//m_Active.emplace(GARLIC, 1);
	//m_Active.emplace(LIGHTNING, 1);
	//m_Active.emplace(FIRE_BALL, 1);

	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if(m_fWeaponLevelCool > 0)
		m_fWeaponLevelCool -= fTimeDelta;
	
	m_pTransformCom->TurnAngle(m_pTransformCom->Get_State(CTransform::STATE_LOOK), -m_fTurnAngle);

	if (m_fTurnAngle >= 30.f)
		m_bTurnLR = false;
	else if (m_fTurnAngle <= -30.f)
		m_bTurnLR = true;

	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return;

	Safe_AddRef(pGameInstance);

	if (GetAsyncKeyState('1') < 0)
	{
		bool noop = false;
		for (auto& iter = m_ActiveList.begin(); iter != m_ActiveList.end(); iter++)
		{
			if (*iter == PLASMA)
				noop = true;
		}
		if (noop == false)
		{
			m_Active.emplace(PLASMA, 1);
			m_ActiveList.push_back(PLASMA);
		}
	}

	if (GetAsyncKeyState('2') < 0)
	{
		bool noop = false;
		for (auto& iter = m_ActiveList.begin(); iter != m_ActiveList.end(); iter++)
		{
			if (*iter == BIBLE)
				noop = true;
		}
		if (noop == false)
		{
			m_Active.emplace(BIBLE, 1);
			m_ActiveList.push_back(BIBLE);
		}
	}

	if (GetAsyncKeyState('3') < 0)
	{
		bool noop = false;
		for (auto& iter = m_ActiveList.begin(); iter != m_ActiveList.end(); iter++)
		{
			if (*iter == GARLIC)
				noop = true;
		}
		if (noop == false)
		{
			m_Active.emplace(GARLIC, 1);
			m_ActiveList.push_back(GARLIC);
		}
	}

	if (GetAsyncKeyState('4') < 0)
	{
		bool noop = false;
		for (auto& iter = m_ActiveList.begin(); iter != m_ActiveList.end(); iter++)
		{
			if (*iter == LIGHTNING)
				noop = true;
		}
		if (noop == false)
		{
			m_Active.emplace(LIGHTNING, 1);
			m_ActiveList.push_back(LIGHTNING);
		}
	}

	if (GetAsyncKeyState('5') < 0)
	{
		bool noop = false;
		for (auto& iter = m_ActiveList.begin(); iter != m_ActiveList.end(); iter++)
		{
			if (*iter == FIRE_BALL)
				noop = true;
		}
		if (noop == false)
		{
			m_Active.emplace(FIRE_BALL, 1);
			m_ActiveList.push_back(FIRE_BALL);
		}
	}

	if (GetAsyncKeyState('6') < 0)
	{
		bool noop = false;
		for (auto& iter = m_ActiveList.begin(); iter != m_ActiveList.end(); iter++)
		{
			if (*iter == CAT)
				noop = true;
		}
		if (noop == false)
		{
			m_Active.emplace(CAT, 1);
			m_ActiveList.push_back(CAT);
		}
	}

	if (GetAsyncKeyState('9') < 0)
	{
		pGameInstance->Delete_LayerObject(LEVEL_GAMEPLAY, TEXT("Layer_Garlic"));
		for (int ti = 0; ti < 15; ti++)
		{
			pGameInstance->Delete_LayerObject(LEVEL_GAMEPLAY, TEXT("Layer_Bible"));
		}
		m_Active.clear();
		m_ActiveList.clear();

		m_fActive_1_Cooltime = 0.f;
		m_fActive_2_Cooltime = 0.f;
		m_fActive_3_Cooltime = 0.f;
		m_fActive_4_Cooltime = 0.f;
		m_fActive_5_Cooltime = 0.f;
		m_fActive_6_Cooltime = 0.f;
	}

	if (GetAsyncKeyState('C') < 0 && m_fWeaponLevelCool <= 0)
	{
		for (auto iter : m_Active)
		{
			if (iter.first == PLASMA)
			{
				if (m_Active[PLASMA] > 1)
					m_Active[PLASMA] -= 1;
			}
			else if (iter.first == BIBLE)
			{
				if (m_Active[BIBLE] > 1)
					m_Active[BIBLE] -= 1;
			}
			else if (iter.first == GARLIC)
			{
				if (m_Active[GARLIC] > 1)
					m_Active[GARLIC] -= 1;
			}
			if (iter.first == LIGHTNING)
			{
				if (m_Active[LIGHTNING] > 1)
					m_Active[LIGHTNING] -= 1;
			}
			if (iter.first == FIRE_BALL)
			{
				if (m_Active[FIRE_BALL] > 1)
					m_Active[FIRE_BALL] -= 1;
			}
			if (iter.first == CAT)
			{
				if (m_Active[CAT] > 1)
					m_Active[CAT] -= 1;
			}
		}

		m_fActive_1_Cooltime = 0.f;
		m_fActive_2_Cooltime = 0.f;
		m_fActive_3_Cooltime = 0.f;
		m_fActive_4_Cooltime = 0.f;
		m_fActive_5_Cooltime = 0.f;
		m_fActive_6_Cooltime = 0.f;

		m_fWeaponLevelCool = 1.f;
	}
	if (GetAsyncKeyState('V') < 0 && m_fWeaponLevelCool <= 0)
	{
		for (auto iter : m_Active)
		{
			if (iter.first == PLASMA)
			{
				if (m_Active[PLASMA] < 8)
					m_Active[PLASMA] += 1;
			}
			else if (iter.first == BIBLE)
			{
				if (m_Active[BIBLE] < 8)
					m_Active[BIBLE] += 1;
			}
			else if (iter.first == GARLIC)
			{
				if (m_Active[GARLIC] < 8)
					m_Active[GARLIC] += 1;
			}
			else if (iter.first == LIGHTNING)
			{
				if (m_Active[LIGHTNING] < 8)
					m_Active[LIGHTNING] += 1;
			}
			else if (iter.first == FIRE_BALL)
			{
				if (m_Active[FIRE_BALL] < 8)
					m_Active[FIRE_BALL] += 1;
			}
			else if (iter.first == CAT)
			{
				if (m_Active[CAT] < 8)
					m_Active[CAT] += 1;
			}
		}

		m_fActive_1_Cooltime = 0.f;
		m_fActive_2_Cooltime = 0.f;
		m_fActive_3_Cooltime = 0.f;
		m_fActive_4_Cooltime = 0.f;
		m_fActive_5_Cooltime = 0.f;
		m_fActive_6_Cooltime = 0.f;

		m_fWeaponLevelCool = 1.f;
	}
	if (GetAsyncKeyState('B') < 0)
	{
		for (auto iter : m_Active)
		{
			if (iter.first == PLASMA)
			{
				m_Active[PLASMA] = 9;
				ResetCoolTime(PLASMA);
			}
			if (iter.first == LIGHTNING)
			{
				m_Active[LIGHTNING] = 9;
				ResetCoolTime(LIGHTNING);
			}
			if (iter.first == FIRE_BALL)
			{
				m_Active[FIRE_BALL] = 9;
				ResetCoolTime(FIRE_BALL);
			}
			if (iter.first == CAT)
			{
				m_Active[CAT] = 9;
				ResetCoolTime(CAT);
			}
			if (iter.first == BIBLE)
			{
				m_Active[BIBLE] = 9;
				ResetCoolTime(BIBLE);
			}
			if (iter.first == GARLIC)
			{
				m_Active[GARLIC] = 9;
				ResetCoolTime(GARLIC);
			}
		}
	}


	if (pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
	{
		m_pTransformCom->Go_Left(fTimeDelta);

	}


	if (pGameInstance->Get_DIKeyState(DIK_R) & 0x80)
	{
		m_pTransformCom->Turn(D3DXVECTOR3(0.f, 1.f, 0.f), fTimeDelta);
	}
	if (pGameInstance->Get_DIKeyState(DIK_T) & 0x80)
	{
		m_pTransformCom->Turn(D3DXVECTOR3(0.f, -1.f, 0.f), fTimeDelta);
	}

	if (pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
	{
		m_bOnGravity = true;
		m_bRideTerrain = false;
		m_fJumpPower = 4.f;
	}


	// 경험치 확인용 코드
	if (pGameInstance->Get_DIKeyState(DIK_Z) & 0x80)
	{
		if (0.f < m_iPlayerExp)
		{
			--m_iPlayerExp;
			cout << m_iPlayerExp << endl;
		}
	}

	else if (pGameInstance->Get_DIKeyState(DIK_C) & 0x80)
	{
		if (100.f > m_iPlayerExp)
		{
			++m_iPlayerExp;
			cout << m_iPlayerExp << endl;
		}
	}

	if (!m_bLevelUp)
	{
		if (100.f <= m_iPlayerExp)
		{
			m_bLevelUp = true;
		}
	}

	else
	{
		if (100.f > m_iPlayerExp)
		{
			m_bLevelUp = false;
		}
	}


	//공격 받았을때의 처리
	if (m_pBattleCom->Get_Attackted())
	{
		m_pBattleCom->Set_Attackted();
		m_pHPBar->Set_HPBar_UI(m_pBattleCom->Get_HP());
		/*if (m_pBattleCom->Get_HP() <= 0)
			Set_Dead();*/
	}

	//m_pMasterCube->Set_Positon(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	//플레이어 위치 출력
	//cout << "x : " << m_pTransformCom->Get_State(CTransform::STATE_POSITION).x << "y : " << m_pTransformCom->Get_State(CTransform::STATE_POSITION).y << "z : " << m_pTransformCom->Get_State(CTransform::STATE_POSITION).z << endl;

	Safe_Release(pGameInstance);

	int ti = 0;

	for (auto iter = m_ActiveList.begin(); iter != m_ActiveList.end(); iter++)
	{
		++ti;
		switch (ti)
		{
		case 1:
			if (m_fActive_1_Cooltime <= 0.f)
			{
				CreateWeapon(1, *iter, m_Active[*iter]);
			}
			else if (m_fActive_1_Cooltime > 9000.f)
			{
				
			}
			else
				m_fActive_1_Cooltime -= fTimeDelta;
			break;

		case 2:
			if (m_fActive_2_Cooltime <= 0.f)
			{
				CreateWeapon(2, *iter, m_Active[*iter]);
			}
			else if (m_fActive_2_Cooltime > 9000.f)
			{
				
			}
			else
				m_fActive_2_Cooltime -= fTimeDelta;
			break;

		case 3:
			if (m_fActive_3_Cooltime <= 0.f)
			{
				CreateWeapon(3, *iter, m_Active[*iter]);
			}
			else if (m_fActive_3_Cooltime > 9000.f)
			{

			}
			else
				m_fActive_3_Cooltime -= fTimeDelta;
			break;

		case 4:
			if (m_fActive_4_Cooltime <= 0.f)
			{
				CreateWeapon(4, *iter, m_Active[*iter]);
			}
			else if (m_fActive_4_Cooltime > 9000.f)
			{

			}
			else
				m_fActive_4_Cooltime -= fTimeDelta;
			break;

		case 5:
			if (m_fActive_5_Cooltime <= 0.f)
			{
				CreateWeapon(5, *iter, m_Active[*iter]);
			}
			else if (m_fActive_5_Cooltime > 9000.f)
			{

			}
			else
				m_fActive_5_Cooltime -= fTimeDelta;
			break;

		case 6:
			if (m_fActive_6_Cooltime <= 0.f)
			{
				CreateWeapon(6, *iter, m_Active[*iter]);
			}
			else if (m_fActive_6_Cooltime > 9000.f)
			{

			}
			else
				m_fActive_6_Cooltime -= fTimeDelta;
			break;
		}
	}


}

void CPlayer::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	__super::Late_Tick(fTimeDelta);
	
	m_iTerrainIndex = m_pTerrain->Get_TerrainRange(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_iTerrainIndexX, m_iTerrainIndexY);
	__super::SetUp_OnTerrain(m_pTransformCom, LEVEL_GAMEPLAY, TEXT("Layer_Terrain"));

	m_pColliderCom->Set_Position(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_pCollisionCom->Add_Collisions(m_iTerrainIndex, L"Player", this);
	/*if (m_iTerrainIndexX != -1)
		m_pCollisionCom->Add_Collisions(m_iTerrainIndexX, L"Player", this);
	if (m_iTerrainIndexY != -1)
		m_pCollisionCom->Add_Collisions(m_iTerrainIndexY, L"Player", this);*/

	__super::Affected_By_Gravity(m_pTransformCom, fTimeDelta);
	//m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHABLEND, this);

	

	if (m_bTurnLR)
		m_fTurnAngle += 3.f;
	else
		m_fTurnAngle -= 3.f;

	m_pTransformCom->TurnAngle(m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_fTurnAngle);
}

HRESULT CPlayer::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

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

HRESULT CPlayer::Set_Turn(D3DXVECTOR3 vVec, float fFloat)
{
	m_pTransformCom->Turn(vVec, fFloat);
	return S_OK;
}

HRESULT CPlayer::Create_Attack_Collision(ATTACK_COLLISION* Atk_Col)
{
	if (Atk_Col == nullptr)
		return E_FAIL;

	ATTACK_COLLISION Attack_Collision;
	ZeroMemory(&Attack_Collision, sizeof(ATTACK_COLLISION));
	memcpy(&Attack_Collision, Atk_Col, sizeof(ATTACK_COLLISION));

	m_pBattleCom->Set_Atk((_uint)Attack_Collision.fAttack);
	Attack_Collision.fAttack = (float)m_pBattleCom->Get_FinalAtk();
	Attack_Collision.vAttackDir = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	Attack_Collision.eAtkClass = ATK_PLAYER;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Attack"), TEXT("Prototype_GameObject_AttackCollision"), &Attack_Collision)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

void CPlayer::ResetCoolTime(WEAPONLIST eWEAPON)
{
	int ti = 0;
	for (auto iter = m_ActiveList.begin(); iter != m_ActiveList.end(); iter++)
	{
		++ti;
		if (*iter == eWEAPON)
			break;

		if (iter == m_ActiveList.end())
			return;
	}

	switch (ti)
	{
	case 1:
		m_fActive_1_Cooltime = 0.f;
		break;
	case 2:
		m_fActive_2_Cooltime = 0.f;
		break;
	case 3:
		m_fActive_3_Cooltime = 0.f;
		break;
	case 4:
		m_fActive_4_Cooltime = 0.f;
		break;
	case 5:
		m_fActive_5_Cooltime = 0.f;
		break;
	case 6:
		m_fActive_6_Cooltime = 0.f;
		break;
	}
}

HRESULT CPlayer::Set_MasterCube(CMasterCube * MasterCube)
{
	if (nullptr == MasterCube)
		return E_FAIL;

	m_pMasterCube = MasterCube;
	m_pMasterCube->Set_BindTransform(m_pTransformCom);

	return S_OK;
}


//HRESULT CPlayer::Set_Terrain(MyTerrain * Terrain)
//{
//	if (nullptr == Terrain)
//		return E_FAIL;
//
//	m_MyTerrain = Terrain;
//	Safe_AddRef(m_MyTerrain);
//
//	return S_OK;
//}

HRESULT CPlayer::Set_HPBar(CUI_HPBar * HP_Bar)
{
	if (nullptr == HP_Bar)
		return E_FAIL;

	m_pHPBar = HP_Bar;

	return S_OK;
}

void CPlayer::Get_TurnVector(D3DXVECTOR3 vAxis, _float3 &vRight, _float3 &vUp, _float3 &vLook, float fAngle)
{
	_float4x4	RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, D3DXToRadian(fAngle));

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);
}

void CPlayer::Emplace_ActiveMap(WEAPONLIST _WeaponType)
{
	_bool bFind = false;

	for (auto pair : m_Active)
	{
		if (_WeaponType == pair.first)
		{
			m_Active[_WeaponType] += 1;
			bFind = true;
			break;
		}
	}

	if (!bFind)
	{
		m_Active.emplace(_WeaponType, 1);
		m_pInventory->PushBack_WeaponIcon(_WeaponType);
	}
}

void CPlayer::CreateWeapon(int iActiveSlot, WEAPONLIST eWeapon, int iLevel)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	float fCoolTime = 0.f;
	ACTIVE_RECIEVE_DATA ActiveData;
	ZeroMemory(&ActiveData, sizeof(ACTIVE_RECIEVE_DATA));

	ActiveData.fPower = m_fPower;
	ActiveData.fArea = m_fArea;
	ActiveData.fDuration = m_fDuration;
	ActiveData.fSpeed = m_fSpeed;

	ActiveData.iLevel = iLevel;
	D3DXVec3Normalize(&ActiveData.vRight, &m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
	ActiveData.vUp = D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXVec3Normalize(&ActiveData.vLook, &m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	ActiveData.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	ActiveData.vLook *= -1.f;

	int ShotAmount = 0;
	float fAngle = 0.f;
	int tA;

	switch (eWeapon)
	{
	case PLASMA:

		fCoolTime = 2.f;

		if (iLevel < 4)
		{
			ShotAmount = 1 + m_iAmount;
		}
		else if (iLevel >= 4 && iLevel < 8)
		{
			ShotAmount = 2 + m_iAmount;
		}
		else if (iLevel >= 8 && iLevel < 9)
		{
			ShotAmount = 3 + m_iAmount;
		}
		else if (iLevel == 9)
		{
			ShotAmount = 6 + m_iAmount;

			float fAngle = 360.f / (float)ShotAmount;

			for (int tA = 0; tA < ShotAmount; ++tA)
			{
				if (tA % 2 == 0)
					ActiveData.iEffect = 1;
				else
					ActiveData.iEffect = 0;

				if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Active"), TEXT("Prototype_Effect_Plasma"), &ActiveData)))
					return;

				Get_TurnVector(D3DXVECTOR3(0.f, 1.f, 0.f), ActiveData.vRight, ActiveData.vUp, ActiveData.vLook, fAngle);
			}
			break;
		}

		if (ShotAmount != 1)
		{
			Get_TurnVector(D3DXVECTOR3(0.f, 1.f, 0.f), ActiveData.vRight, ActiveData.vUp, ActiveData.vLook, 16.f * (ShotAmount / 2));
		}
		if (ShotAmount % 2 == 0)
		{
			Get_TurnVector(D3DXVECTOR3(0.f, 1.f, 0.f), ActiveData.vRight, ActiveData.vUp, ActiveData.vLook, -8.f);
		}
		for (int tA = 0; tA < ShotAmount; ++tA)
		{
			if (ShotAmount == 1)
			{
				if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Active"), TEXT("Prototype_Effect_Plasma"), &ActiveData)))
					return;
				fCoolTime = 1.f;
			}
			else
			{
				if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Active"), TEXT("Prototype_Effect_Plasma"), &ActiveData)))
					return;

				Get_TurnVector(D3DXVECTOR3(0.f, 1.f, 0.f), ActiveData.vRight, ActiveData.vUp, ActiveData.vLook, -16.f);
			}
		}

		break;

	case BIBLE:

		fCoolTime = 6.f;
		if (iLevel >= 4)
			fCoolTime += 0.5f;
		if (iLevel >= 7)
			fCoolTime += 0.5f;

		for (int ti = 0; ti < 15; ti++)
		{
			pGameInstance->Delete_LayerObject(LEVEL_GAMEPLAY, TEXT("Layer_Bible"));
		}

		if (iLevel < 2)
		{
			ShotAmount = 1 + m_iAmount;
		}
		else if (iLevel >= 2 && iLevel < 5)
		{
			ShotAmount = 2 + m_iAmount;
		}
		else if (iLevel >= 5 && iLevel < 8)
		{
			ShotAmount = 3 + m_iAmount;
		}
		else if (iLevel >= 8 && iLevel < 9)
		{
			ShotAmount = 4 + m_iAmount;
		}
		else if (iLevel >= 9)
		{
			ShotAmount = 6 + m_iAmount;

			ActiveData.iEffect = 1;

			fAngle = 360.f / (float)ShotAmount;
			for (int tA = 0; tA < ShotAmount; ++tA)
			{
				if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Bible"), TEXT("Prototype_Effect_Bible"), &ActiveData)))
					return;

				Get_TurnVector(D3DXVECTOR3(0.f, 1.f, 0.f), ActiveData.vRight, ActiveData.vUp, ActiveData.vLook, fAngle);
			}

			fCoolTime = 10000.f;
			break;
		}

		fAngle = 360.f / (float)ShotAmount;

		for (int tA = 0; tA < ShotAmount; ++tA)
		{
			if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Active"), TEXT("Prototype_Effect_Bible"), &ActiveData)))
				return;

			Get_TurnVector(D3DXVECTOR3(0.f, 1.f, 0.f), ActiveData.vRight, ActiveData.vUp, ActiveData.vLook, fAngle);
		}
		break;

	case GARLIC:
		pGameInstance->Delete_LayerObject(LEVEL_GAMEPLAY, TEXT("Layer_Garlic"));
		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Garlic"), TEXT("Prototype_Effect_Garlic"), &ActiveData)))
			return;

		fCoolTime = 10000.f;
		break;

	case METEO:

		break;

	case LIGHTNING:
		ActiveData.vPos.y -= 0.3f;

		if (iLevel == 9)
		{
			if (m_fFor_Lightning_Angle > 45.f)
				m_fFor_Lightning_Angle = 0.f;

			if (m_fFor_Lightning_Angle == 0.f)
			{
				m_fFor_Lightning_Pos = ActiveData.vPos;
				m_fFor_Lightning_Look = ActiveData.vLook;
				m_fFor_Lightning_Right = ActiveData.vRight;
			}
			else
			{
				ActiveData.vPos = m_fFor_Lightning_Pos;
				ActiveData.vLook = m_fFor_Lightning_Look;
				ActiveData.vRight = m_fFor_Lightning_Right;
			}

			ActiveData.vPos += ActiveData.vLook * 2.f * (m_fFor_Lightning_Angle + 1);

			if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Active"), TEXT("Prototype_Effect_Lightning"), &ActiveData)))
				return;

			ActiveData.vPos += ActiveData.vRight * 2.f;
			if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Active"), TEXT("Prototype_Effect_Lightning"), &ActiveData)))
				return;
			ActiveData.vPos += ActiveData.vRight * 2.f;
			if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Active"), TEXT("Prototype_Effect_Lightning"), &ActiveData)))
				return;
			ActiveData.vPos -= ActiveData.vRight * 6.f;
			if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Active"), TEXT("Prototype_Effect_Lightning"), &ActiveData)))
				return;
			ActiveData.vPos -= ActiveData.vRight * 2.f;
			if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Active"), TEXT("Prototype_Effect_Lightning"), &ActiveData)))
				return;


			if (m_fFor_Lightning_Angle < 4.f + (float)m_iAmount)
			{
				m_fFor_Lightning_Angle++;
				fCoolTime = 0.1f;
			}
			else
			{
				m_fFor_Lightning_Angle = 0.f;
				fCoolTime = 5.f;
			}

			break;
		}

		Get_TurnVector(D3DXVECTOR3(0.f, 1.f, 0.f), ActiveData.vRight, ActiveData.vUp, ActiveData.vLook, m_fFor_Lightning_Angle);

		tA = (int)m_fFor_Lightning_Angle / 360 + 1;
		ActiveData.vPos += ActiveData.vLook * 3.f * tA;

		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Active"), TEXT("Prototype_Effect_Lightning"), &ActiveData)))
			return;

		m_fFor_Lightning_Angle += 45.f;

		if (m_fFor_Lightning_Angle >= 360.f)
			m_fFor_Lightning_Angle = 0.f;

		if (iLevel < 3)
			fCoolTime = 0.5f;
		if (iLevel >= 3 && iLevel < 5)
			fCoolTime = 0.4f;
		if (iLevel >= 5 && iLevel < 8)
			fCoolTime = 0.3f;
		if (iLevel >= 8 && iLevel < 9)
			fCoolTime = 0.2f;
		break;

	case FIRE_BALL:

		if (iLevel < 3)
			ShotAmount = 1 + m_iAmount;
		else if(iLevel >= 3 && iLevel < 5)
			ShotAmount = 2 + m_iAmount;
		else if (iLevel >= 5 && iLevel < 7)
			ShotAmount = 3 + m_iAmount;
		else if (iLevel >= 7 )
			ShotAmount = 4 + m_iAmount;

		if (iLevel == 9)
		{
			for (int tA = 0; tA < ShotAmount; ++tA)
			{
				ActiveData.vPos += ActiveData.vLook * 4.f;

				ActiveData.iEffect = 1;

				if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Active"), TEXT("Prototype_Effect_FireBall"), &ActiveData)))
					return;

				if (tA >= 4)
					Get_TurnVector(D3DXVECTOR3(0.f, 1.f, 0.f), ActiveData.vRight, ActiveData.vUp, ActiveData.vLook, 45.f);

				Get_TurnVector(D3DXVECTOR3(0.f, 1.f, 0.f), ActiveData.vRight, ActiveData.vUp, ActiveData.vLook, 90.f);


				ActiveData.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			}
		}
		else
		{
			for (int tA = 0; tA < ShotAmount; ++tA)
			{
				ActiveData.vPos += ActiveData.vLook;

				if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Active"), TEXT("Prototype_Effect_FireBall"), &ActiveData)))
					return;

				if (tA >= 4)
					Get_TurnVector(D3DXVECTOR3(0.f, 1.f, 0.f), ActiveData.vRight, ActiveData.vUp, ActiveData.vLook, 45.f);

				Get_TurnVector(D3DXVECTOR3(0.f, 1.f, 0.f), ActiveData.vRight, ActiveData.vUp, ActiveData.vLook, 90.f);


				ActiveData.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			}
		}


		fCoolTime = 4.f;

		break;

	case CAT:
		fCoolTime = 12.f;

		if (iLevel < 3)
			ShotAmount = 1 + m_iAmount;
		else if (iLevel >= 3 && iLevel < 6)
			ShotAmount = 2 + m_iAmount;
		else if (iLevel >= 6 && iLevel < 9)
			ShotAmount = 3 + m_iAmount;
		else if (iLevel >= 9)
		{
			ActiveData.iEffect = 1;
			ShotAmount = 1;
		}

		

		fAngle = 360.f / (float)ShotAmount;

		for (int tA = 0; tA < ShotAmount; ++tA)
		{
			ActiveData.vPos += ActiveData.vLook*2.f;

			if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Active"), TEXT("Prototype_Effect_Cat"), &ActiveData)))
				return;

			Get_TurnVector(D3DXVECTOR3(0.f, 1.f, 0.f), ActiveData.vRight, ActiveData.vUp, ActiveData.vLook, fAngle);
			ActiveData.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		}

		
		break;
	}



	fCoolTime *= m_fCoolTime;
	
	switch (iActiveSlot)
	{
	case 1:
		m_fActive_1_Cooltime = fCoolTime;
		break;
	case 2:
		m_fActive_2_Cooltime = fCoolTime;
		break;
	case 3:
		m_fActive_3_Cooltime = fCoolTime;
		break;
	case 4:
		m_fActive_4_Cooltime = fCoolTime;
		break;
	case 5:
		m_fActive_5_Cooltime = fCoolTime;
		break;
	case 6:
		m_fActive_6_Cooltime = fCoolTime;
		break;
	}

	Safe_Release(pGameInstance);
}

_float3 CPlayer::Get_Position()
{
	return m_pTransformCom->Get_State(CTransform::STATE_POSITION);
}

_float3 CPlayer::Get_Look()
{
	return m_pTransformCom->Get_State(CTransform::STATE_LOOK);
}


HRESULT CPlayer::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 2.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;
	
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;	

	/* For.Com_Renderer `*/
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// CH 추가
	/* For.Com_Collider */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, _float3(0.7f, 0.7f, 0.7f))))
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

	BattleData.iAtk = 0;
	BattleData.iDeffence = 0;
	BattleData.iMaxHP = 43;
	BattleData.iHP = 43;

	/* For.Com_Battle */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Battle"), TEXT("Com_Battle"), (CComponent**)&m_pBattleCom, &BattleData)))
	{
		MSGBOX(L"Failed : Add_Components Com_Collision");
		return E_FAIL;
	}

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Rect"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); // CH 추가

	/*m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);*/


	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	

	return S_OK;
}

HRESULT CPlayer::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); // CH 추가

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	/*m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);*/

	return S_OK;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer*	pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer*	pInstance = new CPlayer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CPlayer::Free()
{
	__super::Free();

	m_Active.clear();
	m_Passive.clear();

	Safe_Release(m_pCollisionCom);
	Safe_Release(m_pColliderCom);

	Safe_Release(m_pHPBar);

	Safe_Release(m_pMasterCube);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pBattleCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}
