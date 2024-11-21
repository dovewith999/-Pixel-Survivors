#pragma once

#include "Component.h"

/* 객체의 월드 상태를 정의한다.(행렬) */


BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		_float		fSpeedPerSec = 0.f;
		_float		fRotationPerSec = 0.f;		
	}TRANSFORMDESC;

private:
	CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_float3 Get_State(STATE eState) {
 
		return *(_float3 *)&m_WorldMatrix.m[eState][0];
	}

	_float4x4 Get_WorldMatrixInverse() {
		_float4x4		WorldMatrix;
		return *D3DXMatrixInverse(&WorldMatrix, nullptr, &m_WorldMatrix);
	}

	_float4x4 Get_WorldMatrix() {
		return m_WorldMatrix;
	}

	void Set_State(STATE eState, const D3DXVECTOR3& vState) {
		memcpy(&m_WorldMatrix.m[eState][0], &vState, sizeof(D3DXVECTOR3));
	}

	void Set_TransformDesc(const TRANSFORMDESC& TransformDesc) {
		m_TransformDesc = TransformDesc;
	}

	void Set_Scaled(const _float3& vScale);

	_float3 Get_Scale() {
		return _float3(D3DXVec3Length(&Get_State(STATE_RIGHT)),
			D3DXVec3Length(&Get_State(STATE_UP)),
			D3DXVec3Length(&Get_State(STATE_LOOK)));
	}

	void Set_Scale(const D3DXVECTOR3& vScale) {

		D3DXVECTOR3 vRight = Get_State(STATE_RIGHT);
		D3DXVECTOR3 vUp = Get_State(STATE_UP);
		D3DXVECTOR3 vLook = Get_State(STATE_LOOK);

		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);
		D3DXVec3Normalize(&vLook, &vLook);

		vRight *= vScale.x;
		vUp *= vScale.y;
		vLook *= vScale.z;

		Set_State(CTransform::STATE_RIGHT, vRight);
		Set_State(CTransform::STATE_UP, vUp);
		Set_State(CTransform::STATE_LOOK, vLook);
	}

	void Set_Scale(const float& fSize) {

		D3DXVECTOR3 vRight = Get_State(STATE_RIGHT);
		D3DXVECTOR3 vUp = Get_State(STATE_UP);
		D3DXVECTOR3 vLook = Get_State(STATE_LOOK);

		vRight *= fSize;
		vUp *= fSize;
		vLook *= fSize;

		Set_State(CTransform::STATE_RIGHT, vRight);
		Set_State(CTransform::STATE_UP, vUp);
		Set_State(CTransform::STATE_LOOK, vLook);
	}

	void Set_ScalePlus(const D3DXVECTOR3& vScale) {

		D3DXVECTOR3 vRight = Get_State(STATE_RIGHT);
		D3DXVECTOR3 vUp = Get_State(STATE_UP);
		D3DXVECTOR3 vLook = Get_State(STATE_LOOK);

		vRight *= 1.f + vScale.x;
		vUp *= 1.f + vScale.y;
		vLook *= 1.f + vScale.z;

		Set_State(CTransform::STATE_RIGHT, vRight);
		Set_State(CTransform::STATE_UP, vUp);
		Set_State(CTransform::STATE_LOOK, vLook);
	}
	void Set_State(STATE eState, D3DXVECTOR3 vState, _float fDeltatime) {
		vState.x = vState.x * fDeltatime;
		vState.y = vState.y * fDeltatime;
		vState.z = vState.z * fDeltatime;
		memcpy(&m_WorldMatrix.m[eState][0], &vState, sizeof(D3DXVECTOR3));
	}
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Go_Straight(_float fTimeDelta);
	HRESULT Go_Left(_float fTimeDelta);
	HRESULT Go_Right(_float fTimeDelta);
	HRESULT Go_Backward(_float fTimeDelta);
	HRESULT Rotation(const _float3& vAxis, _float fRadian);
	HRESULT Turn(const _float3& vAxis, _float fTimeDelta);
	HRESULT TurnAngle(const _float3& vAxis, float fAngle);
	HRESULT LookAt(const _float3& vTargetPos);
	HRESULT Chase(const _float3& vTargetPos, _float fTimeDelta, _float fLimit = 1.f);

	HRESULT Go_Straight_OnTerrain(_float fTimeDelta);
	HRESULT Go_Backward_OnTerrain(_float fTimeDelta);

	HRESULT Go_Direction(const _float3& vDir, _float fSpeed, _float fTimeDelta);

public:
	HRESULT Bind_OnGraphicDevice();

private:
	_float4x4			m_WorldMatrix;
	TRANSFORMDESC		m_TransformDesc;

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;

};

END