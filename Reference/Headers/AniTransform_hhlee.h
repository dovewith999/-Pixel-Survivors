#pragma once

#include "Component.h"
//20220512_수정
BEGIN(Engine)
class ENGINE_DLL  CAniTransform final : public CComponent
{
public:
	enum ANI_TRANS_INFO { ANI_TRANS_RIGHT, ANI_TRANS_UP, ANI_TRANS_LOOK, ANI_TRANS_POSITION, ANI_TRANS_END };
private:
	CAniTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAniTransform(const CAniTransform& rhs);
	virtual ~CAniTransform() = default;

public:
	//엔진으로 코드 옮길 것
	void Init_LocalAngle()
	{
		m_fAngle = m_TransformDesc.fLocalAngle;
	}

	_float3 Get_State(ANI_TRANS_INFO eState) {
		return *(_float3 *)&m_TransformDesc.WorldMatrix.m[eState][0];
	}

	_float3 Get_LocalState(ANI_TRANS_INFO eState) {
		return *(_float3 *)&m_TransformDesc.LocalMatrix.m[eState][0];
	}

	_float4x4 Get_LocalMatrixInverse() {
		_float4x4		LocalMatrix;
		return *D3DXMatrixInverse(&LocalMatrix, nullptr, &m_TransformDesc.LocalMatrix);
	}

	_float4x4 Get_WorldMatrixInverse() {
		_float4x4		WorldMatrix;
		return *D3DXMatrixInverse(&WorldMatrix, nullptr, &m_TransformDesc.WorldMatrix);
	}

	void Set_State(ANI_TRANS_INFO eState, D3DXVECTOR3& vState) {
		memcpy(&m_TransformDesc.WorldMatrix.m[eState][0], &vState, sizeof(D3DXVECTOR3));

	}

	ANIMATIONDESC Get_TansDesc();

	_float4x4 Get_WorldMatrix() {
		return m_TransformDesc.WorldMatrix;
	}

	_float4x4 Get_BindMatrix() {
		return m_BindLocalMatrix * m_BindWorldMatrix;
	}

	void Set_LocalState(ANI_TRANS_INFO eState, const D3DXVECTOR3& vState) {
		memcpy(&m_TransformDesc.LocalMatrix.m[eState][0], &vState, sizeof(D3DXVECTOR3));
	}

	void Set_TransformDesc(const ANIMATIONDESC& TransformDesc) {
		m_TransformDesc = TransformDesc;
	}

	void Set_State(ANI_TRANS_INFO eState, D3DXVECTOR3 vState, _float fDeltatime) {
		vState.x = vState.x * fDeltatime;
		vState.y = vState.y * fDeltatime;
		vState.z = vState.z * fDeltatime;
		memcpy(&m_TransformDesc.WorldMatrix.m[eState][0], &vState, sizeof(D3DXVECTOR3));
	}
	void Bind_Parent(CAniTransform& Rhs, TCHAR* pTag)
	{
		m_pTag = pTag;

		if (!lstrcmp(pTag, KEY_BONE_PELVIS))
		{
			m_pParentLocalMatrix = &(Rhs.m_TransformDesc.LocalMatrix);
			m_pParentWorldMatrix = &(Rhs.m_TransformDesc.WorldMatrix);

		}
		else
		{
			m_pParentLocalMatrix = &(Rhs.m_BindLocalMatrix);
			m_pParentWorldMatrix = &(Rhs.m_BindWorldMatrix);
		}
	}

	_float3 Get_Scale() {
		return _float3(D3DXVec3Length(&Get_State(ANI_TRANS_RIGHT)),
			D3DXVec3Length(&Get_State(ANI_TRANS_UP)),
			D3DXVec3Length(&Get_State(ANI_TRANS_LOOK)));
	}

	_float3 Get_LocalScale() {
		return _float3(D3DXVec3Length(&Get_LocalState(ANI_TRANS_RIGHT)),
			D3DXVec3Length(&Get_LocalState(ANI_TRANS_UP)),
			D3DXVec3Length(&Get_LocalState(ANI_TRANS_LOOK)));
	}
public:
	_float Get_MaxAngle()
	{
		return m_TransformDesc.fMaxAngle;
	}
	_float Get_LocalAngle()
	{
		return m_TransformDesc.fLocalAngle;
	}
	_float3 Get_State(ANI_TRANS_INFO eState, _float4x4 matrix) {
		return *(_float3 *)&matrix.m[eState][0];
	}

	void Set_State(ANI_TRANS_INFO eState, const D3DXVECTOR3& vState) {
		memcpy(&m_TransformDesc.WorldMatrix.m[eState][0], &vState, sizeof(D3DXVECTOR3));
	}

	void Set_State(ANI_TRANS_INFO eState, _float4x4& matrix, const D3DXVECTOR3& vState) {
		memcpy(&matrix.m[eState][0], &vState, sizeof(D3DXVECTOR3));
	}

#ifdef _DEBUG
	const TCHAR* Get_Tag()
	{
		return m_pTag;
	}
#endif
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
	_float Turn_Local(_float3& vAxis, _float fTimeDelta);
	HRESULT LookAt(const _float3& vTargetPos);
	HRESULT LookAt(const _float3 & vTargetPos, _float3 vAxis);
	HRESULT Chase(const _float3& vTargetPos, _float fTimeDelta, _float fLimit = 1.f);
	HRESULT TurnAngle(const _float3 & vAxis, float fAngle);
public:
	HRESULT Bind_OnGraphicDevice();
private:
	_float3 Set_MatrixPos(ANI_TRANS_INFO eState, _float4x4 matrix, _float3 vPos);
	void Turn_Bone(_float3 & vAxis, _float fTimeDelta, _float fAngle);
private:
	_bool				m_bCamearaTrans = false;
	_float3				m_vTargetCam;
	_float				m_fAngle = 0.f;
	_float				m_fDir = 1.f;
private:
	_float4x4			m_BindWorldMatrix;
	_float4x4			m_BindLocalMatrix;
private:
	_float4x4			*m_pParentWorldMatrix = NULL;
	_float4x4			*m_pParentLocalMatrix = NULL;
	ANIMATIONDESC		m_TransformDesc;
	TCHAR*				m_pTag = NULL;
public:
	static CAniTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};
END