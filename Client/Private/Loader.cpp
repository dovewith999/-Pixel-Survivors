#include "stdafx.h"
#include "..\Public\Loader.h"
#include "Sky.h"
#include "Player.h"
#include "BackGround.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Camera_Player.h"
#include "MasterCube.h"
#include "Cube.h"
#include "Inventory.h"
#include "AttackCollision.h"
#include "Battle.h"
#include "Spawner.h"

// Monster
#include "Slime.h"
#include "Star.h"
#include "Curvy.h"
#include "Coil.h"
#include "NyanCat.h"
#include "TexMonster.h"
//Anmation
#include "Test.h"
#include "Santa.h"
#include "AngryBird.h"
#include "Bone_hhlee.h"
//Effect
#include "Plasma.h"
#include "Bible.h"
#include "Garlic.h"
#include "Lightning.h"
#include "FireBall.h"
#include "Cat.h"
#include "Effect0.h"

// UI
#include "UI_Inventory.h"
#include "UI_HPBar.h"
#include "UI_HPElement.h"
#include "UI_Enemy_HPBar.h"
#include "UI_Enemy_HPElement.h"
#include "UI_WeaponIcon.h"
#include "UI_Scope.h"
#include "UI_Sniper.h"
#include "UI_Logo.h"
#include "UI_BackGround.h"
#include "UI_Start.h"
#include "UI_ExperienceBar.h"
#include "UI_ExperiencePoint.h"
#include "UI_LevelUp.h"
#include "UI_SelectItem.h"
#include "UI_PlayTimer.h"
#include "UI_PlayerStats.h"
#include "UI_Cursor.h"
#include "UI_IconBox.h"

#include "Terrain.h"
#include "Wall.h"

// 0511
// Item
#include "Item.h"
#include "Item_EXP.h"
#include "Item_Object.h"
#include "Item_HP.h"
#include "Item_Vacuum.h"
#include "Item_Timer.h"
#include "Item_Treasure.h"
#include "Item_TimerEffect.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

_uint APIENTRY EntryFunction(void* pArg)
{
	CLoader*			pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	_uint		iProgress = 0;

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO:
		iProgress = pLoader->Loading_ForLogo();
		break;
	case LEVEL_GAMEPLAY:
		iProgress = pLoader->Loading_ForGamePlay();
		break;
	}

	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return iProgress;
}

HRESULT CLoader::NativeConstruct(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, EntryFunction, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

_uint CLoader::Loading_ForLogo()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

#pragma region GAMEOBJECT
	lstrcpy(m_szLoadingText, TEXT("객체 원형을 생성중입니다."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pGraphic_Device))))
		return -1;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Logo"),
		CUI_Logo::Create(m_pGraphic_Device))))
		return -1;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_BackGround"),
		CUI_BackGround::Create(m_pGraphic_Device))))
		return -1;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Start"),
		CUI_Start::Create(m_pGraphic_Device))))
		return -1;

#pragma endregion


#pragma region COMPONENT
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));

	/* For.Prototype_Component_Texture_HPBar. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_UI_Logo"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Logo/UI_Logo.png")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_UI_BackGround"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_BackGround.png")))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_UI_Start"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_Start.png")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을 생성중입니다."));

#pragma endregion


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return _uint();
}

_uint CLoader::Loading_ForGamePlay()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

#pragma region GAMEOBJECT
	lstrcpy(m_szLoadingText, TEXT("객체 원형을 생성중입니다."));


	/*For.Prototype_GameObject_Player*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Player"),
		CCamera_Player::Create(m_pGraphic_Device))))
		return -1;



	/*For.Prototype_GameObject_AttackCollision*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AttackCollision"),
		CAttackCollision::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_Cube */
	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_GameObject_Cube", CCube::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_MasterCube */
	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_GameObject_MasterCube", CMasterCube::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_UI_Inventory */
	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_UIObject_Inventory", CUI_Inventory::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_UI_WeaponIcon */
	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_UIObject_WeaponIcon", CUI_WeaponIcon::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_UI_Scope */
	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_UIObject_Scope", CUI_Scope::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_UI_Sniper */
	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_UIObject_Sniper", CUI_Sniper::Create(m_pGraphic_Device))))
		return -1;


	/* For.Prototype_GameObject_UI_HPBar */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_UIObject_HPBar"), CUI_HPBar::Create(m_pGraphic_Device))))
		return -1;

	//AHS 추가
	/* For.Prototype_GameObject_UI_HPElement */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_UIObject_HPElement"), CUI_HPElement::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_UI_HPBar */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_UIObject_EnemyHPBar"), CUI_Enemy_HPBar::Create(m_pGraphic_Device))))
		return -1;

	//AHS 추가
	/* For.Prototype_GameObject_UI_HPElement */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_UIObject_EnemyHPElement"), CUI_Enemy_HPElement::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_UI_ExperienceBar */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_UIObject_ExperienceBar"), CUI_ExperienceBar::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_UI_ExperiencePoint */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_UIObject_ExperiencePoint"), CUI_ExperiencePoint::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_UI_LevelUp */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_UIObject_LevelUp"), CUI_LevelUp::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_UI_SelectItem */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_UIObject_SelectItem"), CUI_SelectItem::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_UI_PlayTimer */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_UIObject_PlayTimer"), CUI_PlayTimer::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_UI_PlayerStats */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_UIObject_PlayerStats"), CUI_PlayerStats::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_UI_Cursor */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_UIObject_Cursor"), CUI_Cursor::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_UI_IconBox */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_UIObject_IconBox"), CUI_IconBox::Create(m_pGraphic_Device))))
		return -1;
	
	//플레이어 무기,액티브 아이템=====================================================================================================
	/* For.Prototype_Effect_Plasma */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Effect_Plasma"), CPlasma::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_Effect_Plasma */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Effect_Bible"), CBible::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_Effect_Garlic */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Effect_Garlic"), CGarlic::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_Effect_Lightning */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Effect_Lightning"), CLightning::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_Effect_FireBall */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Effect_FireBall"), CFireBall::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_Effect_Cat */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Effect_Cat"), CCat::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_Effect_Effect0 */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_Effect_Effect0"), CEffect0::Create(m_pGraphic_Device))))
		return -1;



	//===========================================================몬스터 관련===========================================================

	/*For.Prototype_GameObject_Monster_Default*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Slime"), CSlime::Create(m_pGraphic_Device))))
		return -1;

	/*For.Prototype_GameObject_Monster_Default*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Star"), CStar::Create(m_pGraphic_Device))))
		return -1;

	/*For.Prototype_GameObject_Monster_Default*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Curvy"), CCurvy::Create(m_pGraphic_Device))))
		return -1;

	/*For.Prototype_GameObject_Monster_Default*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Coil"), CCoil::Create(m_pGraphic_Device))))
		return -1;

	/*For.Prototype_GameObject_Monster_Default*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_NyanCat"), CNyanCat::Create(m_pGraphic_Device))))
		return -1;

	/*For.Prototype_GameObject_Spawner*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Spawner"), CSpawner::Create(m_pGraphic_Device))))
		return -1;

	/*For.Prototype_GameObject_TexMonster*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TexMonster"), CTexMonster::Create(m_pGraphic_Device))))
		return -1;

	//==========================================================================================================================

	//// 0505
	///* For.Prototype_GameObject_Tile */
	//if (FAILED(pGameInstance->Add_Prototype(L"Prototype_GameObject_Tile", CTile::Create(m_pGraphic_Device))))
	//	return -1;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_GameObject_Terrain", CTerrain::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_Wall */
	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_GameObject_Wall", CWall::Create(m_pGraphic_Device))))
		return -1;

	///* For.Prototype_GameObject_TerrainCube */
	//if (FAILED(pGameInstance->Add_Prototype(L"Prototype_GameObject_TerrainCube", CTerrainCube::Create(m_pGraphic_Device))))
	//	return -1;

	// 0511
	// 아이템 =======================================================================================================

	/* For.Prototype_GameObject_Item_EXP */
	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_GameObject_Item_EXP", CItem_EXP::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_Item_Object */
	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_GameObject_Item_Object", CItem_Object::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_Item_HP */
	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_GameObject_Item_HP", CItem_HP::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_Item_Vacuum */
	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_GameObject_Item_Vacuum", CItem_Vacuum::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_Item_Timer */
	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_GameObject_Item_Timer", CItem_Timer::Create(m_pGraphic_Device))))
		return -1;


	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_GameObject_Item_Timer_Effect", CItem_TimerEffect::Create(m_pGraphic_Device))))
		return -1;

	/* For.Prototype_GameObject_Item_Treasure */
	if (FAILED(pGameInstance->Add_Prototype(L"Prototype_GameObject_Item_Treasure", CItem_Treasure::Create(m_pGraphic_Device))))
		return -1;

#pragma endregion

#pragma region COMPONENT
	lstrcpy(m_szLoadingText, TEXT("지형을 생성 중입니다. "));

	/* For.Prototype_Component_VIBuffer_Cube. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 0507
	/* For.Prototype_Component_VIBuffer_Terrain. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 0, 0))))
		return E_FAIL;

	/* For.Prototype_Component_Collider. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider"),
		CCollider::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Battle. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Battle"),
		CBattle::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Player. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Player/AKIHA_AKI00_00%d.png"), 12))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_SkyBox. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SkyBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_CUBEMAP, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Inventory. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Inventory"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_Inventory.png")))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_WeaponIcon. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_WeaponIcon"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Image/WeaponIcon/Weapon_%d.png"), 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Scope"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_Scope.png")))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sniper"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_Sniper.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_HPBar. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_HPBar"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Image/Overlays/EnergyBarFrame.png")))))
		return E_FAIL;

	//AHS 추가
	/* For.Prototype_Component_Texture_HPElement. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_HPElement"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Image/Overlays/EnergyBarElement.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_HPBar. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_EnemyHPBar"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Image/Overlays/life_energy_EnemyBlack.png")))))
		return E_FAIL;

	//AHS 추가
	/* For.Prototype_Component_Texture_HPElement. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_EnemyHPElement"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Image/Overlays/life_energy_EnemyRed.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ExperienceBar. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ExperienceBar"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_ExperienceBar.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ExperiencePoint. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ExperiencePoint"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_ExperiencePoint.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_LevelUp. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_LevelUp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_LevelUp.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_SelectItem. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SelectItem"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_SelectItem.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Cursor. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Cursor"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_Cursor_%d.png"), 5))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Cursor_IconBox. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_IconBox"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_IconBox.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_SelectItem. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Cursor_Reverse"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/UI/UI_Cursor_Reverse_%d.png"), 5))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Potal. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Potal"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Monster/Potal.png"), 1))))
		return E_FAIL;

	// 0505
	/* For.Prototype_Component_Texture */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Tile",
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.png"), 52))))
		return E_FAIL;

	//몬스터 텍스쳐======================================================================================================
	/* For.Prototype_Component_Texture_LizardMan. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_LizardMan"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Monster/LizardMan_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Bat. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Bat"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Monster/Bat_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Drago. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Drago"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Monster/Drago_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Skeleton. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Skeleton"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Monster/Skeleton_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Undead. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Undead"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Monster/Undead_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Wraith. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Wraith"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Monster/Wraith_%d.png"), 10))))
		return E_FAIL;


	//이펙트,무기 텍스쳐======================================================================================================
	/* For.Prototype_Component_Texture_Plasma. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Plasma"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Plasma_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Bible. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Bible"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Bible_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Garlic. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Garlic"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Garlic_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Lightning. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Lightning"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Lightning_%d.png"), 3))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_FireBall. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_FireBall"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/FireBall_%d.png"), 14))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Cat. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Cat"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Cat_%d.png"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Effect0. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect0"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Effect/Effect0_%d.png"), 1))))
		return E_FAIL;

	// 0511
	// 아이템 텍스쳐 ======================================================================================================
	/* For.Prototype_Component_Texture_Item_EXP. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item_EXP"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Item/EXP/EXP_%d.png"), 3))))
		return E_FAIL;
	//저기

	/* For.Prototype_Component_Texture_Item_CandleStick. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item_CandleStick"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Item/CandleStick/CandleStick_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Item_HP. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item_HP"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Item/Hp/Hp_%d.png"), 1))))
		return E_FAIL;
	//여기
	/* For.Prototype_Component_Texture_Item_Blue_Brazier. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item_Blue_Brazier"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Item/Brazier/Brazier_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Item_Red_Brazier. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item_Red_Brazier"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Item/Brazier_Red/Brazier_%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Item_StreetLamp. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item_StreetLamp"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Item/StreetLamp/StreetLamp_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Item_Vacuum. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item_Vacuum"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Item/Vacuum/Vacuum_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Item_Timer. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item_Timer"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Item/Timer/Timer_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Item_Treasure. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item_Treasure"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Item/Treasure/Treasure_%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Item_Timer_Effect. */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Item_Timer_Effect"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Textures/Item/Timer/Timer_Effect_%d.png"), 1))))
		return E_FAIL;

	// 셰이더 ======================================================================================================
	/* For.Prototype_Component_Shader_Rect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Rect"),
		CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Rect.hlsl")))))
		return E_FAIL;

	//* For.Animation Texture */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_AngryBird"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Ani/AngryBird.jpg"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Santa"),
		CTexture::Create(m_pGraphic_Device, CTexture::TYPE_DEFAULT, TEXT("../Bin/Resources/Ani/Santa.jpg"), 1))))
		return E_FAIL;

	//* For.Animation Bone */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, KEY_COMPONENT_BONE, CBone::Create(m_pGraphic_Device))))
		return E_FAIL;

	//* For.Animation Transform */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AniTransform"), CAniTransform::Create(m_pGraphic_Device))))
		return E_FAIL;
	//* For.Animation Body */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_AniBody"), CVIBuffer_Ani_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;
	//* For.Animation File Parser */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, KEY_COMPONENT_PARSER, CAnimation_Parser::Create(m_pGraphic_Device))))
		return E_FAIL;

	CAnimation_Parser* pParser = (CAnimation_Parser*)pGameInstance->Clone_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Parser"), NULL);

	ST_GAMEOBJECT_FILE_INFO m_stGameObj_FilInfo;
	CString fileName = L"../../Data/zombie.bin";
	pParser->Load_Process_Animation(fileName, m_stGameObj_FilInfo.vecBoneTransform, m_stGameObj_FilInfo.vecAniCube, m_stGameObj_FilInfo.vecAniInfo);

	if (FAILED(pGameInstance->Add_Prototype(L"TEST", (CTest*)CTest::Create(m_pGraphic_Device, m_stGameObj_FilInfo))))
	{
		return E_FAIL;
	}

	for (int i = 0; i < TEST_STATE_END; ++i)
	{
		m_stGameObj_FilInfo.vecBoneTransform[i].swap(vector<ANIMATIONDESC>());
	}
	m_stGameObj_FilInfo.vecBoneTransform.swap(vector<vector<ANIMATIONDESC>>());

	m_stGameObj_FilInfo.vecAniInfo.swap(vector<TEST_STRUCT>());
	m_stGameObj_FilInfo.vecAniCube.swap(vector<STSHAPE>());

	fileName = L"../../Data/santa.bin";
	pParser->Load_Process_Animation(fileName, m_stGameObj_FilInfo.vecBoneTransform, m_stGameObj_FilInfo.vecAniCube, m_stGameObj_FilInfo.vecAniInfo);

	if (FAILED(pGameInstance->Add_Prototype(L"Santa", (CSanta*)CSanta::Create(m_pGraphic_Device, m_stGameObj_FilInfo))))
	{
		return E_FAIL;
	}

	for (int i = 0; i < TEST_STATE_END; ++i)
	{
		m_stGameObj_FilInfo.vecBoneTransform[i].swap(vector<ANIMATIONDESC>());
	}
	m_stGameObj_FilInfo.vecBoneTransform.swap(vector<vector<ANIMATIONDESC>>());

	m_stGameObj_FilInfo.vecAniInfo.swap(vector<TEST_STRUCT>());
	m_stGameObj_FilInfo.vecAniCube.swap(vector<STSHAPE>());

	fileName = L"../../Data/angrybird.bin";
	pParser->Load_Process_Animation(fileName, m_stGameObj_FilInfo.vecBoneTransform, m_stGameObj_FilInfo.vecAniCube, m_stGameObj_FilInfo.vecAniInfo);

	if (FAILED(pGameInstance->Add_Prototype(L"AngryBird", (CAngryBird*)CAngryBird::Create(m_pGraphic_Device, m_stGameObj_FilInfo))))
	{
		return E_FAIL;
	}

	for (int i = 0; i < TEST_STATE_END; ++i)
	{
		m_stGameObj_FilInfo.vecBoneTransform[i].swap(vector<ANIMATIONDESC>());
	}
	m_stGameObj_FilInfo.vecBoneTransform.swap(vector<vector<ANIMATIONDESC>>());

	m_stGameObj_FilInfo.vecAniInfo.swap(vector<TEST_STRUCT>());
	m_stGameObj_FilInfo.vecAniCube.swap(vector<STSHAPE>());

	m_isFinished = true;

#pragma endregion

	Safe_Release(pGameInstance);

	return _uint();
}

CLoader * CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID)
{
	CLoader*	pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eNextLevelID)))
	{
		MSGBOX(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	Safe_Release(m_pGraphic_Device);

	DeleteCriticalSection(&m_CriticalSection);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);
}
