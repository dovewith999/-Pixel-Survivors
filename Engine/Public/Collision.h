#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollision final : public CComponent
{
public:
	//enum COLLISIONGROUP {
	//	GROUP_LT,
	//	GROUP_RT,
	//	GROUP_LB,
	//	GROUP_RB,
	//	GROUP_END
	//};

	enum COLLISIONTYPE {
		TYPE_SETPOSISION,
		TYPE_ATTACK,
		TYPE_INTERACTION,
		TYPE_END
	};

private:
	CCollision(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CCollision() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;

public:
	HRESULT Add_Collisions(_uint iLevelIndex, const _tchar* pLayerTag, class CGameObject* pGameObject, void* pArg = nullptr);
	HRESULT Delete_CollisionObject(_uint iLevelIndex, const TCHAR* pLayerTag, _int iObjectIndex = -1, void* pArg = nullptr);
	CGameObject* Find_CollisionObject(_uint iLevelIndex, const TCHAR* pLayerTag, _int iObjectIndex = -1, void* pArg = nullptr); // ¼öÁ¤
	class CCollisionLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);
	HRESULT Collision_Object(TERRAINGROUP eCollisionGroup, COLLISIONTYPE eType, const _tchar* pSourTag, const _tchar* pDestTag, _float fTimeDelta);
public:
	HRESULT Check_Collision();
	HRESULT Check_Collision(_float fTimeDelta);
public:
	HRESULT Collision_Terrain(TERRAINGROUP eCollisionGroup, COLLISIONTYPE eType, const _tchar* pSourTag, const _tchar* pDestTag);
	HRESULT Collision_Object(TERRAINGROUP eCollisionGroup, COLLISIONTYPE eType, const _tchar* pSourTag, const _tchar* pDestTag);
	bool	AABB(class CCollider* pSourCollider, class CCollider* pDestCollider, _float3* vRange);

private:
	map<const _tchar*, class CCollisionLayer*>				m_Collisions[GROUP_END];
	typedef map<const _tchar*, list<class CGameObject*>>	COLLISIONS;

public:
	static CCollision* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;

};

END