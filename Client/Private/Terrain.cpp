#include "stdafx.h"
#include "..\Public\Terrain.h"
#include "GameInstance.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{

}

HRESULT CTerrain::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT CTerrain::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		if (nullptr == pGameInstance)
			return E_FAIL;

		Safe_AddRef(pGameInstance);

		TERRAIN tTerrain = *(TERRAIN*)pArg;

		m_tTerrain = tTerrain;

		m_iTileCountX = (_uint)tTerrain.vSize.x;
		m_iTileCountY = (_uint)tTerrain.vSize.y;
		m_iTileCountZ = (_uint)tTerrain.vSize.z;

		Safe_Release(pGameInstance);
	}

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	Set_Position(_float3{ m_tTerrain.vBasePos.x , m_tTerrain.vBasePos.y + m_iTileCountY, m_tTerrain.vBasePos.z });

	return S_OK;
}

void CTerrain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


}

void CTerrain::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	__super::Late_Tick(fTimeDelta);

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_TERRAIN, this);

}

HRESULT CTerrain::Render()
{
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(m_tTerrain.iDrawID)))
		return E_FAIL;

	//if (FAILED(SetUp_RenderState()))
	//	return E_FAIL;

	m_pVIBufferCom->Render();

	//if (FAILED(Release_RenderState()))
	//	return E_FAIL;

	return S_OK;
}

void CTerrain::Set_Position(const _float3& vPosition)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTerrain::Set_Mode(MODE eMode)
{
	m_eMode = eMode;
}

void CTerrain::Rotation(const _float3& vAxis, _float fRadian)
{
	m_pTransformCom->Rotation(vAxis, fRadian);
}

_int CTerrain::Get_Height(const _float3& vPosition)
{
	if (nullptr == m_iHeights)
		return -1;

	_uint iIndex = (_uint)vPosition.z * m_iTileCountX + (_uint)vPosition.x;

	if (0 > iIndex || m_iTileCountX * m_iTileCountZ <= iIndex)
		return -1;

	return m_iHeights[iIndex];
}

_uint CTerrain::Get_TerrainRange(const _float3& vPosition, int &iTerrainIndexX, int &iTerrainIndexY)
{
	iTerrainIndexX = -1;
	iTerrainIndexY = -1;

	_uint tResult = (_uint)(vPosition.x / m_iTileDivision) + (_uint)(vPosition.z / m_iTileDivision) * (60 / m_iTileDivision);

	/*if (tResult <= 0)
		return 1;

	if (tResult >= m_iTileCountX * m_iTileCountZ)
		return 2;*/

	//콜리전 영역에서 1정도의 위치차이는 서로 콜리전 여부를 검색해야한다.

	/*if ((_uint)vPosition.x % m_iTileDivision == 0 && (_uint)vPosition.x > (m_iTileDivision - 1))
	{
		iTerrainIndexX = tResult - 1;
	}
	else if ((_uint)vPosition.x % m_iTileDivision == (m_iTileDivision - 1) && (_uint)vPosition.x <= m_iTileCountX - m_iTileDivision)
	{
		iTerrainIndexX = tResult + 1;
	}
	else
		iTerrainIndexX = -1;

	if ((_uint)vPosition.z % m_iTileDivision == 0 && (_uint)vPosition.z > (m_iTileDivision - 1))
	{
		iTerrainIndexY = tResult - m_iTileCountX;
	}
	else if ((_uint)vPosition.z % m_iTileDivision == (m_iTileDivision - 1) && (_uint)vPosition.z <= m_iTileCountZ - m_iTileDivision)
	{
		iTerrainIndexY = tResult + m_iTileCountX;
	}
	else
		iTerrainIndexY = -1;*/

	if (tResult < 0)
		return 1;

	if (tResult >= 400)
		return 300;

	return tResult;
}

_uint CTerrain::Get_TerrainRangeAttackCollision(const _float3& vPosition)
{
	_uint tResult = (_uint)(vPosition.x / m_iTileDivision) + (_uint)(vPosition.z / m_iTileDivision) * (60 / m_iTileDivision);

	if (tResult < 0)
		return 1;

	if (tResult >= 400)
		return 300;

	return tResult;
}


HRESULT CTerrain::SetUp_Components()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, &m_tTerrain.vSize)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Tile"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CTerrain::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (MODE_NONTEXTURE == m_eMode)
		m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CTerrain::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (MODE_NONTEXTURE == m_eMode)
		m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	else
		m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return S_OK;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain*	pInstance = new CTerrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain::Clone(void * pArg)
{
	CTerrain*	pInstance = new CTerrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Delete_Array(m_iHeights);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
