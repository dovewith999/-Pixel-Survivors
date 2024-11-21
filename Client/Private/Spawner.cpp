#include "stdafx.h"
#include "..\Public\Spawner.h"
#include "..\Public\MonsterLayer.h"
#include "GameInstance.h"
#include "Monster.h"
#include "UI_Enemy_HPBar.h"
#include "UI_Enemy_HPElement.h"

#include "MasterCube.h"
#include "Cube.h"

#include "Item_Object.h"

#include <ctime>
#include <random>
#include <functional>

CSpawner::CSpawner(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CSpawner::CSpawner(const CSpawner & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSpawner::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT CSpawner::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(SetUp_Waves()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		_float3 vPosition = *(_float3*)pArg;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

		m_tRect.left = LONG(vPosition.x - SPAWN_RANGE);
		m_tRect.top = LONG(vPosition.z + SPAWN_RANGE);
		m_tRect.right = LONG(vPosition.x + SPAWN_RANGE);
		m_tRect.bottom = LONG(vPosition.z - SPAWN_RANGE);
	}

	return S_OK;
}

HRESULT CSpawner::Set_Terrain(CTerrain * pTerrain)
{
	if (nullptr == pTerrain)
		return E_FAIL;

	m_pTerrain = pTerrain;

	return S_OK;
}

void CSpawner::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return;

	Safe_AddRef(pGameInstance);

	m_fMUpdateTime += TIEM_SPEED * fTimeDelta;
	m_fIUpdateTime += TIEM_SPEED * fTimeDelta;
	m_fAccTime += TIEM_SPEED * fTimeDelta;

	if (true == m_bSpawn)
	{
		if (m_fAccTime >= WAVE_TIME) // Chanege Wage
		{
			//++m_iWaveLevel;
			m_iMonsterIndex = 0;
			m_iMonsterNumber = 0;
			m_fAccTime = 0.f;

			m_iMonsterLimitCnt += 10;

			if (m_iMonsterLimitCnt >= MONSTER_MAX_COUNT)
				m_iMonsterLimitCnt = 100;

			if (WAVE_END == m_iWaveLevel)
				m_bSpawn = false;
		}

		// Spawn Monster
		if (m_fMUpdateTime >= MONSTER_SPAWN_TIME && m_iMonsterLimitCnt >= pGameInstance->Get_LayerCount(LEVEL_GAMEPLAY, L"Layer_Monster_Wave1"))
		{
			Spawn_Monster(m_iWaveLevel, fTimeDelta);
			m_fMUpdateTime = 0.f;
		}

		// Spawn Item
		if (m_fIUpdateTime >= MONSTER_SPAWN_TIME && m_iItemLimitCnt >= pGameInstance->Get_LayerCount(LEVEL_GAMEPLAY, L"Layer_Item_Object"))
		{
			Create_ItemObject();
			m_fIUpdateTime = 0.f;
		}
	}

	Safe_Release(pGameInstance);

}

void CSpawner::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	__super::Late_Tick(fTimeDelta);

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_ALPHABLEND, this);

}

HRESULT CSpawner::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	_float4x4		ViewMatrix;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

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

HRESULT CSpawner::SetUp_Waves()
{
	// TODO : 보스 몬스터?
	m_pWaves = new vector<WAVE>[WAVE_END];

	// WAVE 1
	/*WAVE tagWave = { WAVE_1, L"Prototype_GameObject_Monster_Slime", L"Layer_Monster_Wave1", 1 };
	m_pWaves[WAVE_1].push_back(tagWave);
	tagWave = { WAVE_1, L"Prototype_GameObject_Monster_Star", L"Layer_Monster_Wave1", 2 };
	m_pWaves[WAVE_1].push_back(tagWave);
	tagWave = { WAVE_1, L"Prototype_GameObject_Monster_Coil", L"Layer_Monster_Wave1", 3 };
	m_pWaves[WAVE_1].push_back(tagWave);
	tagWave = { WAVE_1, L"Prototype_GameObject_Monster_NyanCat", L"Layer_Monster_Wave1", 4 };
	m_pWaves[WAVE_1].push_back(tagWave);*/
	
	WAVE tagWave; /*= { WAVE_1, L"Santa", L"Layer_Enemy", 1, MONSTER_END };
	m_pWaves[WAVE_1].push_back(tagWave);
	tagWave = { WAVE_1, L"TEST", L"Layer_Enemy", 1, MONSTER_END };
	m_pWaves[WAVE_1].push_back(tagWave);*/
	tagWave = { WAVE_1, L"Prototype_GameObject_TexMonster", L"Layer_Enemy", 30, MONSTER_BAT };
	m_pWaves[WAVE_1].push_back(tagWave);
	/*tagWave = { WAVE_1, L"Prototype_GameObject_TexMonster", L"Layer_Enemy", 30, MONSTER_SKELETON };
	m_pWaves[WAVE_1].push_back(tagWave);
	tagWave = { WAVE_1, L"Prototype_GameObject_TexMonster", L"Layer_Enemy", 30, MONSTER_LIZARDMAN };
	m_pWaves[WAVE_1].push_back(tagWave);
	tagWave = { WAVE_1, L"Prototype_GameObject_TexMonster", L"Layer_Enemy", 30, MONSTER_WRAITH };
	m_pWaves[WAVE_1].push_back(tagWave);
	tagWave = { WAVE_1, L"Prototype_GameObject_TexMonster", L"Layer_Enemy", 20, MONSTER_DRAGO };
	m_pWaves[WAVE_1].push_back(tagWave);
	tagWave = { WAVE_1, L"Prototype_GameObject_TexMonster", L"Layer_Enemy", 10, MONSTER_UNDEAD };
	m_pWaves[WAVE_1].push_back(tagWave);*/
	//WAVE tagWave = { WAVE_1, L"TEST", L"Layer_Enemy", 30, MONSTER_BAT };
	//m_pWaves[WAVE_1].push_back(tagWave);
	//tagWave = { WAVE_1, L"Prototype_GameObject_TexMonster", L"Layer_Enemy", 60, MONSTER_SKELETON };
	//m_pWaves[WAVE_1].push_back(tagWave);
	//tagWave = { WAVE_1, L"Prototype_GameObject_TexMonster", L"Layer_Enemy", 90, MONSTER_LIZARDMAN };
	//m_pWaves[WAVE_1].push_back(tagWave);
	//tagWave = { WAVE_1, L"Prototype_GameObject_TexMonster", L"Layer_Enemy", 120, MONSTER_WRAITH };
	//m_pWaves[WAVE_1].push_back(tagWave);
	//tagWave = { WAVE_1, L"Prototype_GameObject_TexMonster", L"Layer_Enemy", 150, MONSTER_DRAGO };
	//m_pWaves[WAVE_1].push_back(tagWave);
	//tagWave = { WAVE_1, L"Prototype_GameObject_TexMonster", L"Layer_Enemy", 180, MONSTER_UNDEAD };
	//m_pWaves[WAVE_1].push_back(tagWave);

	/*for (_uint ti = 0; ti < 150; ++ti)
	{
		srand((_uint)time(NULL));
		_uint eTex = rand() % 6;
		WAVE tagWave;
		switch (eTex)
		{
		case 0:
			tagWave = { WAVE_1, L"Prototype_GameObject_TexMonster", L"Layer_Monster_Wave1", ti + 1, MONSTER_BAT };
			break;
		case 1:
			tagWave = { WAVE_1, L"Prototype_GameObject_TexMonster", L"Layer_Monster_Wave1", ti + 1, MONSTER_SKELETON };
			break;
		case 2:
			tagWave = { WAVE_1, L"Prototype_GameObject_TexMonster", L"Layer_Monster_Wave1", ti + 1, MONSTER_LIZARDMAN };
			break;
		case 3:
			tagWave = { WAVE_1, L"Prototype_GameObject_TexMonster", L"Layer_Monster_Wave1", ti + 1, MONSTER_DRAGO };
			break;
		case 4:
			tagWave = { WAVE_1, L"Prototype_GameObject_TexMonster", L"Layer_Monster_Wave1", ti + 1, MONSTER_UNDEAD };
			break;
		case 5:
			tagWave = { WAVE_1, L"Prototype_GameObject_TexMonster", L"Layer_Monster_Wave1", ti + 1, MONSTER_WRAITH };
			break;
		}
		m_pWaves[WAVE_1].push_back(tagWave);
	}*/
	/*WAVE tagWave = { WAVE_1, L"Prototype_GameObject_LiazrdMan", L"Layer_Monster_Wave1", 1 };
	m_pWaves[WAVE_1].push_back(tagWave);*/

	// WAVE 2
	//tagWave = { WAVE_2, L"Prototype_GameObject_Monster_Star", L"Layer_Monster_Star", 0 };
	//m_pWaves[WAVE_2].push_back(tagWave);

	//// WAVE 3
	//tagWave = { WAVE_2, L"Prototype_GameObject_Monster_Curvy", L"Layer_Monster_Curvy", 0 };
	//m_pWaves[WAVE_3].push_back(tagWave);

	return S_OK;
}

void CSpawner::Spawn_Monster(_uint iWaveLevel, _float fTimeDelta)
{
	if (0 == m_pWaves[iWaveLevel][m_iMonsterIndex].iMonsterCount)
		return;

	Create_Monster(m_pWaves[iWaveLevel][m_iMonsterIndex].eLevel,
		m_pWaves[iWaveLevel][m_iMonsterIndex].szMonsterPrototypeTag, 
		m_pWaves[iWaveLevel][m_iMonsterIndex].szMonsterLayerTag,
		m_pWaves[iWaveLevel][m_iMonsterIndex].eTexMonster);

	++m_iMonsterNumber;

	if (m_iMonsterNumber == m_pWaves[iWaveLevel][m_iMonsterIndex].iMonsterCount)
	{
		++m_iMonsterIndex;
		m_iMonsterNumber = 0;
	}

	if (m_iMonsterIndex >= m_pWaves[iWaveLevel].size())
		m_bSpawn = false;
}

HRESULT CSpawner::Create_Monster(WAVELEVEL eWaveLevel, const _tchar* szMonsterPrototypeTag, const _tchar* szMonsterTag, TEXMONSTER eTex)
{
	// TODO : 몬스터 위치 초기화
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	// 임시 랜덤 좌표 몬스터 생성
	srand((_uint)time(NULL));

	// rand() % (마지막 값 - 시작 값 + 1) + 시작 값
	_float fX = _float(rand() % (m_tRect.right - m_tRect.left + 1) + m_tRect.left);
	_float fZ = _float(rand() % (m_tRect.bottom - m_tRect.top + 1) + m_tRect.top);

	TEXMONDATA eData;
	eData.vPos = _float3(fX, 5.f, fZ);
	eData.eTex = eTex;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, szMonsterTag, szMonsterPrototypeTag, &eData)))
		return E_FAIL;

	dynamic_cast<CMonster*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, szMonsterTag))->Set_Terrain(m_pTerrain);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CSpawner::Create_ItemObject()
{
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	std::random_device rd;

	std::mt19937 gen(rd());

	uniform_int_distribution<int> distribution(0, 3);   // 생성 범위
	auto generator = bind(distribution, gen);
	_int iItemNum = generator();

	uniform_int_distribution<int> distribution1(5, 55);
	auto generator1 = bind(distribution1, gen);
	_float fX = generator1();

	uniform_int_distribution<int> distribution2(5, 55);
	auto generator2 = bind(distribution2, gen);
	_float fZ = generator2();
	
	cout << "fX : " << fX << endl;
	cout << "fZ : " << fZ << endl;

	ITEM tItem = { _float3(fX, 0.5f, fZ), iItemNum };
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Item_Object", L"Prototype_GameObject_Item_Object", &tItem)))
		return E_FAIL;

	dynamic_cast<CItem_Object*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Object"))->Set_Terrain(m_pTerrain);
	dynamic_cast<CItem_Object*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Object"))->Set_Player(dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player")));

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CSpawner::SetUp_Components()
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
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Potal"), TEXT("Com_Texture_Potal"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpawner::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


/*	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);*/	

	return S_OK;
}

HRESULT CSpawner::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

CSpawner * CSpawner::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpawner*	pInstance = new CSpawner(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CSpawner"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSpawner::Clone(void * pArg)
{
	CSpawner*	pInstance = new CSpawner(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CSpawner"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CSpawner::Free()
{
	__super::Free();


	Safe_Delete_Array(m_pWaves);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
