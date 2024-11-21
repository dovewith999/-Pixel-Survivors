#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "MasterCube.h"
#include "Terrain.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

#define WAVE_TIME			3.f
#define TIEM_SPEED			0.1f
#define SPAWN_RANGE			5.f
#define MONSTER_MAX_COUNT	100
#define MONSTER_SPAWN_TIME	0.01f
#define ITEM_SPAWN_TIME		5.f

class CSpawner final : public CGameObject
{
public:
	enum WAVELEVEL{ WAVE_1, /*WAVE_2, WAVE_3,*/ WAVE_END };

public:
	typedef struct tagWave
	{
		WAVELEVEL		eLevel;
		const _tchar*	szMonsterPrototypeTag;
		const _tchar*	szMonsterLayerTag;
		_uint			iMonsterCount;
		TEXMONSTER		eTexMonster;
	}WAVE;

private:
	explicit CSpawner(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CSpawner(const CSpawner& rhs);
	virtual ~CSpawner() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Set_Terrain(CTerrain* pTerrain);

private:
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	
private:
	HRESULT SetUp_Waves();
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	void	Spawn_Monster(_uint iWaveLevel, _float fTimeDelta);
	HRESULT Create_Monster(WAVELEVEL eWaveLevel, const _tchar* szMonsterPrototypeTag, const _tchar* szMonsterTag, TEXMONSTER eTex);
	HRESULT Create_ItemObject();

private:
	_int					m_iMonsterLimitCnt = 50;
	_int					m_iItemLimitCnt = 5;

private:
	_uint					m_iWaveLevel = WAVE_1;
	_float					m_fMUpdateTime = 0.f;
	_float					m_fIUpdateTime = 0.f;
	_float					m_fAccTime = 0.f;

	_uint					m_iMonsterIndex = 0;
	_uint					m_iMonsterNumber = 0;

	bool					m_bSpawn = true;

	RECT					m_tRect;

private:
	CTerrain*				m_pTerrain = nullptr;
	CGameInstance*			m_pGameInstance = nullptr;

private:
	vector<WAVE>*			m_pWaves = nullptr;

public:
	static CSpawner* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END