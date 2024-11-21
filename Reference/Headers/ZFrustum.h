
#include "Base.h"

BEGIN(Engine)
class ZFrustum_Manager final : public CBase
{
	DECLARE_SINGLETON(ZFrustum_Manager)
public:
	enum ZFrustumSate { WAIT_INIT, COMPLETE_CAMERA, COMPLETE_GAPHIC_DEVICE };
	ZFrustumSate Get_ZFrustumSate() {
		return m_eFrustumState;
	}
private:
	ZFrustumSate m_eFrustumState = WAIT_INIT;
private:
	D3DXVECTOR3     m_vtx[8];       /// 프러스텀을 구성할 정점 8개
	D3DXVECTOR3     m_vPos;         /// 현재 카메라의 월드좌표
	D3DXPLANE       m_plane[6];     /// 프러스텀을 구성하는 6개의 평면
public:
	ZFrustum_Manager();
	virtual ~ZFrustum_Manager() = default;

public:
	void	Tick();
	HRESULT Render();

	_bool    IsIn(_float3* pv);

	BOOL IsInSphere(D3DXVECTOR3* pv, float radius);
	/// 현재 카메라의 월드좌표를 얻어준다.
	D3DXVECTOR3*    GetPos() { return &m_vPos; }
public:
	void Set_Camera(_float4x4* pArg);
	void Set_Graphic_Device(LPDIRECT3DDEVICE9 _pGraphicDevice){
		m_eFrustumState = COMPLETE_GAPHIC_DEVICE;
		m_pGraphic_Device = _pGraphicDevice;
	}
private:
	_float4x4* m_pMatViewProj = nullptr;
	LPDIRECT3DDEVICE9 m_pGraphic_Device = nullptr;
public:
	virtual void Free() override {};
};
END
