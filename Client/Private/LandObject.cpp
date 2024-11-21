#include "stdafx.h"
#include "..\Public\LandObject.h"
#include "GameInstance.h"

CLandObject::CLandObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CLandObject::CLandObject(const CLandObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLandObject::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CLandObject::NativeConstruct(void * pArg)
{
	return S_OK;
}

void CLandObject::Tick(_float fTimeDelta)
{
}

void CLandObject::Late_Tick(_float fTimeDelta)
{
}

HRESULT CLandObject::Render()
{
	return S_OK;
}

HRESULT CLandObject::Set_Terrain(CTerrain* pTerrain)
{
	if (nullptr == pTerrain)
		return E_FAIL;

	m_pTerrain = pTerrain;

	return S_OK;
}

HRESULT CLandObject::SetUp_OnTerrain(CTransform * pTransform, _uint iLevelID, const _tchar* pLayerTag)
{
	_float3		vPosition = pTransform->Get_State(CTransform::STATE_POSITION);

	_int iHeight = m_pTerrain->Get_Height(vPosition);

	if (vPosition.y < iHeight + 0.5f &&
		(m_fJumpPower * m_fTimeAccumulate) - ((9.8f * m_fTimeAccumulate * m_fTimeAccumulate) * 0.5f) < 0)
	{
		m_bOnGravity = false;
		m_bRideTerrain = true;
		m_fTimeAccumulate = 0.f;
		m_fJumpPower = 0.f;
	}

	else if (vPosition.y > iHeight + 0.5f)
	{
		m_bOnGravity = true;
		//Safe_Release(pObject_Manager);
		return S_OK;
	}

	if (-1 == iHeight)
	{
		// TODO: ÁöÇü ¹ÛÀ¸·Î ¹þ¾î³µÀ» ¶§
		vPosition.y = 0.f;
		//MSGBOX(L"Failed : SetUp_OnTerrain");
		//return E_FAIL;
	}

	else
		vPosition.y = iHeight + 0.5f;

	pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CLandObject::SetUp_OnTerrain(CAniTransform * pTransform, _uint iLevelID, const _tchar* pLayerTag)
{
	_float3		vPosition = pTransform->Get_State(CAniTransform::ANI_TRANS_POSITION);

	_int iHeight = m_pTerrain->Get_Height(vPosition);

	if (vPosition.y < iHeight + 0.5f &&
		(m_fJumpPower * m_fTimeAccumulate) - ((9.8f * m_fTimeAccumulate * m_fTimeAccumulate) * 0.5f) < 0)
	{
		m_bOnGravity = false;
		m_bRideTerrain = true;
		m_fTimeAccumulate = 0.f;
		m_fJumpPower = 0.f;
	}

	else if (vPosition.y > iHeight)
	{
		m_bOnGravity = true;
		//Safe_Release(pObject_Manager);
		return S_OK;
	}

	if (-1 == iHeight)
	{
		// TODO: ÁöÇü ¹ÛÀ¸·Î ¹þ¾î³µÀ» ¶§
		vPosition.y = 0.f;
		//MSGBOX(L"Failed : SetUp_OnTerrain");
		//return E_FAIL;
	}

	else
		vPosition.y = iHeight + 0.5f;

	pTransform->Set_State(CAniTransform::ANI_TRANS_POSITION, vPosition);

	return S_OK;
}


HRESULT CLandObject::Affected_By_Gravity(CTransform * pTransform, float fTimeDelta)
{
	if (!m_bOnGravity)
		return S_OK;

	Safe_AddRef(pTransform);

	m_fTimeAccumulate += fTimeDelta;

	_float3		vPosition = pTransform->Get_State(CTransform::STATE_POSITION);

	float fY = ((m_fJumpPower * m_fTimeAccumulate) - ((9.8f * m_fTimeAccumulate * m_fTimeAccumulate) * 0.5f)) * 0.1f;

	if (fY > 0.2f)
		fY = 0.2f;
	else if (fY < -0.2f)
		fY = -0.2f;

	vPosition.y += fY;

	pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	Safe_Release(pTransform);

	return S_OK;
}

HRESULT CLandObject::Affected_By_Gravity(CAniTransform * pTransform, float fTimeDelta)
{
	if (!m_bOnGravity)
		return S_OK;

	Safe_AddRef(pTransform);

	m_fTimeAccumulate += fTimeDelta;

	_float3		vPosition = pTransform->Get_State(CAniTransform::ANI_TRANS_POSITION);

	float fY = ((m_fJumpPower * m_fTimeAccumulate) - ((9.8f * m_fTimeAccumulate * m_fTimeAccumulate) * 0.5f)) * 0.1f;

	if (fY > 0.2f)
		fY = 0.2f;
	else if (fY < -0.2f)
		fY = -0.2f;

	vPosition.y += fY;

	pTransform->Set_State(CAniTransform::ANI_TRANS_POSITION, vPosition);

	Safe_Release(pTransform);

	return S_OK;
}

void CLandObject::Free()
{
	__super::Free();
}
