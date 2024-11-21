#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Ani_Cube final : public CVIBuffer
{
private :
	ANI_CUBE m_eBody = ANI_CUBE_END;
public:
	CVIBuffer_Ani_Cube(LPDIRECT3DDEVICE9 pGraphicDevice);
	CVIBuffer_Ani_Cube(const CVIBuffer_Ani_Cube& rhs);
	virtual ~CVIBuffer_Ani_Cube() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render(_float4x4 worldmatrix);
public:
	void Set_VerticesWidth(void* pArg);
	void Set_VerticesHeight(void* pArg);
public:
	STSHAPE m_stShape;
	CString	m_ImagePath;
	STSHAPE* Get_ShapeInfo()
	{
		return &m_stShape;
	}
protected:
	VERTEXTXT* pVertices = nullptr;
	FACEINDICES16* pIndices = nullptr;
//	CTexture*		m_pTextureCom = nullptr;
private :
	HRESULT Set_Vertices(_float width, _float topheight, _float bottom, _float pos);
	HRESULT Set_Component();
	HRESULT Setup_RenderState();
	HRESULT Rlease_RenderState();
	IDirect3DTexture9* m_pTexture = NULL;
	tagAniVertex m_Vetices[8];
private :
	HRESULT    Set_Head_Vertex();
	HRESULT    Set_Body_Vertex();
	HRESULT    Set_Leg_Vertex();
	HRESULT    Set_Arm_Vertex();
	HRESULT	   Set_Ankle_Vertex();
	HRESULT	   Set_Elbow_Vertex();
	HRESULT	   Set_Default_Vertex();
public:
	static CVIBuffer_Ani_Cube* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};
END