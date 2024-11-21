#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "MasterCube.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CBullet final : public CGameObject
{
public:
	explicit CBullet(LPDIRECT3DDEVICE9 _GraphicDevice);
	explicit CBullet(const CBullet& rhs);
	virtual ~CBullet() override = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	HRESULT Render_Cube();

	//public: // 05/04 ¿”»Òº∑ √ﬂøÏ∞°
	//	_float3 Get_RayDir() { return m_vWorldRayDir; }
	//	_float3 Get_RayPos() { return m_vWorldRayPos; }
	//
	//public:
	//	void Shot_Bullet();
	//
	//private:
	//	_float3				m_vWorldRayDir, m_vWorldRayPos;

private:
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	CTransform*				m_pCameraTransform = nullptr;

private:
	CMasterCube*			m_pMasterCube = nullptr;
	WEAPONSTATS				m_tagWeaponStats; // 04/28 ¿”»Òº∑ √ﬂøÏ∞°
	float					m_fAngle = 0.f;
	float					m_fMotionSpeed = 15.f;
	_float3					m_vRight;
	_float3					m_vUp;
	_float3					m_vLook;
	bool					m_bShow = true;

	ATTACK_COLLISION		m_eCollisionInfo;
	//WEAPON					m_eWeapon = SWORD;


	CCube*					m_pCube = nullptr; // 04/28 ¿”»Òº∑ √ﬂøÏ∞°

private:
	DWORD		dwByte = 0;
	D3DXVECTOR3 MasterCubeSize;
	D3DXVECTOR3 AllCubeSize;
	D3DXVECTOR3 AllVertexPos;
	int			CubeCount = 0;
	CUBE		eCube;
	int			iIndex;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CBullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

