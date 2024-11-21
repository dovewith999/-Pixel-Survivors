#pragma once
#include "Client_Defines.h"
#include "Weapon.h"

BEGIN(Client)
class CSword final : public CWeapon
{
public:
	explicit CSword(LPDIRECT3DDEVICE9 _GraphicDevice);
	explicit CSword(const CSword& rhs);
	virtual ~CSword() override = default;

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

	virtual void Set_CameraTransform(CTransform* pTransform) {
		m_pCameraTransform = pTransform;
	}

	virtual void Set_Show(bool Show) { m_bShow = Show; }

public:
	virtual D3DXVECTOR3 Get_PlayerPos() { return m_pTransformCom->Get_State(CTransform::STATE_POSITION); }
	virtual WEAPONSTATS Get_WeaponStats() const { return m_tagWeaponStats; } // 04/28 ¿”»Òº∑ √ﬂøÏ∞°

//protected:
//	CTransform*				m_pTransformCom = nullptr;
//	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
//	CRenderer*				m_pRendererCom = nullptr;
public:
	virtual HRESULT Render_Cube() override;

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
	static CSword* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

