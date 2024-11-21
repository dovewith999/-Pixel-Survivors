#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"
#include "Camera_Free.h"

#include <ctime>

#include "Item_EXP.h"
#include "Item_Object.h"
#include "Item_HP.h"
#include "Item_Vacuum.h"
#include "Item_Timer.h"

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_GamePlay::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;


	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"), CAMERA_PLAYER)))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Spawner(TEXT("Layer_Spawner"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI_Inventory(TEXT("Layer_UI_Inventory"))))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_UI_WeaponIcon(TEXT("Layer_UI_WeaponIcon"))))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_UI_HPBar(TEXT("Layer_UI_HPBar"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI_ExperienceBar(TEXT("Layer_UI_ExperienceBar"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI_ExperiencePoint(TEXT("Layer_UI_ExperiencePoint"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI_PlayTimer(TEXT("Layer_UI_PlayTimer"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI_Cursor(TEXT("Layer_UI_Cursor"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI_PlayerStats(TEXT("Layer_UI_PlayerStats"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI_LevelUp(TEXT("Layer_UI_LevelUp"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI_SelectItem(TEXT("Layer_UI_SelectItem"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Test(TEXT("Layer_Test"))))// 0511
		return E_FAIL;


	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	//SetWindowText(g_hWnd, TEXT("게임플레이레벨임."));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag, CAMERA eCamera)
{

	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 10.f, -7.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.fFovy = D3DXToRadian(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.02f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.fSpeedPerSec = 5.0f;
	CameraDesc.TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);


	switch (eCamera)
	{
	case CAMERA_PLAYER:
		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_Player"), &CameraDesc)))
			return E_FAIL;
		m_pCameraPlayer = dynamic_cast<CCamera_Player*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, pLayerTag));
		break;
	case CAMERA_FREE:
		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_Free"), &CameraDesc)))
			return E_FAIL;
		m_pCameraPlayer = dynamic_cast<CCamera_Player*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, pLayerTag));
		break;
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	m_pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player"));

	m_pCameraPlayer->Set_Player(m_pPlayer);

	// 0505
	m_pPlayer->Set_Terrain(dynamic_cast<CTerrain*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Terrain")));

	//모습이 될 큐브 생성

	TCHAR* strPath = L"../../Data/NyanCat_Rainbow.dat";
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

	cout << "큐브 숫자 : " << CubeCount << endl;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_MasterCube", L"Prototype_GameObject_MasterCube", MasterCubeSize)))
		return E_FAIL;

	m_pMasterCube = dynamic_cast<CMasterCube*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_MasterCube"));
	dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player"))->Set_MasterCube(m_pMasterCube);
	//m_mapCube.emplace(strName, m_pMasterCube);

	CUBE eCube;
	int iIndex;

	for (int ti = 0; ti < CubeCount; ++ti)
	{
		ReadFile(hFile, &eCube, sizeof(CUBE), &dwByte, nullptr);
		ReadFile(hFile, &iIndex, sizeof(int), &dwByte, nullptr);

		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Cube", L"Prototype_GameObject_Cube", &eCube)))
			return E_FAIL;

		m_pCube = dynamic_cast<CCube*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Cube"));
		m_pMasterCube->Add_Cube(iIndex, eCube.vSize, m_pCube);
	}
	m_pMasterCube->Set_VertexPos(AllVertexPos);
	m_pMasterCube->Set_CubeSize(D3DXVECTOR3(1.f, 0.5f, 0.25f));

	m_pMasterCube->Set_CubeTurnAngle(D3DXVECTOR3(0.f, 1.f, 0.f), 180.f);


	CloseHandle(hFile);

	Safe_Release(pGameInstance);

	return S_OK;
}

// CH 추가
HRESULT CLevel_GamePlay::Ready_Layer_Spawner(const _tchar * pLayerTag)
{

	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	_float3 vPosition{ 5.f, 5.f, 5.f };

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Spawner"), &vPosition)))
		return E_FAIL;
	CSpawner* pSpawner = dynamic_cast<CSpawner*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Spawner"));
	pSpawner->Set_Terrain(m_pTerrain);

	vPosition = { 30.f, 5.f, 5.f };

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Spawner"), &vPosition)))
		return E_FAIL;
	pSpawner = dynamic_cast<CSpawner*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Spawner"));
	pSpawner->Set_Terrain(m_pTerrain);

	vPosition = { 55.f, 5.f, 5.f };

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Spawner"), &vPosition)))
		return E_FAIL;
	pSpawner = dynamic_cast<CSpawner*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Spawner"));
	pSpawner->Set_Terrain(m_pTerrain);

	vPosition = { 55.f, 5.f, 30.f };

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Spawner"), &vPosition)))
		return E_FAIL;
	pSpawner = dynamic_cast<CSpawner*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Spawner"));
	pSpawner->Set_Terrain(m_pTerrain);

	vPosition = { 55.f, 5.f, 55.f };

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Spawner"), &vPosition)))
		return E_FAIL;
	pSpawner = dynamic_cast<CSpawner*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Spawner"));
	pSpawner->Set_Terrain(m_pTerrain);

	vPosition = { 30.f, 5.f, 55.f };

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Spawner"), &vPosition)))
		return E_FAIL;
	pSpawner = dynamic_cast<CSpawner*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Spawner"));
	pSpawner->Set_Terrain(m_pTerrain);

	vPosition = { 5.f, 5.f, 55.f };

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Spawner"), &vPosition)))
		return E_FAIL;
	pSpawner = dynamic_cast<CSpawner*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Spawner"));
	pSpawner->Set_Terrain(m_pTerrain);

	vPosition = { 5.f, 5.f, 30.f };

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Spawner"), &vPosition)))
		return E_FAIL;
	pSpawner = dynamic_cast<CSpawner*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Spawner"));
	pSpawner->Set_Terrain(m_pTerrain);

	Safe_Release(pGameInstance);

	return S_OK;
}

// 0507
HRESULT CLevel_GamePlay::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	const TCHAR*      pGetPath = L"../../Data/Terrain/FTerrain.dat";

	HANDLE      hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	DWORD	dwStrByte = 0;

	TERRAIN		tTerrain{};
	TERRAINCUBE	tTerrainCube{};

	_uint iTerrainCX = 0;
	_uint iTerrainCZ = 0;

	ReadFile(hFile, &iTerrainCX, sizeof(int), &dwByte, nullptr);
	ReadFile(hFile, &iTerrainCZ, sizeof(int), &dwByte, nullptr);

	int iTerrainSize = iTerrainCX * iTerrainCZ;

	// 지면 로드 ===============================================================================================
	ReadFile(hFile, &tTerrain, sizeof(TERRAIN), &dwByte, nullptr);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Terrain", L"Prototype_GameObject_Terrain", &tTerrain)))
		return E_FAIL;

	m_pTerrain = dynamic_cast<CTerrain*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Terrain"));

	TERRAINWALL tWall{};

	// Wall 로드 ===============================================================================================
	for (int i = 0; i < 4; ++i)
	{
		ReadFile(hFile, &tWall, sizeof(TERRAINWALL), &dwByte, nullptr);

		WALL tTerrain{ (WALL::ROLE)i, tWall.vSize, tWall.vBasePos, tWall.iDrawID };

		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Wall", L"Prototype_GameObject_Wall", &tTerrain)))
			return E_FAIL;

		CWall* pWall = dynamic_cast<CWall*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Wall"));

		//pWall->Set_Role((CWall::ROLE)i);
		pWall->Rotation(tWall.vAxis, D3DXToRadian(tWall.fAngle));
	}

	// HeightMap 로드 ===============================================================================================
	int iHeight = 0;

	m_pTerrain->m_iHeights = new int[iTerrainCX * iTerrainCZ];

	for (int i = 0; i < iTerrainCX * iTerrainCZ; ++i)
	{
		ReadFile(hFile, &iHeight, sizeof(int), &dwByte, nullptr);
		m_pTerrain->m_iHeights[i] = iHeight;
	}

	CloseHandle(hFile);

	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Layer_UI_WeaponIcon(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_WeaponIcon"), &_float2(390.f, 660.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_WeaponIcon"), &_float2(490.f, 660.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_WeaponIcon"), &_float2(590.f, 660.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_WeaponIcon"), &_float2(690.f, 660.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_WeaponIcon"), &_float2(790.f, 660.f))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_WeaponIcon"), &_float2(890.f, 660.f))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI_ExperienceBar(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_ExperienceBar"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI_ExperiencePoint(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_ExperiencePoint"))))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI_SelectItem(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_SelectItem"), _float2(640.f, 250.f))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_SelectItem"), _float2(640.f, 380.f))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_SelectItem"), _float2(640.f, 510.f))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI_PlayTimer(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_PlayTimer"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI_Cursor(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_Cursor"), _float2(420.f, 250.f))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_Cursor"), _float2(860.f, 250.f))))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI_PlayerStats(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);


	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_PlayerStats"))))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI_LevelUp(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_LevelUp"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI_Inventory(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_Inventory"), &_float2(100.f, 65.f))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI_HPBar(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	_float2 HP_Pos(1200.f, 500.f);
	//플레이어 HPBar 관련 내용 추가!!
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_HPBar"), &HP_Pos)))
		return E_FAIL;

	m_pHPBar = dynamic_cast<CUI_HPBar*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_UI_HPBar"));
	m_pPlayer->Set_HPBar(m_pHPBar);

	for (int ti = 0; ti < 43; ti++)
	{
		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_UIObject_HPElement"), &_float2(HP_Pos.x + 24.f, HP_Pos.y + 25.f - (5.f * ti)))))
			return E_FAIL;

		m_pHPBar->Add_Element(dynamic_cast<CUI_HPElement*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_UI_HPBar")));
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

// 0511
HRESULT CLevel_GamePlay::Ready_Layer_Test(const _tchar * pLayerTag)
{
	CGameInstance*	pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

#pragma region Item_Exp
	ITEM tItem{};
	int j = 0;
	for (int i = 0; i < 20; ++i, ++j)
	{
		tItem = { _float3(i + 10.f, 0.5f, j + 15.f), 0 };
		if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Item_EXP", L"Prototype_GameObject_Item_EXP", &tItem)))
			return E_FAIL;

		dynamic_cast<CItem_EXP*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_EXP"))->Set_Terrain(m_pTerrain);
		dynamic_cast<CItem_EXP*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_EXP"))->Set_Player(dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player")));

	}

	tItem = { _float3(13.f, 0.5f, 15.f), 1 };
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Item_EXP", L"Prototype_GameObject_Item_EXP", &tItem)))
		return E_FAIL;

	dynamic_cast<CItem_EXP*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_EXP"))->Set_Terrain(m_pTerrain);
	dynamic_cast<CItem_EXP*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_EXP"))->Set_Player(dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player")));

	tItem = { _float3(15.f, 0.5f, 15.f), 2 };
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Item_EXP", L"Prototype_GameObject_Item_EXP", &tItem)))
		return E_FAIL;

	dynamic_cast<CItem_EXP*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_EXP"))->Set_Terrain(m_pTerrain);
	dynamic_cast<CItem_EXP*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_EXP"))->Set_Player(dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player")));
#pragma endregion

#pragma region Item_Object
	tItem = { _float3(15.f, 0.5f, 10.f), 0 };
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Item_Object", L"Prototype_GameObject_Item_Object", &tItem)))
		return E_FAIL;

	dynamic_cast<CItem_Object*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Object"))->Set_Terrain(m_pTerrain);
	dynamic_cast<CItem_Object*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Object"))->Set_Player(dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player")));

	tItem = { _float3(17.f, 0.5f, 10.f), 1 };
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Item_Object", L"Prototype_GameObject_Item_Object", &tItem)))
		return E_FAIL;

	dynamic_cast<CItem_Object*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Object"))->Set_Terrain(m_pTerrain);
	dynamic_cast<CItem_Object*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Object"))->Set_Player(dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player")));

	tItem = { _float3(19.f, 0.5f, 10.f), 3 };
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Item_Object", L"Prototype_GameObject_Item_Object", &tItem)))
		return E_FAIL;

	dynamic_cast<CItem_Object*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Object"))->Set_Terrain(m_pTerrain);
	dynamic_cast<CItem_Object*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Object"))->Set_Player(dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player")));

	tItem = { _float3(20.f, 0.5f, 10.f), 2 };
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Item_Object", L"Prototype_GameObject_Item_Object", &tItem)))
		return E_FAIL;

	dynamic_cast<CItem_Object*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Object"))->Set_Terrain(m_pTerrain);
	dynamic_cast<CItem_Object*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Object"))->Set_Player(dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player")));

#pragma endregion

#pragma region Item_HP
	tItem = { _float3(15.f, 0.5f, 5.f), 0 };
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Item_HP", L"Prototype_GameObject_Item_HP", &tItem)))
		return E_FAIL;

	dynamic_cast<CItem_HP*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_HP"))->Set_Terrain(m_pTerrain);
	dynamic_cast<CItem_HP*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_HP"))->Set_Player(dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player")));
#pragma endregion

#pragma region Item_Vacuum
	ITEM tItem2 = { _float3(13.f, 0.5f, 5.f), 0 };
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Item_Vacuum", L"Prototype_GameObject_Item_Vacuum", &tItem2)))
		return E_FAIL;

	dynamic_cast<CItem_Vacuum*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Vacuum"))->Set_Terrain(m_pTerrain);
	dynamic_cast<CItem_Vacuum*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Vacuum"))->Set_Player(dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player")));
#pragma endregion

#pragma region Item_Timer

	tItem = { _float3(30.f, 0.5f, 40.f), 0 };
	if (FAILED(pGameInstance->Add_Layer(LEVEL_GAMEPLAY, L"Layer_Item_Timer", L"Prototype_GameObject_Item_Timer", &tItem)))
		return E_FAIL;

	dynamic_cast<CItem_Timer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Timer"))->Set_Terrain(m_pTerrain);
	dynamic_cast<CItem_Timer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Item_Timer"))->Set_Player(dynamic_cast<CPlayer*>(pGameInstance->Find_LayerObject(LEVEL_GAMEPLAY, L"Layer_Player")));
#pragma endregion

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();

	Safe_Release(m_pTerrain);
	Safe_Release(m_pCube);
}
