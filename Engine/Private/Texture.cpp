#include "..\Public\Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_Textures(rhs.m_Textures)
{
	for (auto& pTexture : m_Textures)
		Safe_AddRef(pTexture);
}

HRESULT CTexture::NativeConstruct_Prototype(TYPE eType, const _tchar * pTextureFilePath, _uint iNumTexture)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	_tchar		szTextureFilePath[MAX_PATH] = TEXT("");

	for (_uint i = 0; i < iNumTexture; ++i)
	{
		LPDIRECT3DBASETEXTURE9			pTexture = nullptr;

		wsprintf(szTextureFilePath, pTextureFilePath, i);

		HRESULT		hr = 0;

		if(TYPE_DEFAULT == eType)
			hr = D3DXCreateTextureFromFile(m_pGraphic_Device, szTextureFilePath, (IDirect3DTexture9**)&pTexture);
		else
			hr = D3DXCreateCubeTextureFromFile(m_pGraphic_Device, szTextureFilePath, (IDirect3DCubeTexture9**)&pTexture);

		if (FAILED(hr))
			return E_FAIL;

		m_Textures.push_back(pTexture);
	}

	return S_OK;
}

HRESULT CTexture::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_OnGraphicDevice(_uint iIndex)
{
	if (nullptr == m_pGraphic_Device || 
		iIndex >= m_Textures.size())
		return E_FAIL;

	return m_pGraphic_Device->SetTexture(0, m_Textures[iIndex]);	
}

HRESULT CTexture::Release_OnGraphicDevice()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->SetTexture(0, nullptr);
}

LPDIRECT3DBASETEXTURE9 CTexture::Get_Texture(_uint iIndex)
{
	if (iIndex >= m_Textures.size())
		return nullptr;

	return m_Textures[iIndex];
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TYPE eType, const _tchar * pTextureFilePath, _uint iNumTexture)
{
	CTexture*	pInstance = new CTexture(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType, pTextureFilePath, iNumTexture)))
	{
		MSGBOX(TEXT("Failed to Created : CTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTexture::Clone(void * pArg)
{
	CTexture*	pInstance = new CTexture(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pTexture : m_Textures)
		Safe_Release(pTexture);

	m_Textures.clear();
}
