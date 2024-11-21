#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CPlayer;
class CUI_PlayerStats final : public CUIObject
{
public:
	explicit CUI_PlayerStats(LPDIRECT3DDEVICE9 _GraphicDevice);
	explicit CUI_PlayerStats(const CUI_PlayerStats& rhs);
	virtual ~CUI_PlayerStats() override = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	LPD3DXSPRITE			m_pSprite = nullptr;
	LPD3DXFONT				m_pFont = nullptr;
private:
	CPlayer* m_pPlayer = nullptr;

	float	m_fPower = 1.f;			//���ݷ�����
	int		m_iArmor = 0;			//���������
	float	m_fRecovery = 0.1f;		//ü��ȸ����
	float	m_fCoolTime = 1.f;		//��Ÿ�� ����
	float	m_fArea = 1.f;			//���ݹ��� ����
	float	m_fSpeed = 1.f;			//����ü �ӵ� ����
	float	m_fDuration = 1.f;		//���ӽð� ����
	int		m_iAmount = 0;			//����ü �� ����
	float	m_fMoveSpeed = 0.f;		//�̵��ӵ�����
	float	m_fMagnet = 1.f;		//������ŉ�����
	float	m_fLuck = 1.f;			//��� 
	float	m_fGrowth = 1.f;		//�޴� ����ġ ����

private:
	_uint					m_iTextureNum = 0;
	_float					m_fFrame = 0.f;
	_float4x4				m_ProjMatrix;
	_float					m_fSizeX, m_fSizeY;
	_float2					m_vPos; // m_fX, m_fY
	_float					m_iPlayerExp = 0.f;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CUI_PlayerStats* Create(LPDIRECT3DDEVICE9 _GraphicDevice);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

