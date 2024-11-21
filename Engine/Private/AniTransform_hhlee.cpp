#include "..\Public\AniTransform_hhlee.h"

CAniTransform::CAniTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CAniTransform::CAniTransform(const CAniTransform & rhs)
	: CComponent(rhs)
	, m_pParentLocalMatrix(rhs.m_pParentLocalMatrix)
	, m_pParentWorldMatrix(rhs.m_pParentWorldMatrix)
	, m_BindLocalMatrix(rhs.m_BindLocalMatrix)
	, m_BindWorldMatrix(rhs.m_BindWorldMatrix)
	, m_fAngle(rhs.m_fAngle)
	, m_fDir(rhs.m_fDir)
	, m_pTag(rhs.m_pTag)
{
	m_TransformDesc.LocalMatrix = rhs.m_TransformDesc.LocalMatrix;
	m_TransformDesc.WorldMatrix = rhs.m_TransformDesc.WorldMatrix;
}

HRESULT CAniTransform::NativeConstruct_Prototype()
{
	D3DXMatrixIdentity(&m_TransformDesc.WorldMatrix);
	D3DXMatrixIdentity(&m_TransformDesc.LocalMatrix);
	D3DXMatrixIdentity(&m_BindLocalMatrix);
	D3DXMatrixIdentity(&m_BindWorldMatrix);
	m_pParentLocalMatrix = NULL;
	m_pParentWorldMatrix = NULL;

	return S_OK;
}

HRESULT CAniTransform::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
	{
		ANIMATIONDESC st = *(ANIMATIONDESC*)pArg;
		memcpy(&m_TransformDesc, pArg, sizeof(ANIMATIONDESC));
		//로컬 월드 매트릭스 멤버 변수 삭제
		if (0 == m_TransformDesc.fDir)
			m_TransformDesc.fDir = 1.f;

		if (!lstrcmp(KEY_BONE_ROOT, m_TransformDesc.szKey))
		{
			m_pParentLocalMatrix = new _float4x4;
			m_pParentWorldMatrix = new _float4x4;
			D3DXMatrixIdentity(m_pParentLocalMatrix);
			D3DXMatrixIdentity(m_pParentWorldMatrix);
		}
	}

	return S_OK;
}

HRESULT CAniTransform::Go_Straight(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CAniTransform::ANI_TRANS_POSITION);
	_float3		vLook = Get_State(CAniTransform::ANI_TRANS_LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CAniTransform::ANI_TRANS_POSITION, vPosition);

	return S_OK;
}

HRESULT CAniTransform::Go_Left(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CAniTransform::ANI_TRANS_POSITION);
	_float3		vRight = Get_State(CAniTransform::ANI_TRANS_RIGHT);

	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CAniTransform::ANI_TRANS_POSITION, vPosition);

	return S_OK;
}

HRESULT CAniTransform::Go_Right(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CAniTransform::ANI_TRANS_POSITION);
	_float3		vRight = Get_State(CAniTransform::ANI_TRANS_RIGHT);

	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CAniTransform::ANI_TRANS_POSITION, vPosition);

	return S_OK;
}
#include <iostream>
using namespace std;

HRESULT CAniTransform::Go_Backward(_float fTimeDelta)
{
	_float3		vPosition = Get_State(CAniTransform::ANI_TRANS_POSITION);
	_float3		vLook = Get_State(CAniTransform::ANI_TRANS_LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CAniTransform::ANI_TRANS_POSITION, vPosition);

	return S_OK;
}

HRESULT CAniTransform::Rotation(const _float3 & vAxis, _float fRadian)
{
	_float3		vScaleDesc = Get_Scale();

	_float3		vRight = _float3(1.f, 0.f, 0.f) * vScaleDesc.x;
	_float3		vUp = _float3(0.f, 1.f, 0.f) * vScaleDesc.y;
	_float3		vLook = _float3(0.f, 0.f, 1.f) * vScaleDesc.z;

	_float4x4	RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CAniTransform::ANI_TRANS_RIGHT, vRight);
	Set_State(CAniTransform::ANI_TRANS_UP, vUp);
	Set_State(CAniTransform::ANI_TRANS_LOOK, vLook);

	return S_OK;
}

HRESULT CAniTransform::Turn(const _float3 & vAxis, _float fTimeDelta)
{
	_float3		vRight = Get_State(CAniTransform::ANI_TRANS_RIGHT);
	_float3		vUp = Get_State(CAniTransform::ANI_TRANS_UP);
	_float3		vLook = Get_State(CAniTransform::ANI_TRANS_LOOK);

	_float4x4	RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_TransformDesc.fRotationPerSec * fTimeDelta);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CAniTransform::ANI_TRANS_RIGHT, vRight);
	Set_State(CAniTransform::ANI_TRANS_UP, vUp);
	Set_State(CAniTransform::ANI_TRANS_LOOK, vLook);
	return S_OK;
}

HRESULT CAniTransform::TurnAngle(const _float3 & vAxis, float fAngle)
{
	_float3		vRight = Get_State(CAniTransform::ANI_TRANS_RIGHT);
	_float3		vUp = Get_State(CAniTransform::ANI_TRANS_UP);
	_float3		vLook = Get_State(CAniTransform::ANI_TRANS_LOOK);

	_float4x4	RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, D3DXToRadian(fAngle));

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CAniTransform::ANI_TRANS_RIGHT, vRight);
	Set_State(CAniTransform::ANI_TRANS_UP, vUp);
	Set_State(CAniTransform::ANI_TRANS_LOOK, vLook);

	return S_OK;
}

_float CAniTransform::Turn_Local(_float3 & vAxis, _float fTimeDelta)
{

	if (3.141592 * 2 < fabsf(m_fAngle))
		m_fAngle = 0.f;

	if (0 < m_TransformDesc.fMaxAngle - m_TransformDesc.fLocalAngle)
	{
		if (m_TransformDesc.fLocalAngle > m_fAngle)
		{
			m_TransformDesc.fDir = 1.f;
		}

		if (m_TransformDesc.fMaxAngle < m_fAngle)
		{
			m_TransformDesc.fDir = -1.f;
		}
	}

	if (0 > m_TransformDesc.fMaxAngle - m_TransformDesc.fLocalAngle)
	{
		if (m_TransformDesc.fLocalAngle < m_fAngle)
		{
			m_TransformDesc.fDir = -1.f;
		}

		if (m_TransformDesc.fMaxAngle > m_fAngle)
		{
			m_TransformDesc.fDir = 1.f;
		}
	}

	if (0 == m_TransformDesc.fMaxAngle - m_TransformDesc.fLocalAngle)
	{
		m_fAngle = m_TransformDesc.fMaxAngle;
	}
	//hhlee
	m_fAngle += fTimeDelta * m_TransformDesc.fDir * m_TransformDesc.fRotationPerSec;

	_float3		vRight = _float3{ 1.f, 0.f, 0.f };
	_float3		vUp = _float3{ 0.f, 1.f, 0.f };
	_float3		vLook = _float3{ 0.f, 0.f, 1.f };

	_float4x4	RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_fAngle);

	//_float3 worldPos = Set_MatrixPos(STATE::STATE_POSITION, m_TransformDesc.WorldMatrix, _float3{ 0.f, 0.f, 0.f });

	if (0 != m_TransformDesc.fMaxAngle || 0 != m_TransformDesc.fLocalAngle)
	{
		D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
		Set_State(ANI_TRANS_RIGHT, vRight);
		D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
		Set_State(ANI_TRANS_UP, vUp);
		D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);
		Set_State(ANI_TRANS_LOOK, vLook);
	}
	return m_fAngle;
}


HRESULT CAniTransform::LookAt(const _float3 & vTargetPos)
{
	_float3		vPosition = Get_State(CAniTransform::ANI_TRANS_POSITION);
	_float3		vLook = vTargetPos - vPosition;
	_float3		vRight;
	_float3		vUp;

	D3DXVec3Cross(&vRight, &_float3(0.f, 1.f, 0.f), &vLook);
	D3DXVec3Cross(&vUp, &vLook, &vRight);

	_float3		vScaleDesc = Get_Scale();

	Set_State(CAniTransform::ANI_TRANS_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScaleDesc.x);
	Set_State(CAniTransform::ANI_TRANS_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScaleDesc.y);
	Set_State(CAniTransform::ANI_TRANS_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScaleDesc.z);

	return S_OK;
}

HRESULT CAniTransform::LookAt(const _float3 & vTargetPos, _float3 vAxis)
{
	_float3		vPosition = Get_State(CAniTransform::ANI_TRANS_POSITION);
	_float3		vLook = vTargetPos - vPosition;
	_float3		vRight;
	_float3		vUp;

	D3DXVec3Cross(&vRight, &vAxis, &vLook);
	D3DXVec3Cross(&vUp, &vLook, &vRight);

	_float3		vScaleDesc = Get_Scale();

	Set_State(CAniTransform::ANI_TRANS_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScaleDesc.x);
	Set_State(CAniTransform::ANI_TRANS_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScaleDesc.y);
	Set_State(CAniTransform::ANI_TRANS_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScaleDesc.z);

	return S_OK;
}

HRESULT CAniTransform::Chase(const _float3 & vTargetPos, _float fTimeDelta, _float fLimit)
{
	LookAt(vTargetPos);

	_float fDistance = D3DXVec3Length(&(vTargetPos - Get_State(CAniTransform::ANI_TRANS_POSITION)));

	if (fDistance > fLimit)
		Go_Straight(fTimeDelta);

	return S_OK;
}

HRESULT CAniTransform::Bind_OnGraphicDevice()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	_float4x4 World;
	//부모행렬의 경우 Tag이름을 설정하지 않는다.
	if (0 < lstrlen(m_TransformDesc.szKey))
	{
		if (!lstrcmp(m_TransformDesc.szKey, KEY_BONE_ROOT))
			World = m_TransformDesc.LocalMatrix * m_TransformDesc.WorldMatrix;
		else
		{
			D3DXMatrixIdentity(&m_BindLocalMatrix);
			D3DXMatrixIdentity(&m_BindWorldMatrix);

			m_BindLocalMatrix = m_TransformDesc.LocalMatrix * m_pParentLocalMatrix[0];
			m_BindWorldMatrix = m_TransformDesc.WorldMatrix * m_pParentWorldMatrix[0];
			World = m_TransformDesc.LocalMatrix * m_pParentLocalMatrix[0] * m_TransformDesc.WorldMatrix *  m_pParentWorldMatrix[0];

			_float3 bindlocalPos = Set_MatrixPos(ANI_TRANS_POSITION, m_pParentLocalMatrix[0], _float3{ 0.f, 0.f, 0.f });
			_float3 localPos = Set_MatrixPos(ANI_TRANS_POSITION, m_TransformDesc.LocalMatrix, _float3{ 0.f, 0.f, 0.f });

			World = m_TransformDesc.LocalMatrix * m_pParentLocalMatrix[0] * m_TransformDesc.WorldMatrix * m_pParentWorldMatrix[0];

			Set_MatrixPos(ANI_TRANS_POSITION, m_pParentLocalMatrix[0], bindlocalPos);
			Set_MatrixPos(ANI_TRANS_POSITION, m_TransformDesc.LocalMatrix, localPos);
		}
	}
	else
		World = m_TransformDesc.LocalMatrix * m_TransformDesc.WorldMatrix;

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &World);

	return S_OK;
}

void CAniTransform::Turn_Bone(_float3 & vAxis, _float fTimeDelta, _float fAngle)
{

	_float4x4   RotationMatrix;

	_float3 Sacle = Get_LocalScale();

	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fAngle);

	_float3		vRight = _float3{ 1.f, 0.f, 0.f };
	_float3		vUp = _float3{ 0.f, 1.f, 0.f };
	_float3		vLook = _float3{ 0.f, 0.f, 1.f };

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);
	Set_LocalState(ANI_TRANS_RIGHT, vRight * Sacle.x);
	Set_LocalState(ANI_TRANS_UP, vUp * Sacle.y);
	Set_LocalState(ANI_TRANS_LOOK, vLook * Sacle.z);

}

_float3 CAniTransform::Set_MatrixPos(ANI_TRANS_INFO eState, _float4x4 matrix, _float3 vPos)
{
	//행렬의 포지션 값을 가져옴
	_float3 position = Get_State(eState, matrix);

	Set_State(eState, matrix, vPos);

	return position;
}

CAniTransform * CAniTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAniTransform*	pInstance = new CAniTransform(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CAniTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CAniTransform::Clone(void * pArg)
{
	CAniTransform*	pInstance = new CAniTransform(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CAniTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAniTransform::Free()
{
	__super::Free();

	if (!lstrcmp(KEY_BONE_ROOT, m_TransformDesc.szKey))
	{
		Safe_Delete(m_pParentLocalMatrix);
		Safe_Delete(m_pParentWorldMatrix);
	}
}

ANIMATIONDESC CAniTransform::Get_TansDesc()
{
	return m_TransformDesc;
}
