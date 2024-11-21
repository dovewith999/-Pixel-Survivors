#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "Camera_Player.h"
#include "Cube.h"
#include "MasterCube.h"
#include "Terrain.h"
#include "Player.h"
#include "Spawner.h"
#include "Wall.h"

#include "UI_HPElement.h"

BEGIN(Client)


class CLevel_GamePlay final : public CLevel
{
private:
	explicit CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT NativeConstruct();
	virtual void Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag, CAMERA eCamera);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI_Inventory(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI_HPBar(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI_WeaponIcon(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI_ExperienceBar(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI_ExperiencePoint(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI_SelectItem(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI_PlayTimer(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI_Cursor(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI_PlayerStats(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI_LevelUp(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI_ItemBox(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Spawner(const _tchar* pLayerTag); // CH Ãß°¡
	HRESULT Ready_Layer_Test(const _tchar* pLayerTag); // 0511
	
private:
	CPlayer*		m_pPlayer = nullptr;
	CMasterCube*	m_pMasterCube = nullptr;
	CCube*			m_pCube = nullptr;
	CTerrain*		m_pTerrain = nullptr;
	CCamera_Player* m_pCameraPlayer = nullptr;
	CUI_HPBar*		m_pHPBar = nullptr;

public:
	static CLevel_GamePlay* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END

