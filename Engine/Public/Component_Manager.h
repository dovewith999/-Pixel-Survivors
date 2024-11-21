#pragma once

#include "Texture.h"
#include "Renderer.h"
#include "Transform.h"
#include "AniTransform_hhlee.h"
#include "Battle.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_ColorCube.h"
#include "VIBuffer_Terrain_Cube.h"
#include "VIBuffer_Cube_hhlee.h"
#include "Animation_Parser_hhlee.h"
#include "Collider.h"
#include "Collision.h"

// 0505
#include "VIBuffer_Tile.h"

// 0512
#include "Shader.h"

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
public:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);


private:
	_uint				m_iNumLevels = 0;

	map<const _tchar*, class CComponent*>*				m_pPrototypes = nullptr;
	typedef map<const _tchar*, class CComponent*>		PROTOTYPES;

private:
	class CComponent* Find_Component(_uint iLevelIndex, const _tchar* pPrototypeTag);

public:
	virtual void Free() override;
};

END