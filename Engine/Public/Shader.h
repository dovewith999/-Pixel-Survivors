#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	CShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pShaderFilePath);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT SetUp_RawValue(D3DXHANDLE hParameters, void* pData, _uint iByteLength);
	HRESULT SetUp_Texture(D3DXHANDLE hParameter, LPDIRECT3DBASETEXTURE9 pTexture);

public:
	HRESULT Begin_Shader(_uint iPassIndex);
	HRESULT End_Shader();

private:
	LPD3DXEFFECT			m_pEffect = nullptr;

public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END