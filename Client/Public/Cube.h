#pragma once
//#include "Client_Define.h"
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CVIBuffer_ColorCube;
END


class CCube final : public CGameObject
{
public:
	explicit CCube(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CCube(const CCube& pOrigin);
	virtual ~CCube() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(float fTimeDelta) override;
	virtual void Late_Tick(float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void TurnCube(D3DXVECTOR3 vAxis, float fTimeDelta);
	void Set_Position(D3DXVECTOR3 vPos);
	void Move_Position(D3DXVECTOR3 vVec);
	void Set_Size(const D3DXVECTOR3& vSize);
	//마스터큐브의 사이즈에 따른 하위큐브의 사이즈 설정
	void Set_WorldSize(D3DXVECTOR3 vMasterCubeSize, D3DXVECTOR3 vCubeSize);

	//마스터큐브의 중점이동에 따른 하위큐브의 버텍스 이동
	void Set_VertexPos(D3DXVECTOR3 vVertexPos, D3DXVECTOR3 vMasterCubeSize);

	//큐브의 모습 보이기 설정
	void Set_CubeRender(bool bRender) { m_bCubeRender = bRender; }

	//트랜스폼 바인딩
	void Set_BindTransform(CTransform* pTransform);

public:
	CUBE Get_CubeInfo(void) { return m_eCube; }

private:
	CTransform* m_pTransformComponent = nullptr;
	CRenderer* m_pRendererComponent = nullptr;
	CVIBuffer_ColorCube* m_pVIBufferComponent = nullptr;

private:
	int m_iIndex = 0;
	D3DXVECTOR3 m_vPosition;
	bool m_bCubeRender = true;

private:
	HRESULT Set_Component();
	CUBE m_eCube;

public:
	static CCube* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CCube* Clone(void* pArg) override;
	virtual void Free() override;
};
