#pragma once

#include "Client_Defines.h"
#include "LandObject.h"
#include "MasterCube.h"
#include "UI_HPBar.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CCollider;
class CCollision;
class CBattle;
class CShader;
END

BEGIN(Client)
//class MyTerrain;
class CCamera_Player;
class CUI_IconBox;
class CUI_Inventory;
class CPlayer final : public CLandObject
{

public:
	// 0511
	typedef struct tagPlayerState {
		_uint iLevel = 1;
		_uint iExp = 0;
		_uint iGold = 0;
	}PLAYERSTATE;

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override; // 04/26 임희섭 수우정
	virtual HRESULT Render() override;

public:
	HRESULT Set_Turn(D3DXVECTOR3 vAxis, float fTimeDelta);
	void Set_CubeRender(bool m_bRender) {
		if (nullptr != m_pMasterCube)
			m_pMasterCube->Set_CubeRender(m_bRender);
	}
	HRESULT Create_Attack_Collision(ATTACK_COLLISION* Atk_Col);
	void ResetCoolTime(WEAPONLIST eWEAPON);

public:
	CTransform* Get_PlayerTransform() { return m_pTransformCom; }
	_bool Get_LevelUp() const { return m_bLevelUp; }
	_float Get_Exp() const { return m_iPlayerExp; }
	_uint Get_Level() const { return m_iLevel; }
	_bool Get_ItemBox() const { return m_bGetItemBox; }
	_bool Get_SameItem() {

		if (m_iIconBox[0] == m_iIconBox[1] || m_iIconBox[0] == m_iIconBox[2] || m_iIconBox[1] == m_iIconBox[2])
		{
			return true;
		}

		return false;
	}

public:
	HRESULT Set_MasterCube(CMasterCube* MasterCube);
	HRESULT Set_HPBar(CUI_HPBar* HP_Bar);
	void Set_OnShot(_bool _Result) { m_bShot = _Result; }
	void Get_TurnVector(D3DXVECTOR3 vAxis, _float3 &vRight, _float3 &vUp, _float3 &vLook, float fAngle);
	void Set_SelectItem(_uint _SelectItem) { m_iSelectitem = _SelectItem; }

public:
	void Emplace_ActiveMap(WEAPONLIST _WeaponType); // 05/11 임희섭 추우가
	int Get_ItemLevel(WEAPONLIST _WeaponName)
	{
		for (auto pair : m_Active)
		{
			if (_WeaponName == pair.first)
			{
				return pair.second;
			}
		}
		return 0;
	}

	void	Set_IconBox(int _BoxPos, _uint _WeaponType) { m_iIconBox[_BoxPos] = _WeaponType; }

public:
	// 0511
	_float3 Get_Position();
	_float3 Get_Look();

public:
	// 0511
	_uint Get_Exp() { return m_tPlayerState.iExp; }
	void Set_Exp(_uint iExp) { m_tPlayerState.iExp = iExp; }

private:
	PLAYERSTATE				m_tPlayerState; // 0511

private:
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CBattle*				m_pBattleCom = nullptr;
	CShader*				m_pShaderCom = nullptr;

	CCollider*				m_pColliderCom = nullptr;
	CCollision*				m_pCollisionCom = nullptr;
	CUI_IconBox*			m_pIconBox = nullptr;
	CUI_Inventory*			m_pInventory = nullptr;

	_bool					m_bOnScope = false; // 05/02 임희섭 추우가
	_bool					m_bZoom = false; // 05/03 임희섭 추우가
	_bool					m_bShot = false; // 05/04 임희섭 추우가
	_uint					m_iLevel = 0; // 05/09 임희섭 추우가
	_bool					m_bLevelUp = false; // 05/08 임희섭 추우가
	_float					m_iPlayerExp = 0.f; // 05/08 임희섭 추우가
	_uint					m_iSelectitem = 0;
	_bool					m_bGetItemBox = false; // 05/12 임희섭 추우가
	_uint					m_iIconBox[3] = {0};

private:
	void CreateWeapon(int iActiveSlot, WEAPONLIST eWeapon, int iLevel);

private:
	float	m_fPower = 1.f;			//공격력증가
	int		m_iArmor = 0;			//대미지감소
	//float	m_fRecovery = 0.1f;		//체력회복력
	float	m_fCoolTime = 1.f;		//쿨타임 감소
	float	m_fArea = 1.f;			//공격범위 증가
	float	m_fSpeed = 1.f;			//투사체 속도 증가
	float	m_fDuration = 1.f;		//지속시간 증가
	int		m_iAmount = 0;			//투사체 수 증가
	float	m_fMoveSpeed = 0.f;		//이동속도증가
	float	m_fMagnet = 1.f;		//아이템흭득범위
	//float	m_fLuck = 1.f;			//행운
	//float	m_fGrowth = 1.f;		//받는 경험치 증가

	float m_fFor_Lightning_Angle = 0.f;
	D3DXVECTOR3 m_fFor_Lightning_Pos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_fFor_Lightning_Look = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_fFor_Lightning_Right = D3DXVECTOR3(0.f, 0.f, 0.f);

	float m_fActive_1_Cooltime = 0.f;
	float m_fActive_2_Cooltime = 0.f;
	float m_fActive_3_Cooltime = 0.f;
	float m_fActive_4_Cooltime = 0.f;
	float m_fActive_5_Cooltime = 0.f;
	float m_fActive_6_Cooltime = 0.f;

	//고양이 움직이는 효과?
	bool m_bTurnLR = false;
	float m_fTurnAngle = 0.f;

	// 무기 강제 레벨업 쿨타임 생성을 위한 변수
	float m_fWeaponLevelCool = 0.f;

private:
	CMasterCube*			m_pMasterCube = nullptr;
	CUI_HPBar*				m_pHPBar = nullptr;
	CCamera_Player*			m_pCamera_Player = nullptr;

private:
	//키값 액티브 종류 밸류 레벨
	//액티브 종류와 레벨을 알면 쿨타임을 알수있고 무기를 발사나 생성할때 1.시작위치 2.공격방향 3.아이템레벨 4.플레이어의 공격력(증가하는 대미지)을 건네준다
	//액티브 무기 내에서 자기 공격력을 결정한다.
	map<WEAPONLIST, int> m_Active;
	map<PASSIVELIST, int> m_Passive;

	list<WEAPONLIST> m_ActiveList;

	vector<WEAPONSTATS> m_Inventory;

public:
	void Push_Back_Inventory(WEAPONSTATS _WeaponStats) {
		m_Inventory.push_back(_WeaponStats);
	}
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END