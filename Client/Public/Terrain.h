#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Terrain;
END

class CTerrain final : public CGameObject
{
public:
	enum MODE { MODE_NONTEXTURE, MODE_TEXTURE };

private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Mode(MODE eMode);
	void Set_Position(const _float3& vPosition);
	void Rotation(const _float3& vAxis, _float fRadian);

public:
	_int Get_Height(const _float3& vPosition);
	_uint Get_TerrainRange(const _float3& vPosition, int &iTerrainIndexX, int &iTerrainIndexY);
	_uint Get_TerrainRangeAttackCollision(const _float3& vPosition);

public:
	TERRAIN m_tTerrain;
	MODE m_eMode = MODE_TEXTURE;

public:
	_uint					m_iTileCountX = 0;
	_uint					m_iTileCountY = 0;
	_uint					m_iTileCountZ = 0;

	_uint					m_iTileDivision = 3;

	int*					m_iHeights = nullptr;
	//ROLE					m_eRole = ROLE_END;

private:
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Terrain*		m_pVIBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();


public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};