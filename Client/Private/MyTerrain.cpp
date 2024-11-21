#include "stdafx.h"
#include "..\Public\MyTerrain.h"
#include "../Public/TerrainCube.h"

MyTerrain::MyTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{

}

MyTerrain::MyTerrain(const MyTerrain & rhs)
	: CGameObject(rhs)
{

}

HRESULT MyTerrain::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT MyTerrain::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		if (nullptr == pGameInstance)
			return E_FAIL;

		Safe_AddRef(pGameInstance);

		D3DXVECTOR3 vTerrainSize = *(D3DXVECTOR3*)pArg;

		m_iCubeCountX = (_uint)vTerrainSize.x;
		m_iCubeCountZ = (_uint)vTerrainSize.z;

		for (_uint i = 0; i < vTerrainSize.z; ++i)
		{
			for (_uint j = 0; j < vTerrainSize.x; ++j)
			{
				_uint iIndex = i * (_uint)vTerrainSize.x + j;

				CUBESTATE tagCubeState;

				D3DXVECTOR3 vPosition{ (j * TERRAIN_CUBE_SIZE) + (TERRAIN_CUBE_SIZE / 2.f), TERRAIN_CUBE_SIZE / 2.f, (i * TERRAIN_CUBE_SIZE) + (TERRAIN_CUBE_SIZE / 2.f) };

				tagCubeState.vPosition = vPosition;

				for (int iVertexIdx = 0; iVertexIdx < 8; ++iVertexIdx)
					tagCubeState.vVertexPos[iVertexIdx] = { 0.f, 0.f, 0.f };

				tagCubeState.iTextureOption = 0;
				tagCubeState.iCubeOption = 0;

				if (FAILED(pGameInstance->Add_Layer(0, L"Layer_Terrain_Cube", L"Prototype_GameObject_Cube", &tagCubeState)))
					MSGBOX(L"큐브 생성실패");

				m_CubeMap[iIndex].push_back(dynamic_cast<TerrainCube*>(pGameInstance->Find_LayerObject(0, L"Layer_Terrain_Cube")));

				if (MODE_TEXTURE == eMode)
					m_CubeMap[iIndex].back()->Set_Mode(1);

				m_IndexMap[iIndex].push_back(iIndex);

				++m_iCubeCount;
			}
		}

		Safe_Release(pGameInstance);
	}

	return S_OK;
}

void MyTerrain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


}

void MyTerrain::Late_Tick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	__super::Late_Tick(fTimeDelta);

	m_pRendererCom->Add_RenderList(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT MyTerrain::Render()
{
	/*if (nullptr == m_pVIBufferCom)
		return E_FAIL;*/

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

void MyTerrain::Set_CubeHeight(const RECTRANGE& tagRange, _uint iDirection, _int iHeight)
{
	int iSourIdxZ = static_cast<int>(tagRange.vLeftDownPos.z / TERRAIN_CUBE_SIZE);
	int iDestIdxZ = static_cast<int>((tagRange.vLeftUpPos.z - 1) / TERRAIN_CUBE_SIZE);

	int iSourIdxX = static_cast<int>(tagRange.vLeftDownPos.x / TERRAIN_CUBE_SIZE);
	int iDestIdxX = static_cast<int>((tagRange.vRightDownPos.x - 1) / TERRAIN_CUBE_SIZE);

	_uint iIndex = 0;
	_float fHeight = (_float)iHeight;
	_float fCurrentHeight = 0.0f;
	_float fPreHeight = 0.0f;

	// BACK(0), RIGHT(1), FRONT(2), LEFT(3)

	// BACK
	if (0 == iDirection)
	{
		// 렉트 세로 길이
		_float fDepth = static_cast<_float>(fabs(iDestIdxZ - iSourIdxZ) + 1);

		// 기존 큐브 높이와 조절하려는 높이 사이의 각도
		//_float fRadian = atanf(fHeight / (TERRAIN_CUBE_SIZE * fDepth));

		// 고정 높이값
		_float fStaticHeight = tanf(fHeight / (TERRAIN_CUBE_SIZE * fDepth));

		for (_int i = iSourIdxZ; i <= iDestIdxZ; ++i)
		{
			for (_int j = iSourIdxX; j <= iDestIdxX; ++j)
			{
				iIndex = i * m_iCubeCountX + j;

				TerrainCube* pCube = m_CubeMap[iIndex].back();

				// 큐브의 앞쪽 높이 조절
				if (i != iSourIdxZ)
					pCube->Set_Height(2, fPreHeight);

				fCurrentHeight = fPreHeight + fStaticHeight;

				// 큐브의 뒤쪽 높이 조절
				pCube->Set_Height(iDirection, fCurrentHeight);
			}

			fPreHeight = fCurrentHeight;
		}
		return;
	}

	// FRONT
	if (2 == iDirection)
	{
		// 렉트 세로 길이
		_float fDepth = static_cast<_float>(fabs(iDestIdxZ - iSourIdxZ) + 1);

		// 고정 높이값
		_float fStaticHeight = tanf(fHeight / (TERRAIN_CUBE_SIZE * fDepth));

		for (_int i = iDestIdxZ; i >= iSourIdxZ; --i)
		{
			for (_int j = iSourIdxX; j <= iDestIdxX; ++j)
			{
				iIndex = i * m_iCubeCountX + j;

				TerrainCube* pCube = m_CubeMap[iIndex].back();

				// 큐브의 뒤쪽 높이 조절
				if (i != iDestIdxZ)
					pCube->Set_Height(0, fPreHeight);

				fCurrentHeight = fPreHeight + fStaticHeight;

				// 큐브의 앞쪽 높이 조절
				pCube->Set_Height(iDirection, fCurrentHeight);
			}

			fPreHeight = fCurrentHeight;
		}
		return;
	}

	// LEFT
	if (3 == iDirection)
	{
		// 렉트 가로 길이
		_float fDepth = static_cast<_float>(fabs(iDestIdxX - iSourIdxX) + 1);

		// 고정 높이값
		_float fStaticHeight = tanf(fHeight / (TERRAIN_CUBE_SIZE * fDepth));

		for (_int i = iDestIdxX; i >= iSourIdxX; --i)
		{
			for (_int j = iSourIdxZ; j <= iDestIdxZ; ++j)
			{
				iIndex = j * m_iCubeCountX + i;

				TerrainCube* pCube = m_CubeMap[iIndex].back();

				// 큐브의 오른쪽 높이 조절
				if (i != iDestIdxX)
					pCube->Set_Height(1, fPreHeight);

				fCurrentHeight = fPreHeight + fStaticHeight;

				// 큐브의 왼쪽 높이 조절
				pCube->Set_Height(iDirection, fCurrentHeight);
			}

			fPreHeight = fCurrentHeight;
		}
		return;
	}

	// RIGHT
	if (1 == iDirection)
	{
		// 렉트 가로 길이
		_float fDepth = static_cast<_float>(fabs(iDestIdxX - iSourIdxX) + 1);

		// 고정 높이값
		_float fStaticHeight = tanf(fHeight / (TERRAIN_CUBE_SIZE * fDepth));

		for (_int i = iSourIdxX; i <= iDestIdxX; ++i)
		{
			for (_int j = iSourIdxZ; j <= iDestIdxZ; ++j)
			{
				iIndex = j * m_iCubeCountX + i;

				TerrainCube* pCube = m_CubeMap[iIndex].back();

				// 큐브의 왼쪽 높이 조절
				if (i != iSourIdxX)
					pCube->Set_Height(3, fPreHeight);

				fCurrentHeight = fPreHeight + fStaticHeight;

				// 큐브의 오른쪽 높이 조절
				pCube->Set_Height(iDirection, fCurrentHeight);
			}

			fPreHeight = fCurrentHeight;
		}
		return;
	}
}

void MyTerrain::Create_Cube(const RECTRANGE& tagRange, _uint iCubeCount)
{
	_uint iSourIdxZ = static_cast<_uint>(tagRange.vLeftDownPos.z / TERRAIN_CUBE_SIZE);
	_uint iDestIdxZ = static_cast<_uint>((tagRange.vLeftUpPos.z - 1) / TERRAIN_CUBE_SIZE);

	_uint iSourIdxX = static_cast<_uint>(tagRange.vLeftDownPos.x / TERRAIN_CUBE_SIZE);
	_uint iDestIdxX = static_cast<_uint>((tagRange.vRightDownPos.x - 1) / TERRAIN_CUBE_SIZE);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return;

	Safe_AddRef(pGameInstance);

	for (_uint i = iSourIdxZ; i <= iDestIdxZ; ++i)
	{
		for (_uint j = iSourIdxX; j <= iDestIdxX; ++j)
		{
			_uint iIndex = i * m_iCubeCountX + j;

			for (_uint iCount = 0; iCount < iCubeCount; ++iCount)
			{
				CUBESTATE tagCubeState;

				D3DXVECTOR3 vLastPosition = m_CubeMap[iIndex].back()->Get_Position();
				tagCubeState.vPosition = { vLastPosition.x, vLastPosition.y + TERRAIN_CUBE_SIZE, vLastPosition.z };

				for (int iVertexIdx = 0; iVertexIdx < 8; ++iVertexIdx)
					tagCubeState.vVertexPos[iVertexIdx] = { 0.f, 0.f, 0.f };

				tagCubeState.iTextureOption = 0;
				tagCubeState.iCubeOption = 0;

				if (FAILED(pGameInstance->Add_Layer(0, L"Layer_Terrain_Cube", L"Prototype_GameObject_Cube", &tagCubeState)))
					MSGBOX(L"큐브 생성실패");

				m_CubeMap[iIndex].push_back(dynamic_cast<TerrainCube*>(pGameInstance->Find_LayerObject(0, L"Layer_Terrain_Cube")));

				++m_iCubeCount;

				_uint iMaxIndex = 0;

				for (auto& Pair : m_IndexMap)
				{
					for (auto& iter : Pair.second)
					{
						if (iMaxIndex < iter)
							iMaxIndex = iter;
					}
				}

				++iMaxIndex;

				m_IndexMap[iIndex].push_back(iMaxIndex);
			}
		}
	}

	Safe_Release(pGameInstance);

}

void MyTerrain::Remove_Cube(const RECTRANGE& tagRange, _uint iCubeCount)
{
	_uint iSourIdxZ = static_cast<_uint>(tagRange.vLeftDownPos.z / TERRAIN_CUBE_SIZE);
	_uint iDestIdxZ = static_cast<_uint>((tagRange.vLeftUpPos.z - 1) / TERRAIN_CUBE_SIZE);

	_uint iSourIdxX = static_cast<_uint>(tagRange.vLeftDownPos.x / TERRAIN_CUBE_SIZE);
	_uint iDestIdxX = static_cast<_uint>((tagRange.vRightDownPos.x - 1) / TERRAIN_CUBE_SIZE);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return;

	Safe_AddRef(pGameInstance);

	for (_uint i = iSourIdxZ; i <= iDestIdxZ; ++i)
	{
		for (_uint j = iSourIdxX; j <= iDestIdxX; ++j)
		{
			_uint iIndex = i * m_iCubeCountX + j;

			for (_uint iCount = 0; iCount < iCubeCount; ++iCount)
			{
				if (0 == m_IndexMap[iIndex].size() || iCount > m_IndexMap[iIndex].size())
					break;

				_uint iObjectIndex = m_IndexMap[iIndex].back();

				pGameInstance->Delete_LayerObject(0, L"Layer_Terrain_Cube", iObjectIndex);

				m_CubeMap[iIndex].pop_back();

				--m_iCubeCount;

				m_IndexMap[iIndex].pop_back();

				for (auto& Pair : m_IndexMap)
				{
					for (auto& iter : Pair.second)
					{
						if (iObjectIndex <= iter)
							--iter;
					}
				}
			}
		}
	}

	Safe_Release(pGameInstance);

}

CCube* MyTerrain::Find_Cube(_uint iIndex, _float fHeight)
{
	//D3DXVECTOR3 vLastPosition = m_CubeList[iIndex]->Get_Position();

	return nullptr;
}

void MyTerrain::Set_CubeMode(_uint iMode)
{
	eMode = (MODE)iMode;

	for (auto& Pair : m_CubeMap)
	{
		for (auto& iter : Pair.second)
			iter->Set_Mode(iMode);
	}
}

HRESULT MyTerrain::SetUp_Components()
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

	m_pTransformCom = (CTransform*)pGameInstance->Clone_Component(0, TEXT("Prototype_Component_Transform"), &TransformDesc);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	/* For.Com_Renderer `*/
	m_pRendererCom = (CRenderer*)pGameInstance->Clone_Component(0, TEXT("Prototype_Component_Renderer"));
	if (nullptr == m_pRendererCom)
		return E_FAIL;

	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT MyTerrain::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (MODE_TEXTURE == eMode)
		m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	else
		m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return S_OK;
}

HRESULT MyTerrain::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (MODE_TEXTURE == eMode)
		m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

_uint MyTerrain::Check_CubeIndex(_float3 vPos) // 04/26 임희섭 추우가
{
	m_iCubeIndex = 0;

	for (auto pair : m_CubeMap)
	{
		for (auto iter : pair.second)
		{
			if (iter->Check_Index(vPos))
			{
				return m_iCubeIndex;
			}
			++m_iCubeIndex;
		}
	}

	return 2;
}

MyTerrain * MyTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	MyTerrain*	pInstance = new MyTerrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(TEXT("Failed to Created : MyTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * MyTerrain::Clone(void * pArg)
{
	MyTerrain*	pInstance = new MyTerrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(TEXT("Failed to Cloned : MyTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT MyTerrain::Save_Data(HANDLE& hFile, DWORD& dwByte)
{
	_uint iIndexMapSize = m_iCubeCountX * m_iCubeCountZ;
	_uint iIndexListSize = 0;

	WriteFile(hFile, &m_iCubeCountX, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &m_iCubeCountZ, sizeof(_uint), &dwByte, nullptr);
	WriteFile(hFile, &iIndexMapSize, sizeof(_uint), &dwByte, nullptr);

	for (auto& iter : m_IndexMap) // 인덱스맵 저장 (맵 키 기준, value의 list 사이즈, 키, value 저장)
	{
		iIndexListSize = static_cast<_uint>(iter.second.size());
		WriteFile(hFile, &iIndexListSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &iter.first, sizeof(_uint), &dwByte, nullptr);
		for (auto& Listiter : iter.second)
		{
			WriteFile(hFile, &(Listiter), sizeof(_uint), &dwByte, nullptr);
		}
	}

	_uint iCubeMapSize = static_cast<_uint>(m_CubeMap.size());
	_uint iCubeListSize = 0;

	for (auto& Pair : m_CubeMap) // 큐브 저장(큐브맵 인덱스 기준)
	{
		iCubeListSize = static_cast<_uint>(Pair.second.size());
		WriteFile(hFile, &iCubeListSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hFile, &Pair.first, sizeof(_uint), &dwByte, nullptr);
		for (auto& iter : Pair.second)
			WriteFile(hFile, &(iter->m_TagCubeState), sizeof(CUBESTATE), &dwByte, nullptr);
	}

	return S_OK;
}

void MyTerrain::Free()
{
	__super::Free();

	/*CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return;

	Safe_AddRef(pGameInstance);

	for (int i = 0; i < m_iCubeCount; ++i)
		pGameInstance->Delete_LayerObject(0, L"Layer_Terrain_Cube");
	m_CubeMap.clear();

	Safe_Release(pGameInstance);*/

	for (auto& Pair : m_IndexMap)
		Pair.second.clear();
	m_IndexMap.clear();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	//Safe_Release(m_pVIBufferCom);
}
