#include "..\Public\Collision.h"
#include "GameObject.h"
#include "CollisionLayer.h"
#include "Collider.h"
#include "GameInstance.h"

CCollision::CCollision(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

HRESULT CCollision::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CCollision::Add_Collisions(_uint iLevelIndex, const _tchar * pLayerTag, class CGameObject* pGameObject, void * pArg)
{
	if (nullptr == m_Collisions)
		return E_FAIL;

	CCollisionLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CCollisionLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		if (FAILED(pLayer->Add_GameObject(pGameObject)))
			return E_FAIL;

		m_Collisions[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	else
	{
		if (FAILED(pLayer->Add_GameObject(pGameObject)))
			return E_FAIL;
	}

	return S_OK;
}

CCollisionLayer * CCollision::Find_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	auto iter = find_if(m_Collisions[iLevelIndex].begin(), m_Collisions[iLevelIndex].end(), CTagFinder(pLayerTag));

	if (iter == m_Collisions[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

HRESULT CCollision::Delete_CollisionObject(_uint iLevelIndex, const TCHAR * pLayerTag, _int iObjectIndex, void * pArg)
{
	CCollisionLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return E_FAIL;

	if (-1 == iObjectIndex)
		iObjectIndex = (int)pLayer->m_GameObjects.size() - 1;

	if (FAILED(pLayer->Delete_GameObject((_uint)iObjectIndex)))
		return E_FAIL;

	return S_OK;
}

CGameObject * CCollision::Find_CollisionObject(_uint iLevelIndex, const TCHAR * pLayerTag, _int iObjectIndex, void * pArg)
{
	CCollisionLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	if (-1 == iObjectIndex)
		iObjectIndex = (int)pLayer->m_GameObjects.size() - 1;

	CGameObject* pGameObject = pLayer->Find_GameObject((_uint)iObjectIndex);
	if (nullptr == pGameObject)
		return nullptr;

	return pGameObject;
}


HRESULT CCollision::Check_Collision()
{
	// 지형 <> Object
	// Object <> Object

	for (int i = 0; i < GROUP_END; ++i)
	{
		if (GROUP_TERRAIN == i)
		{
			for (int j = i + 1; j < GROUP_END; ++j)
			{
				Collision_Terrain((TERRAINGROUP)j, TYPE_SETPOSISION, L"Wall", L"Player");
			}
		}
		else
		{
			for (auto& Pair : m_Collisions[i])
			{
				Collision_Object((TERRAINGROUP)i, TYPE_SETPOSISION, L"Player", L"Monster");
				Collision_Object((TERRAINGROUP)i, TYPE_SETPOSISION, L"Monster", L"Monster");
				Collision_Object((TERRAINGROUP)i, TYPE_ATTACK, L"PlayerAttack", L"Monster");
				Collision_Object((TERRAINGROUP)i, TYPE_ATTACK, L"EnemyAttack", L"Player");
				Collision_Object((TERRAINGROUP)i, TYPE_ATTACK, L"Monster", L"AttacktedEffect");
				

				// 0511
				Collision_Object((TERRAINGROUP)i, TYPE_INTERACTION, L"Player", L"Item");

				Collision_Object((TERRAINGROUP)i, TYPE_INTERACTION, L"PlayerAttack", L"Item_Object");

				Collision_Object((TERRAINGROUP)i, TYPE_INTERACTION, L"PlayerAttack", L"Item_Obejct");

				Safe_Release(Pair.second);
			}
			m_Collisions[i].clear();
		}
	}


	return S_OK;
}

HRESULT CCollision::Collision_Terrain(TERRAINGROUP eCollisionGroup, COLLISIONTYPE eType, const _tchar* pSourTag, const _tchar* pDestTag)
{
	if (eCollisionGroup >= GROUP_END)
		return E_FAIL;

	CCollisionLayer* pSourLayer = Find_Layer(GROUP_TERRAIN, pSourTag);
	CCollisionLayer* pDestLayer = Find_Layer(eCollisionGroup, pDestTag);

	if (nullptr == pSourLayer || nullptr == pDestLayer)
		return E_FAIL;

	CCollider* pSourCollider = nullptr;
	CCollider* pDestCollider = nullptr;

	bool bCollision = false;

	// TYPE_SETPOSISION
	_float3 vRange = { 0.f, 0.f, 0.f };
	_float3 vSourPos = { 0.f, 0.f, 0.f };
	_float3 vDestPos = { 0.f, 0.f, 0.f };

	for (_uint iSourIdx = 0; iSourIdx < pSourLayer->Get_ObjectCount(); ++iSourIdx)
	{
		CGameObject* pSourObejct = pSourLayer->Find_GameObject(iSourIdx);

		for (_uint iDestIdx = 0; iDestIdx < pDestLayer->Get_ObjectCount(); ++iDestIdx)
		{
			CGameObject* pDestObejct = pDestLayer->Find_GameObject(iDestIdx);

			pSourCollider = dynamic_cast<CCollider*>(pSourObejct->Find_Component(L"Com_Collider"));
			pDestCollider = dynamic_cast<CCollider*>(pDestObejct->Find_Component(L"Com_Collider"));
			bCollision = AABB(pSourCollider, pDestCollider, &vRange);

			_float3 vSourSize;
			_float3 vDestSize;

			CAniTransform* pAniTransform = nullptr;
			CTransform* pTransform = nullptr;

			if (bCollision)
			{
				switch (eType)
				{
				case TYPE_SETPOSISION:
					vSourPos = pSourCollider->Get_Center();

					if (pDestObejct->Get_Tag().IsEmpty())
					{
						pTransform = dynamic_cast<CTransform*>(pDestObejct->Find_Component(L"Com_Transform"));
						if (pTransform)
							vDestPos = pTransform->Get_State(CTransform::STATE_POSITION);
					}
					else
					{

						pAniTransform = dynamic_cast<CAniTransform*>(pDestObejct->Find_Component(L"Com_AniTransform"));
						if (pAniTransform)
							vDestPos = pAniTransform->Get_State(CAniTransform::ANI_TRANS_POSITION);
					}

					if (!pDestObejct->Get_Tag().IsEmpty())
					{
						if (pAniTransform)
							vDestPos = pAniTransform->Get_State(CAniTransform::ANI_TRANS_POSITION);
						else
							continue;
					}
					else if (pDestObejct->Get_Tag().IsEmpty())
					{
						if (pTransform)
							vDestPos = pTransform->Get_State(CTransform::STATE_POSITION);
						else
							continue;
					}

					if (vRange.x > vRange.z)		// 상하 충돌의 경우
					{
						if (vDestPos.z < vSourPos.z)	// 상 충돌
							vDestPos.z -= vRange.z;
						else						// 하 충돌 : vDestPos.z += vRange.z 하면 벽에 껴버림. 일단 임시 조치
							vDestPos.z -= vRange.z;
					}
					else							// 좌우 충돌의 경우
					{
						if (vDestPos.x < vSourPos.x)	// 좌 충돌 
							vDestPos.x -= vRange.x;
						else
							vDestPos.x -= vRange.x;	// 우 충돌 : vDestPos.x += vRange.x 하면 벽에 껴버림. 일단 임시 조치
					}
					if (!pDestObejct->Get_Tag().IsEmpty())
					{
						if (pAniTransform)
							pAniTransform->Set_State(CAniTransform::ANI_TRANS_POSITION, vDestPos);
						else
							continue;
					}
					else if (pDestObejct->Get_Tag().IsEmpty())
					{
						if (pTransform)
							pTransform->Set_State(CTransform::STATE_POSITION, vDestPos);
						else
							continue;
					}
					break;
				case TYPE_ATTACK:
				case TYPE_INTERACTION:
					break;
				}
			}
		}
	}
	return S_OK;
}

HRESULT CCollision::Check_Collision(_float fTimeDelta)
{
	// 지형 <> Object
	// Object <> Object

	for (int i = 0; i < GROUP_END; ++i)
	{
		if (GROUP_TERRAIN == i)
		{
			for (int j = i + 1; j < GROUP_END; ++j)
			{
				Collision_Terrain((TERRAINGROUP)j, TYPE_SETPOSISION, L"Wall", L"Player");
			}
		}
		else
		{
			for (auto& Pair : m_Collisions[i])
			{
				Collision_Object((TERRAINGROUP)i, TYPE_SETPOSISION, L"Player", L"Monster", fTimeDelta);
				Collision_Object((TERRAINGROUP)i, TYPE_SETPOSISION, L"Monster", L"Monster", fTimeDelta);
				Collision_Object((TERRAINGROUP)i, TYPE_ATTACK, L"PlayerAttack", L"Monster", fTimeDelta);
				Collision_Object((TERRAINGROUP)i, TYPE_ATTACK, L"EnemyAttack", L"Player", fTimeDelta);

				// 0511
				Collision_Object((TERRAINGROUP)i, TYPE_INTERACTION, L"Player", L"Item", fTimeDelta);
				Collision_Object((TERRAINGROUP)i, TYPE_INTERACTION, L"PlayerAttack", L"Item_Obejct", fTimeDelta);
				//Collision_Object((TERRAINGROUP)i, TYPE_INTERACTION, L"Player", L"Item_Object");

				Safe_Release(Pair.second);
			}
			m_Collisions[i].clear();
		}
	}


	return S_OK;
}
//합칠 때 주의 할 것
HRESULT CCollision::Collision_Object(TERRAINGROUP eCollisionGroup, COLLISIONTYPE eType, const _tchar* pSourTag, const _tchar* pDestTag)
{
	if (eCollisionGroup >= GROUP_END)
		return E_FAIL;

	CCollisionLayer* pSourLayer = Find_Layer(eCollisionGroup, pSourTag);
	CCollisionLayer* pDestLayer = Find_Layer(eCollisionGroup, pDestTag);

	if (nullptr == pSourLayer || nullptr == pDestLayer)
		return E_FAIL;

	CCollider* pSourCollider = nullptr;
	CCollider* pDestCollider = nullptr;

	bool bCollision = false;

	// TYPE_SETPOSISION
	_float3 vRange = { 0.f, 0.f, 0.f };
	_float3 vSourPos = { 0.f, 0.f, 0.f };
	_float3 vDestPos = { 0.f, 0.f, 0.f };

	CAniTransform* pAniTransform = nullptr;
	CTransform* pTransform = nullptr;

	if (pSourTag != pDestTag)
	{
		for (_uint iSourIdx = 0; iSourIdx < pSourLayer->Get_ObjectCount(); ++iSourIdx)
		{
			CGameObject* pSourObejct = pSourLayer->Find_GameObject(iSourIdx);

			for (_uint iDestIdx = 0; iDestIdx < pDestLayer->Get_ObjectCount(); ++iDestIdx)
			{
				CGameObject* pDestObejct = pDestLayer->Find_GameObject(iDestIdx);

				pSourCollider = dynamic_cast<CCollider*>(pSourObejct->Find_Component(L"Com_Collider"));
				pDestCollider = dynamic_cast<CCollider*>(pDestObejct->Find_Component(L"Com_Collider"));
				bCollision = AABB(pSourCollider, pDestCollider, &vRange);

				_float3 vSourSize;
				_float3 vDestSize;

				if (bCollision)
				{
					if (nullptr != pSourObejct->Get_SoundKey())
					{
						if (lstrcmp(pSourObejct->Get_SoundKey(), L"CPlasma"))
						{
							CSound_Manager::Get_Instance()->PlaySoundW(L"firecast.wav", SOUND_PLAYER, 1.f);
						}
					}
					switch (eType)
					{
					case TYPE_SETPOSISION:
						if (pSourObejct->Get_Tag().IsEmpty())
						{
							pTransform = dynamic_cast<CTransform*>(pSourObejct->Find_Component(L"Com_Transform"));
							if (pTransform)
								vSourPos = pTransform->Get_State(CTransform::STATE_POSITION);
						}
						else
						{
							pAniTransform = dynamic_cast<CAniTransform*>(pSourObejct->Find_Component(L"Com_AniTransform"));
							if (pAniTransform)
								vSourPos = pAniTransform->Get_State(CAniTransform::ANI_TRANS_POSITION);
						}

						if (pDestObejct->Get_Tag().IsEmpty())
						{
							pTransform = dynamic_cast<CTransform*>(pDestObejct->Find_Component(L"Com_Transform"));
							if (pTransform)
								vDestPos = pTransform->Get_State(CTransform::STATE_POSITION);
						}
						else
						{
							pAniTransform = dynamic_cast<CAniTransform*>(pDestObejct->Find_Component(L"Com_AniTransform"));
							if (pAniTransform)
								vDestPos = pAniTransform->Get_State(CAniTransform::ANI_TRANS_POSITION);
						}

						if (vRange.x > vRange.z)		// 상하 충돌의 경우
						{
							if (vDestPos.z < vSourPos.z)	// 상 충돌
								vDestPos.z -= vRange.z;
							else						// 하 충돌
								vDestPos.z += vRange.z;
						}
						else							// 좌우 충돌의 경우
						{
							if (vDestPos.x < vSourPos.x)	// 좌 충돌
								vDestPos.x -= vRange.x;
							else
								vDestPos.x += vRange.x;	// 우 충돌
						}

						float fCos, fDot, fScale;

						fDot = D3DXVec3Dot(&vDestPos, &vSourPos);
						fScale = D3DXVec3Length(&vDestPos) * D3DXVec3Length(&vSourPos);
						fCos = fDot / fScale;

						vDestPos.x += 2.f * cosf(fCos);
						vDestPos.z += 2.f * sinf(fCos);

						if (pDestObejct->Get_Tag().IsEmpty())
						{
							pTransform = dynamic_cast<CTransform*>(pDestObejct->Find_Component(L"Com_Transform"));
							if (pTransform)
								pTransform->Set_State(CTransform::STATE_POSITION, vDestPos);
						}
						else
						{
							pAniTransform = dynamic_cast<CAniTransform*>(pDestObejct->Find_Component(L"Com_AniTransform"));
							if (pAniTransform)
								pAniTransform->Set_State(CAniTransform::ANI_TRANS_POSITION, vDestPos);
						}

						break;
					case TYPE_ATTACK:
						if (true)
						{
							CBattle* pSourBattle = dynamic_cast<CBattle*>(pSourObejct->Find_Component(L"Com_Battle"));
							CBattle* pDestBattle = dynamic_cast<CBattle*>(pDestObejct->Find_Component(L"Com_Battle"));

							int tA = pDestBattle->Attackted(pSourBattle->Get_FinalAtk(), &pSourBattle->Get_BattleData());
						}
						break;

					case TYPE_INTERACTION:
						pDestObejct->Set_Active();
						break;
					}
				}
			}
		}
	}
	else
	{
		for (_uint iSourIdx = 0; iSourIdx < pSourLayer->Get_ObjectCount(); ++iSourIdx)
		{
			CGameObject* pSourObejct = pSourLayer->Find_GameObject(iSourIdx);

			for (_uint iDestIdx = iSourIdx + 1; iDestIdx < pDestLayer->Get_ObjectCount(); ++iDestIdx)
			{
				CGameObject* pDestObejct = pDestLayer->Find_GameObject(iDestIdx);

				pSourCollider = dynamic_cast<CCollider*>(pSourObejct->Find_Component(L"Com_Collider"));
				pDestCollider = dynamic_cast<CCollider*>(pDestObejct->Find_Component(L"Com_Collider"));
				bCollision = AABB(pSourCollider, pDestCollider, &vRange);

				_float3 vSourSize;
				_float3 vDestSize;

				if (bCollision)
				{
					switch (eType)
					{
					case TYPE_SETPOSISION:
						if (pSourObejct->Get_Tag().IsEmpty())
						{
							pTransform = dynamic_cast<CTransform*>(pSourObejct->Find_Component(L"Com_Transform"));
							if (pTransform)
								vSourPos = pTransform->Get_State(CTransform::STATE_POSITION);
						}
						else
						{
							pAniTransform = dynamic_cast<CAniTransform*>(pSourObejct->Find_Component(L"Com_AniTransform"));
							if (pAniTransform)
								vSourPos = pAniTransform->Get_State(CAniTransform::ANI_TRANS_POSITION);
						}

						if (pDestObejct->Get_Tag().IsEmpty())
						{
							pTransform = dynamic_cast<CTransform*>(pDestObejct->Find_Component(L"Com_Transform"));
							if (pTransform)
								vDestPos = pTransform->Get_State(CTransform::STATE_POSITION);
						}
						else
						{
							pAniTransform = dynamic_cast<CAniTransform*>(pDestObejct->Find_Component(L"Com_AniTransform"));
							if (pAniTransform)
								vDestPos = pAniTransform->Get_State(CAniTransform::ANI_TRANS_POSITION);
						}

						if (vRange.x > vRange.z)		// 상하 충돌의 경우
						{
							if (vDestPos.z < vSourPos.z)	// 상 충돌
								vDestPos.z -= vRange.z;
							else						// 하 충돌
								vDestPos.z += vRange.z;
						}
						else							// 좌우 충돌의 경우
						{
							if (vDestPos.x < vSourPos.x)	// 좌 충돌
								vDestPos.x -= vRange.x;
							else
								vDestPos.x += vRange.x;	// 우 충돌
						}
						float fCos, fDot, fScale;
					
						fDot = D3DXVec3Dot(&vDestPos, &vSourPos);
						fScale = D3DXVec3Length(&vDestPos) * D3DXVec3Length(&vSourPos);
						fCos = fDot / fScale;

						vDestPos.x += 2.f * cosf(fCos);
						vDestPos.z += 2.f * sinf(fCos);

						if (pDestObejct->Get_Tag().IsEmpty())
						{
							pTransform = dynamic_cast<CTransform*>(pDestObejct->Find_Component(L"Com_Transform"));
							if (pTransform)
								pTransform->Set_State(CTransform::STATE_POSITION, vDestPos);
						}
						else
						{
							pAniTransform = dynamic_cast<CAniTransform*>(pDestObejct->Find_Component(L"Com_AniTransform"));
							if (pAniTransform)
								pAniTransform->Set_State(CAniTransform::ANI_TRANS_POSITION, vDestPos);
						}

						break;
					case TYPE_ATTACK:
						if (true)
						{
							CBattle* pSourBattle = dynamic_cast<CBattle*>(pSourObejct->Find_Component(L"Com_Battle"));
							CBattle* pDestBattle = dynamic_cast<CBattle*>(pDestObejct->Find_Component(L"Com_Battle"));

							int tA = pDestBattle->Attackted(pSourBattle->Get_FinalAtk(), &pSourBattle->Get_BattleData());
						}
						break;

					case TYPE_INTERACTION:
						pDestObejct->Set_Active();
						break;
					}
				}
			}
		}
	}

	return S_OK;
}

HRESULT CCollision::Collision_Object(TERRAINGROUP eCollisionGroup, COLLISIONTYPE eType, const _tchar* pSourTag, const _tchar* pDestTag, _float fTimeDelta)
{
	if (eCollisionGroup >= GROUP_END)
		return E_FAIL;

	CCollisionLayer* pSourLayer = Find_Layer(eCollisionGroup, pSourTag);
	CCollisionLayer* pDestLayer = Find_Layer(eCollisionGroup, pDestTag);

	if (nullptr == pSourLayer || nullptr == pDestLayer)
		return E_FAIL;

	CCollider* pSourCollider = nullptr;
	CCollider* pDestCollider = nullptr;

	bool bCollision = false;

	// TYPE_SETPOSISION
	_float3 vRange = { 0.f, 0.f, 0.f };
	_float3 vSourPos = { 0.f, 0.f, 0.f };
	_float3 vDestPos = { 0.f, 0.f, 0.f };

	CAniTransform* pAniTransform = nullptr;
	CTransform* pTransform = nullptr;

	if (pSourTag != pDestTag)
	{
		for (_uint iSourIdx = 0; iSourIdx < pSourLayer->Get_ObjectCount(); ++iSourIdx)
		{
			CGameObject* pSourObejct = pSourLayer->Find_GameObject(iSourIdx);

			for (_uint iDestIdx = 0; iDestIdx < pDestLayer->Get_ObjectCount(); ++iDestIdx)
			{
				CGameObject* pDestObejct = pDestLayer->Find_GameObject(iDestIdx);

				pSourCollider = dynamic_cast<CCollider*>(pSourObejct->Find_Component(L"Com_Collider"));
				pDestCollider = dynamic_cast<CCollider*>(pDestObejct->Find_Component(L"Com_Collider"));
				bCollision = AABB(pSourCollider, pDestCollider, &vRange);

				_float3 vSourSize;
				_float3 vDestSize;

				if (bCollision)
				{
					if (nullptr != pSourObejct->Get_SoundKey())
					{
						if (lstrcmp(pSourObejct->Get_SoundKey(), L"CPlasma"))
						{
							CSound_Manager::Get_Instance()->PlaySoundW(L"firecast.wav", SOUND_PLAYER, 1.f);
						}
					}
					switch (eType)
					{
					case TYPE_SETPOSISION:
						if (pSourObejct->Get_Tag().IsEmpty())
						{
							pTransform = dynamic_cast<CTransform*>(pSourObejct->Find_Component(L"Com_Transform"));
							if (pTransform)
								vSourPos = pTransform->Get_State(CTransform::STATE_POSITION);
						}
						else
						{
							pAniTransform = dynamic_cast<CAniTransform*>(pSourObejct->Find_Component(L"Com_AniTransform"));
							if (pAniTransform)
								vSourPos = pAniTransform->Get_State(CAniTransform::ANI_TRANS_POSITION);
						}

						if (pDestObejct->Get_Tag().IsEmpty())
						{
							pTransform = dynamic_cast<CTransform*>(pDestObejct->Find_Component(L"Com_Transform"));
							if (pTransform)
								vDestPos = pTransform->Get_State(CTransform::STATE_POSITION);
						}
						else
						{
							pAniTransform = dynamic_cast<CAniTransform*>(pDestObejct->Find_Component(L"Com_AniTransform"));
							if (pAniTransform)
								vDestPos = pAniTransform->Get_State(CAniTransform::ANI_TRANS_POSITION);
						}

						if (vRange.x > vRange.z)		// 상하 충돌의 경우
						{
							if (vDestPos.z < vSourPos.z)	// 상 충돌
								vDestPos.z -= vRange.z;
							else						// 하 충돌
								vDestPos.z += vRange.z;
						}
						else							// 좌우 충돌의 경우
						{
							if (vDestPos.x < vSourPos.x)	// 좌 충돌
								vDestPos.x -= vRange.x;
							else
								vDestPos.x += vRange.x;	// 우 충돌
						}
						vDestPos += _float3{ 1.f, 0.f, 0.f };
						if (pDestObejct->Get_Tag().IsEmpty())
						{
							pTransform = dynamic_cast<CTransform*>(pDestObejct->Find_Component(L"Com_Transform"));
							if (pTransform)
								pTransform->Set_State(CTransform::STATE_POSITION, vDestPos, fTimeDelta);
						}
						else
						{
							pAniTransform = dynamic_cast<CAniTransform*>(pDestObejct->Find_Component(L"Com_AniTransform"));
							if (pAniTransform)
								pAniTransform->Set_State(CAniTransform::ANI_TRANS_POSITION, vDestPos, fTimeDelta);
						}

						break;
					case TYPE_ATTACK:
						if (true)
						{
							CBattle* pSourBattle = dynamic_cast<CBattle*>(pSourObejct->Find_Component(L"Com_Battle"));
							CBattle* pDestBattle = dynamic_cast<CBattle*>(pDestObejct->Find_Component(L"Com_Battle"));

							int tA = pDestBattle->Attackted(pSourBattle->Get_FinalAtk(), &pSourBattle->Get_BattleData());
						}
						break;

					case TYPE_INTERACTION:
						pDestObejct->Set_Active();
						break;
					}
				}
			}
		}
	}
	else
	{
		for (_uint iSourIdx = 0; iSourIdx < pSourLayer->Get_ObjectCount(); ++iSourIdx)
		{
			CGameObject* pSourObejct = pSourLayer->Find_GameObject(iSourIdx);

			for (_uint iDestIdx = iSourIdx + 1; iDestIdx < pDestLayer->Get_ObjectCount(); ++iDestIdx)
			{
				CGameObject* pDestObejct = pDestLayer->Find_GameObject(iDestIdx);

				pSourCollider = dynamic_cast<CCollider*>(pSourObejct->Find_Component(L"Com_Collider"));
				pDestCollider = dynamic_cast<CCollider*>(pDestObejct->Find_Component(L"Com_Collider"));
				bCollision = AABB(pSourCollider, pDestCollider, &vRange);

				_float3 vSourSize;
				_float3 vDestSize;

				if (bCollision)
				{
					switch (eType)
					{
					case TYPE_SETPOSISION:
						if (pSourObejct->Get_Tag().IsEmpty())
						{
							pTransform = dynamic_cast<CTransform*>(pSourObejct->Find_Component(L"Com_Transform"));
							if (pTransform)
								vSourPos = pTransform->Get_State(CTransform::STATE_POSITION);
						}
						else
						{
							pAniTransform = dynamic_cast<CAniTransform*>(pSourObejct->Find_Component(L"Com_AniTransform"));
							if (pAniTransform)
								vSourPos = pAniTransform->Get_State(CAniTransform::ANI_TRANS_POSITION);
						}

						if (pDestObejct->Get_Tag().IsEmpty())
						{
							pTransform = dynamic_cast<CTransform*>(pDestObejct->Find_Component(L"Com_Transform"));
							if (pTransform)
								vDestPos = pTransform->Get_State(CTransform::STATE_POSITION);
						}
						else
						{
							pAniTransform = dynamic_cast<CAniTransform*>(pDestObejct->Find_Component(L"Com_AniTransform"));
							if (pAniTransform)
								vDestPos = pAniTransform->Get_State(CAniTransform::ANI_TRANS_POSITION);
						}

						if (vRange.x > vRange.z)		// 상하 충돌의 경우
						{
							if (vDestPos.z < vSourPos.z)	// 상 충돌
								vDestPos.z -= vRange.z;
							else						// 하 충돌
								vDestPos.z += vRange.z;
						}
						else							// 좌우 충돌의 경우
						{
							if (vDestPos.x < vSourPos.x)	// 좌 충돌
								vDestPos.x -= vRange.x;
							else
								vDestPos.x += vRange.x;	// 우 충돌
						}

						if (pDestObejct->Get_Tag().IsEmpty())
						{
							pTransform = dynamic_cast<CTransform*>(pDestObejct->Find_Component(L"Com_Transform"));
							if (pTransform)
								pTransform->Set_State(CTransform::STATE_POSITION, vDestPos);
						}
						else
						{
							pAniTransform = dynamic_cast<CAniTransform*>(pDestObejct->Find_Component(L"Com_AniTransform"));
							if (pAniTransform)
								pAniTransform->Set_State(CAniTransform::ANI_TRANS_POSITION, vDestPos);
						}

						break;
					case TYPE_ATTACK:
						if (true)
						{
							CBattle* pSourBattle = dynamic_cast<CBattle*>(pSourObejct->Find_Component(L"Com_Battle"));
							CBattle* pDestBattle = dynamic_cast<CBattle*>(pDestObejct->Find_Component(L"Com_Battle"));

							int tA = pDestBattle->Attackted(pSourBattle->Get_FinalAtk(), &pSourBattle->Get_BattleData());
						}
						break;

					case TYPE_INTERACTION:
						break;
					}
				}
			}
		}
	}

	return S_OK;
}

bool CCollision::AABB(CCollider* pSourCollider, CCollider* pDestCollider, _float3* vRange)
{
	_float3 vSourMinPt = pSourCollider->Get_MinPoint();
	_float3 vSourMaxPt = pSourCollider->Get_MaxPoint();

	_float3 vDestMinPt = pDestCollider->Get_MinPoint();
	_float3 vDestMaxPt = pDestCollider->Get_MaxPoint();

	if (vSourMaxPt.x < vDestMinPt.x || vSourMinPt.x > vDestMaxPt.x) return false;
	if (vSourMaxPt.y < vDestMinPt.y || vSourMinPt.y > vDestMaxPt.y) return false;
	if (vSourMaxPt.z < vDestMinPt.z || vSourMinPt.z > vDestMaxPt.z) return false;

	_float3 vSourSize = pSourCollider->Get_Size() / 2.f;
	_float3 vDestSize = pDestCollider->Get_Size() / 2.f;

	_float3 vSourCenter = pSourCollider->Get_Center();
	_float3 vDestCenter = pDestCollider->Get_Center();

	_float fX = vDestSize.x + vSourSize.x - fabs((vDestCenter.x - vSourCenter.x));
	_float fZ = vDestSize.z + vSourSize.z - fabs((vDestCenter.z - vSourCenter.z));

	*vRange = { fX, 0.f, fZ };

	return true;
}

CCollision * CCollision::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollision*	pInstance = new CCollision(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CCollision"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollision::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CCollision::Free()
{
	__super::Free();

	for (_uint i = 0; i < GROUP_END; ++i)
	{
		for (auto& Pair : m_Collisions[i])
			Safe_Release(Pair.second);
		m_Collisions[i].clear();
	}

}
