#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CVIBuffer_Terrain;
END

BEGIN(Client)
class MyTerrain final : public CGameObject
{
public:
	explicit MyTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit MyTerrain(const MyTerrain& rhs);
    virtual	~MyTerrain() override = default;

public:
	enum MODE { MODE_NONTEXTURE, MODE_TEXTURE };

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	/* 큐브 관련 함수들 */
public:
	void	Set_CubeMode(_uint iMode);
	void	Set_CubeHeight(const RECTRANGE& tagRange, _uint iDirection, _int iHeight);

public:
	void			Create_Cube(const RECTRANGE& tagRange, _uint iCubeCount);
	void			Remove_Cube(const RECTRANGE& tagRange, _uint iCubeCount);
	class CCube*	Find_Cube(_uint iIndex, _float fHeight);

public:
	_uint Check_CubeIndex(_float3 vPos); // 04/26 임희섭 추우가
	_uint m_iCubeIndex = 0;

	/* Terrain 초기화때 필요한 변수들 */
public:
	_uint	m_iCubeCountX = 0;
	_uint	m_iCubeCountZ = 0;
	_uint	m_iCubeCount = 0;
	MODE	eMode = MODE_NONTEXTURE;
	
	/* Terrain Map */
public:
	map<_uint, list<class TerrainCube*>> m_CubeMap;
	map<_uint, list<_uint>> m_IndexMap;

	/* Terrain Component */
private:
	CTransform*				m_pTransformCom = nullptr;
	//CVIBuffer_Terrain*		m_pVIBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();
	

public:
	HRESULT Save_Data(HANDLE& hFile, DWORD& dwByte);
	
public:

	static MyTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

