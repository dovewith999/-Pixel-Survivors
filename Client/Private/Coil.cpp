#include "stdafx.h"
#include "..\Public\Coil.h"
#include "GameInstance.h"

#include "Player.h"
#include "../Public/MyTerrain.h"

CCoil::CCoil(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster(pGraphic_Device)
{

}

CCoil::CCoil(const CCoil & rhs)
	: CMonster(rhs)
{

}

HRESULT CCoil::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT CCoil::NativeConstruct(void* pArg)
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
		_float3 vPosition = (*(TEXMONDATA*)pArg).vPos;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		m_pTransformCom->TurnAngle(D3DXVECTOR3(0.f, 1.f, 0.f), 180.f);
	}

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	Safe_AddRef(pGameInstance);

	m_pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player"));



	//몸체 큐브 생성
	TCHAR* strPath = L"../../Data/Coil.dat";
	HANDLE		hFile = CreateFile(strPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD		dwByte = 0;
	D3DXVECTOR3 MasterCubeSize;
	D3DXVECTOR3 AllCubeSize;
	D3DXVECTOR3 AllVertexPos;
	int			CubeCount = 0;

	ReadFile(hFile, &MasterCubeSize, sizeof(D3DXVECTOR3), &dwByte, nullptr);
	ReadFile(hFile, &AllCubeSize, sizeof(D3DXVECTOR3), &dwByte, nullptr);
	ReadFile(hFile, &AllVertexPos, sizeof(D3DXVECTOR3), &dwByte, nullptr);
	ReadFile(hFile, &CubeCount, sizeof(int), &dwByte, nullptr);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_MasterCube", L"Prototype_GameObject_MasterCube", MasterCubeSize)))
		return E_FAIL;

	CMasterCube* pMasterCube = dynamic_cast<CMasterCube*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_MasterCube"));
	Set_MasterCube(pMasterCube);

	CUBE eCube;
	int iIndex;

	for (int ti = 0; ti < CubeCount; ++ti)
	{
		ReadFile(hFile, &eCube, sizeof(CUBE), &dwByte, nullptr);
		ReadFile(hFile, &iIndex, sizeof(int), &dwByte, nullptr);

		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Cube", L"Prototype_GameObject_Cube", &eCube)))
			return E_FAIL;

		CCube* pCube = dynamic_cast<CCube*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Cube"));
		pMasterCube->Add_Cube(iIndex, eCube.vSize, pCube);
	}

	pMasterCube->Set_VertexPos(AllVertexPos);
	pMasterCube->Set_CubeSize(D3DXVECTOR3(1.f, 0.7f, 0.7f));

	//플레이어 HPBar 관련 내용 추가!!

	_float2 HP_Pos(0.f, 0.3f);
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_UI_EnemyHPBar", TEXT("Prototype_UIObject_EnemyHPBar"), &HP_Pos.y)))
		return E_FAIL;

	m_pHPBar = dynamic_cast<CUI_Enemy_HPBar*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_UI_EnemyHPBar"));
	Set_HPBar(m_pHPBar);
	Set_BindingTransform_HPBar();

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_UI_EnemyHPBar", TEXT("Prototype_UIObject_EnemyHPElement"))))
		return E_FAIL;

	m_pHPBar->Add_Element(dynamic_cast<CUI_Enemy_HPElement*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_UI_EnemyHPBar")));

	CloseHandle(hFile);

	Safe_Release(pGameInstance);

	return S_OK;
}

void CCoil::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	

	//공격상태에 따른 모션 설정
	if (m_eAtkMotion == ATKMOTION_END)
		return;
	else if (m_eAtkMotion == ATKMOTION_1)
		AttackMotion1();
}

void CCoil::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	__super::Late_Tick(fTimeDelta);

	m_iTerrainIndex = m_pTerrain->Get_TerrainRange(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_iTerrainIndexX, m_iTerrainIndexY);
	__super::SetUp_OnTerrain(m_pTransformCom, LEVEL_GAMEPLAY, TEXT("Layer_Terrain"));

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHABLEND, this);

	m_pColliderCom->Set_Position(m_pTransformCom->Get_State(CTransform::STATE_POSITION));


	m_pCollisionCom->Add_Collisions(m_iTerrainIndex, L"Monster", this);
	if (m_iTerrainIndexX != -1)
		m_pCollisionCom->Add_Collisions(m_iTerrainIndexX, L"Monster", this);
	if (m_iTerrainIndexY != -1)
		m_pCollisionCom->Add_Collisions(m_iTerrainIndexY, L"Monster", this);

	m_MasterCube->Set_Positon(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_pTransformCom->TurnAngle(D3DXVECTOR3(0.f, 1.f, 0.f), 180.f);

	__super::Affected_By_Gravity(m_pTransformCom, fTimeDelta);
}

HRESULT CCoil::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pColliderCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	m_pTransformCom->TurnAngle(D3DXVECTOR3(0.f, 1.f, 0.f), 180.f);

	return S_OK;
}

HRESULT CCoil::SetUp_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 3.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Collision */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Collision"), TEXT("Com_Collision"), (CComponent**)&m_pCollisionCom)))
	{
		MSGBOX(L"Failed : Add_Components Com_Collision");
		return E_FAIL;
	}

	/* For.Com_Collider */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, _float3(0.7f, 0.7f, 0.7f))))
	{
		MSGBOX(L"Failed : Add_Components Prototype_Component_Collider");
		return E_FAIL;
	}

	CBattle::BATTLEDATA		BattleData;
	ZeroMemory(&BattleData, sizeof(CBattle::BATTLEDATA));

	BattleData.iAtk = 2;
	BattleData.iDeffence = 0;
	BattleData.iMaxHP = 10;
	BattleData.iHP = 10;
	BattleData.fAttack_Delay = 180;

	/* For.Com_Battle */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Battle"), TEXT("Com_Battle"), (CComponent**)&m_pBattleCom, &BattleData)))
	{
		MSGBOX(L"Failed : Add_Components Com_Collision");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CCoil::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return S_OK;
}

HRESULT CCoil::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

CCoil * CCoil::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCoil*	pInstance = new CCoil(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCoil::Clone(void * pArg)
{
	CCoil*	pInstance = new CCoil(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCoil::Free()
{
	__super::Free();

	
	if (m_bDead == true)
	{
		m_MasterCube->Set_Dead();
	}
}
