#include "stdafx.h"
#include "..\public\MasterCube.h"


CMasterCube::CMasterCube(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
}

CMasterCube::CMasterCube(const CMasterCube & rhs)
	:CGameObject(rhs)
	//, m_pGameInstance(CGameInstance::Get_Instance())
{
	//Safe_AddRef(m_pGameInstance);
}

HRESULT CMasterCube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	//ERRMSG("오브젝트 프로토타입 생성완룟 : CMasterCube");

	return S_OK;
}

HRESULT CMasterCube::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(Set_Component()))
		return E_FAIL;

	//ERRMSG("클론오브젝트 생성했담 : CMasterCube");

	if (nullptr != pArg)
	{
		m_vSize = *(D3DXVECTOR3*)pArg;
		Set_Positon(D3DXVECTOR3(3.f, 3.f, 7.f));
		m_pTransformComponent->Set_Scale(m_vSize);
	}
	return S_OK;
}

void CMasterCube::Tick(float fTimeDelta)
{
	__super::Tick(fTimeDelta);



	if (m_mapCube.empty())
		return;

	
	//m_pTransformComponent->Turn(D3DXVECTOR3(0.f, 1.f, 0.f), fTimeDelta);

}

void CMasterCube::Late_Tick(float fTimeDelta)
{
	if (nullptr == m_pRendererComponent)
		return;

	//m_pRendererComponent->Add_RenderList(CRenderer::RENDER_PRIORITY, this);


	__super::Late_Tick(fTimeDelta);
}

HRESULT CMasterCube::Render()
{
	if (nullptr == m_pVIBufferComponent)
		return E_FAIL;

		__super::Render();

	if (FAILED(m_pTransformComponent->Bind_OnGraphicDevice()))
		return E_FAIL;


	/*if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferComponent->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;*/



	return S_OK;
}

void CMasterCube::Set_Positon(D3DXVECTOR3 vPosition)
{
	m_pTransformComponent->Set_State(CTransform::STATE_POSITION, vPosition);
}

void CMasterCube::Move_Position(D3DXVECTOR3 vPosition)
{
	D3DXVECTOR3 Vec = m_pTransformComponent->Get_State(CTransform::STATE_POSITION);
	m_pTransformComponent->Set_State(CTransform::STATE_POSITION, Vec + vPosition);
}

HRESULT CMasterCube::Set_VertexPos(D3DXVECTOR3 vVertexPos)
{
	m_vVertexPos += vVertexPos;

	m_pVIBufferComponent->Set_VertexPos(vVertexPos, D3DXVECTOR3(1.f, 1.f, 1.f));

	for (auto iter : m_mapCube)
	{
		iter.second->Set_VertexPos(vVertexPos, m_vSize);
	}

	return S_OK;
}

HRESULT CMasterCube::Set_WorldSize(D3DXVECTOR3 vCubeSize)
{
	m_vCubeSize += vCubeSize;
	m_pTransformComponent->Set_ScalePlus(vCubeSize);

	for (auto iter : m_mapCube)
	{
		iter.second->Set_WorldSize(m_vSize, vCubeSize);
	}

	return S_OK;
}

HRESULT CMasterCube::Set_CubeSize(D3DXVECTOR3 vCubeSize)
{
	m_vCubeSize += vCubeSize;

	m_pTransformComponent->Set_Scale(D3DXVECTOR3(vCubeSize.x, vCubeSize.y, vCubeSize.z));

	return S_OK;
}

HRESULT CMasterCube::Set_CubeSizePlus(D3DXVECTOR3 vCubeSize)
{
	m_pTransformComponent->Set_ScalePlus(D3DXVECTOR3(vCubeSize.x, vCubeSize.y, vCubeSize.z));

	return S_OK;
}

HRESULT CMasterCube::Set_CubeVertexPos(D3DXVECTOR3 vCubeVertexPos)
{
	m_vCubeSize = vCubeVertexPos;

	m_pVIBufferComponent->Set_VertexPos(m_vCubeSize, D3DXVECTOR3(1.f, 1.f, 1.f));

	return S_OK;
}

HRESULT CMasterCube::Set_CubeTurn(D3DXVECTOR3 vAxis, float fTimeDelta)
{
	m_pTransformComponent->Turn(vAxis, fTimeDelta);

	return S_OK;
}

HRESULT CMasterCube::Set_CubeTurnAngle(D3DXVECTOR3 vAxis, float fAngle)
{
	m_pTransformComponent->TurnAngle(vAxis, fAngle);

	return S_OK;
}

void CMasterCube::Set_BindTransform(CTransform * pTransform)
{
	Safe_Release(m_pTransformComponent);
	m_pTransformComponent = pTransform;
	Safe_AddRef(m_pTransformComponent);
}


void CMasterCube::Add_Cube(int iIndex, D3DXVECTOR3 vSize, CCube * pCubeObj)
{
	if (nullptr == pCubeObj)
		return;

	m_mapCube.emplace(iIndex, pCubeObj);

	pCubeObj->Set_BindTransform(m_pTransformComponent);

	++m_iCreateCount;
}

int CMasterCube::Cal_Index(D3DXVECTOR3 vLocation, D3DXVECTOR3 vSize)
{
	return 0;
}

HRESULT CMasterCube::Save_Data(HANDLE& hFile, DWORD& dwByte)
{
	int tA = (int)m_mapCube.size();

	WriteFile(hFile, m_vSize, sizeof(D3DXVECTOR3), &dwByte, nullptr);
	WriteFile(hFile, &tA, sizeof(int), &dwByte, nullptr);

	cout << "큐브 숫자 저장 : " << tA << endl;
	for (auto iter : m_mapCube)
	{
		WriteFile(hFile, &iter.second->Get_CubeInfo(), sizeof(CUBE), &dwByte, nullptr);
		WriteFile(hFile, &iter.first, sizeof(int), &dwByte, nullptr);
	}

	return S_OK;
}

D3DXVECTOR3 CMasterCube::Get_Position(void)
{
	return m_pTransformComponent->Get_State(CTransform::STATE_POSITION);
}

HRESULT CMasterCube::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CMasterCube::Release_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CMasterCube::Set_Component()
{

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformComponent, &TransformDesc)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_ColorCube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferComponent)))
		return E_FAIL;

	/* For.Com_Renderer `*/
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererComponent)))
		return E_FAIL;

	return S_OK;
}

CMasterCube * CMasterCube::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CMasterCube* pInstance = new CMasterCube(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(L"오브젝트 프로토타입 생성실팻 : CMasterCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMasterCube * CMasterCube::Clone(void * pArg)
{
	CMasterCube* pInstance = new CMasterCube(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(L"오브젝트 복사 생성실팻 : CMaster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMasterCube::Free()
{
	/*CGameInstance* m_pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(m_pGameInstance);*/

	Safe_Release(m_pRendererComponent);
	Safe_Release(m_pTransformComponent);
	Safe_Release(m_pVIBufferComponent);

	if (m_bDead)
	{
		for (auto& iter : m_mapCube)
		{
			iter.second->Set_Dead();
		}
	}

	m_mapCube.clear();
	m_CubeFill.clear();

	//Safe_Release(m_pGameInstance);

	__super::Free();
}
