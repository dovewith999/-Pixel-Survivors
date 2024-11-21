#include "stdafx.h"
#include "../public/Cube.h"
#include "GameInstance.h"


CCube::CCube(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
}

CCube::CCube(const CCube & pOrigin)
	: CGameObject(pOrigin)
{
}

HRESULT CCube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	//ERRMSG("오브젝트 프로토타입 생성완룟");

	return S_OK;
}

HRESULT CCube::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(Set_Component()))
		return E_FAIL;

	//ERRMSG("클론오브젝트 생성했담");

	if (nullptr != pArg)
	{
		m_eCube = *(CUBE*)pArg;

		m_pVIBufferComponent->Set_VertexPosCubeCreate(m_eCube.vPosition);
		m_pVIBufferComponent->Set_VertexResize(m_eCube.vSize);
		m_pVIBufferComponent->Set_Color(m_eCube.dwColor);
	}

	return S_OK;
}

void CCube::Tick(float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	
}

void CCube::Late_Tick(float fTimeDelta)
{
	if (nullptr == m_pRendererComponent)
		return;

	if(m_bCubeRender)
		m_pRendererComponent->Add_RenderList(CRenderer::RENDER_NONALPHABLEND, this);

	__super::Late_Tick(fTimeDelta);
}

HRESULT CCube::Render()
{
	if (nullptr == m_pVIBufferComponent)
		return E_FAIL;

	if (!m_bCubeRender)
		return S_OK;

	__super::Render();

	m_pGraphic_Device->SetTexture(0, nullptr);

	if (FAILED(m_pTransformComponent->Bind_OnGraphicDevice()))
		return E_FAIL;

	m_pVIBufferComponent->Render();

	return S_OK;
}

void CCube::Set_WorldSize(D3DXVECTOR3 vMasterCubeSize, D3DXVECTOR3 vCubeSize)
{
	if (nullptr == m_pTransformComponent)
		return;

	m_pTransformComponent->Set_ScalePlus(vCubeSize);
	m_pVIBufferComponent->Set_VertexPosCubeCreate(m_eCube.vPosition);
	m_pVIBufferComponent->Set_VertexPos(m_vPosition, D3DXVECTOR3(vMasterCubeSize.x / m_eCube.vSize.x, vMasterCubeSize.y / m_eCube.vSize.y, vMasterCubeSize.z / m_eCube.vSize.z));
}

void CCube::Set_VertexPos(D3DXVECTOR3 vVertexPos, D3DXVECTOR3 vMasterCubeSize)
{
	m_pVIBufferComponent->Set_VertexPos(vVertexPos, D3DXVECTOR3(1.f, 1.f, 1.f));
	m_vPosition.x += vVertexPos.x;
	m_vPosition.y += vVertexPos.y;
	m_vPosition.z += vVertexPos.z;
}

void CCube::Set_BindTransform(CTransform * pTransform)
{
	m_pTransformComponent = pTransform;
	Safe_AddRef(m_pTransformComponent);
}

HRESULT CCube::Set_Component()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	

	/*if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformComponent, &TransformDesc)))
		return E_FAIL;*/

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_ColorCube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferComponent)))
		return E_FAIL;

	/* For.Com_Renderer `*/
	if (FAILED(__super::Add_Components(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererComponent)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CCube * CCube::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CCube* pInstance = new CCube(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX(L"오브젝트 프로토타입 생성실팻");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCube * CCube::Clone(void * pArg)
{
	CCube* pInstance = new CCube(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX(L"오브젝트 복사 생성실팻");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCube::Free()
{
	__super::Free();

	Safe_Release(m_pRendererComponent);
	Safe_Release(m_pTransformComponent);
	Safe_Release(m_pVIBufferComponent);
}
