#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "MasterCube.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CWeapon : public CLandObject
{
protected:
	enum WEAPON_STATE { WEAPON_SWING, WEAPON_RETURN, WEAPON_IDLE };

protected:
	explicit CWeapon(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CWeapon(const CWeapon& rhs);
	virtual ~CWeapon() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT Set_MasterCube(CMasterCube* MasterCube);
	virtual HRESULT Set_Turn(D3DXVECTOR3 vAxis, float fTimeDelta);
	virtual HRESULT Set_TurnAngle(D3DXVECTOR3 vAxis, float fAngle);
	virtual HRESULT Set_WeaponPos(D3DXVECTOR3 vPos, D3DXVECTOR3 vRight, D3DXVECTOR3 vUp, D3DXVECTOR3 vLook);
	virtual HRESULT Set_WeaponInfo();
	virtual ATTACK_COLLISION* Set_StateAttack();

	virtual void State_Motion();
	virtual void Set_WeaponStats(WEAPONSTATS _tagWeaponStats) { // 04/28 ¿”»Òº∑ √ﬂøÏ∞°
		m_tagWeaponStats = _tagWeaponStats;
	}
	virtual void Set_CameraTransform(CTransform* pTransform) { m_pCameraTransform = pTransform; 
	}

	virtual void Set_Show(bool Show) { m_bShow = Show; }

public:
	virtual D3DXVECTOR3 Get_PlayerPos() { return m_pTransformCom->Get_State(CTransform::STATE_POSITION); }
	virtual WEAPONSTATS Get_WeaponStats() const { return m_tagWeaponStats; } // 04/28 ¿”»Òº∑ √ﬂøÏ∞°

public: // 04/28 ¿”»Òº∑ √ﬂøÏ∞°
	void Change_Weapon(const _tchar* _FilePath);
	virtual HRESULT Render_Cube();

protected:
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	CTransform*				m_pCameraTransform = nullptr;

protected:
	CMasterCube*			m_pMasterCube = nullptr;
	WEAPONSTATS				m_tagWeaponStats; // 04/28 ¿”»Òº∑ √ﬂøÏ∞°
	float					m_fAngle = 0.f;
	float					m_fMotionSpeed = 15.f;
	_float3					m_vRight;
	_float3					m_vUp;
	_float3					m_vLook;
	WEAPON_STATE			m_eState = WEAPON_IDLE;
	bool					m_bShow = true;

	ATTACK_COLLISION		m_eCollisionInfo;
	//WEAPON					m_eWeapon = SWORD;
	

	CCube*					m_pCube = nullptr; // 04/28 ¿”»Òº∑ √ﬂøÏ∞°

protected:
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
	static CWeapon* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END