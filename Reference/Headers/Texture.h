#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TYPE { TYPE_DEFAULT, TYPE_CUBEMAP, TYPE_END };

private:
	CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eType, const _tchar* pTextureFilePath, _uint iNumTexture);
	virtual HRESULT NativeConstruct(void* pArg);

public:
	HRESULT Bind_OnGraphicDevice(_uint iIndex = 0);
	HRESULT Release_OnGraphicDevice();

public:
	LPDIRECT3DBASETEXTURE9 Get_Texture(_uint iIndex = 0);

private:
	vector<LPDIRECT3DBASETEXTURE9>			m_Textures;
	typedef vector<LPDIRECT3DBASETEXTURE9>	TEXTURES;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType, const _tchar* pTextureFilePath, _uint iNumTexture = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END