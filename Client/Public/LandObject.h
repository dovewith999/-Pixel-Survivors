#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Terrain.h"

BEGIN(Engine)
class CTransform;
class CAniTransform;
END

BEGIN(Client)

class CLandObject abstract : public CGameObject
{
protected:
	CLandObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLandObject(const CLandObject& rhs);
	virtual ~CLandObject() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Set_Terrain(CTerrain* pTerrain);

protected:
	HRESULT CLandObject::SetUp_OnTerrain(CTransform * pTransform, _uint iLevelID, const _tchar* pLayerTag);
	HRESULT SetUp_OnTerrain(CAniTransform * pTransform, _uint iLevelID, const _tchar* pLayerTag);
	HRESULT Affected_By_Gravity(class CTransform* pTransform, float fTimeDelta);
	HRESULT Affected_By_Gravity(CAniTransform * pTransform, float fTimeDelta);
protected:
	CTerrain* m_pTerrain = nullptr;

protected:
	float m_fGravityPower = 5.f;
	float m_fJumpPower = 0.f;
	float m_fTimeAccumulate = 0.f;
	bool m_bOnGravity = true;

	//이동할때 땅을 타는 기능 점프할때는 이것을 false로 해주고 점프시켜야한다
	bool m_bRideTerrain = false;
	
protected:
	_uint m_iTerrainIndex = 0;
	int m_iTerrainIndexX = 0;
	int m_iTerrainIndexY = 0;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END