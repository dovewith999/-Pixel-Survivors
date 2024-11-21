#include "stdafx.h"
#include "..\Public\Camera_Player.h"
#include "GameInstance.h"

CCamera_Player::CCamera_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{

}

CCamera_Player::CCamera_Player(const CCamera_Player & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Player::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Player::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	m_pTransform->TurnAngle(m_pTransform->Get_State(CTransform::STATE_RIGHT), -10.f);
	//ShowCursor(FALSE);

	return S_OK;
}

void CCamera_Player::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();

	if (nullptr == pGameInstance)
		return;

	Safe_AddRef(pGameInstance);

	_long		MouseMove = 0;

	if (GetAsyncKeyState(VK_RBUTTON) < 0)
	{
		if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_X))
		{
			m_pTransform->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * MouseMove * 0.2f);
			m_pPlayer->Set_Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * MouseMove * 0.2f);
		}
	}

	if (nullptr != m_pPlayer)
	{
		if (m_fDistance > -0.5f)
			m_pPlayer->Set_CubeRender(false);
		else
			m_pPlayer->Set_CubeRender(true);
	}

	

	m_pTransform->Set_State(CTransform::STATE_POSITION, m_pPlayer->Get_PlayerTransform()->Get_State(CTransform::STATE_POSITION));

	if (m_fDistance >= -0.001f)
		m_fDistance = -0.001f;

	D3DXVECTOR3 vCameraPlus = m_pPlayer->Get_PlayerTransform()->Get_State(CTransform::STATE_LOOK) * 3.f;

	m_pTransform->Set_State(CTransform::STATE_POSITION, m_pTransform->Get_State(CTransform::STATE_POSITION) - vCameraPlus + (m_pTransform->Get_State(CTransform::STATE_LOOK) * m_fDistance));

	Safe_Release(pGameInstance);


}

void CCamera_Player::Late_Tick(_float fTimeDelta)
{

	if (FAILED(__super::Bind_TransformMatrices()))
		return;
}

HRESULT CCamera_Player::Render()
{
	return S_OK;
}

HRESULT CCamera_Player::Set_Player(CPlayer* pPlayer)
{
	if (nullptr == pPlayer)
		return E_FAIL;

	m_pPlayer = pPlayer;

	return S_OK;
}

CCamera_Player * CCamera_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Player*	pInstance = new CCamera_Player(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CCamera_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Player::Clone(void * pArg)
{
	CCamera_Player*	pInstance = new CCamera_Player(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Created : CCamera_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Player::Free()
{
	__super::Free();

	Safe_Release(m_pPlayer);

	
}
