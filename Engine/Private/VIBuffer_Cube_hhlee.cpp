#include "../Public/VIBuffer_Cube_hhlee.h"
#include "../Public/AniTransform_hhlee.h"

const float UV_HEAD_UNIT = 0.125f;
const float UV_ARM_BIG = 0.15625f;
const float UV_ARM_MID = 0.0625f;
const float UV_ARM_SMALL = 0.03125f;
const float UV_BODY_WIDHT_CENTER = 0.125f;
const float UV_BODY_HEIGHT_CENTER = 0.1875f;

CVIBuffer_Ani_Cube::CVIBuffer_Ani_Cube(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CVIBuffer(pGraphicDevice)
{
	ZeroMemory(&m_Vetices, sizeof(tagAniVertex));
}

CVIBuffer_Ani_Cube::CVIBuffer_Ani_Cube(const CVIBuffer_Ani_Cube & rhs)
	: CVIBuffer(rhs)
	, m_pTexture(rhs.m_pTexture)
{
	memcpy(&m_Vetices, &rhs.m_Vetices, sizeof(tagAniVertex));
}

HRESULT CVIBuffer_Ani_Cube::NativeConstruct_Prototype()
{
	m_iNumVertices = 24;
	m_iStride = sizeof(tagAniVertex);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;

	m_dwFVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	// Create texture.

	D3DXCreateTextureFromFile(
		m_pGraphic_Device,
		L"../Bin/Resources/Ani/CKMan.jpg",
		&m_pTexture);

	tagAniVertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	// build box
	// fill in the front face vertex data
	//������ �Ʒ�
	v[0] = tagAniVertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, UV_HEAD_UNIT * 2, UV_HEAD_UNIT * 2);
	//��������
	v[1] = tagAniVertex(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -0.5f, UV_HEAD_UNIT * 2, UV_HEAD_UNIT * 1);
	//������
	v[2] = tagAniVertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -0.5f, UV_HEAD_UNIT * 1, UV_HEAD_UNIT * 1);
	//���ʾƷ�
	v[3] = tagAniVertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, UV_HEAD_UNIT * 1, UV_HEAD_UNIT * 2);

	// fill in the back face vertex data
	v[4] = tagAniVertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, UV_HEAD_UNIT * 3, UV_HEAD_UNIT * 2);
	v[5] = tagAniVertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, UV_HEAD_UNIT * 4, UV_HEAD_UNIT * 2);
	v[6] = tagAniVertex(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, UV_HEAD_UNIT * 4, UV_HEAD_UNIT * 1);
	v[7] = tagAniVertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, UV_HEAD_UNIT * 3, UV_HEAD_UNIT * 1);

	// fill in the top face vertex data
	v[8] = tagAniVertex(-0.5f, 0.5f, -0.5f, 0.0f, 0.5f, 0.0f, UV_HEAD_UNIT, UV_HEAD_UNIT);
	v[9] = tagAniVertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f, UV_HEAD_UNIT * 2, UV_HEAD_UNIT);
	v[10] = tagAniVertex(0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f, UV_HEAD_UNIT * 2, 0.0f);
	v[11] = tagAniVertex(0.5f, 0.5f, -0.5f, 0.0f, 0.5f, 0.0f, UV_HEAD_UNIT, 0.0f);

	// fill in the bottom face vertex data
	//x�� ������ �� ����
	//z�� ������ �� ��

	//���� ��
	v[12] = tagAniVertex(-0.5f, -0.5f, -0.5f, 0.0f, -0.5f, 0.0f, UV_HEAD_UNIT * 2, 0.f);
	//������ ��
	v[13] = tagAniVertex(0.5f, -0.5f, -0.5f, 0.0f, -0.5f, 0.0f, UV_HEAD_UNIT * 3, 0.f);
	//������ �Ʒ�
	v[14] = tagAniVertex(0.5f, -0.5f, 0.5f, 0.0f, -0.5f, 0.0f, UV_HEAD_UNIT * 3, UV_HEAD_UNIT * 1);
	//���� �Ʒ�
	v[15] = tagAniVertex(-0.5f, -0.5f, 0.5f, 0.0f, -0.5f, 0.0f, UV_HEAD_UNIT * 2, UV_HEAD_UNIT * 1);

	// fill in the left face vertex data
	//���� �Ʒ�
	v[16] = tagAniVertex(-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.f, UV_HEAD_UNIT * 2);
	//���� ��
	v[17] = tagAniVertex(-0.5f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.f, UV_HEAD_UNIT * 1);
	//������ ��
	v[18] = tagAniVertex(-0.5f, 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, UV_HEAD_UNIT, UV_HEAD_UNIT);
	//������ �Ʒ�
	v[19] = tagAniVertex(-0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, UV_HEAD_UNIT, UV_HEAD_UNIT * 2);

	// fill in the right face vertex data
	//������ �Ʒ�
	v[20] = tagAniVertex(0.5f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, UV_HEAD_UNIT * 3, UV_HEAD_UNIT * 2);
	//���� �Ʒ�
	v[21] = tagAniVertex(0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, UV_HEAD_UNIT * 2, UV_HEAD_UNIT * 2);
	//���� ��
	v[22] = tagAniVertex(0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, UV_HEAD_UNIT * 2, UV_HEAD_UNIT * 1);
	//������ ��
	v[23] = tagAniVertex(0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, UV_HEAD_UNIT * 3, UV_HEAD_UNIT * 1);

	m_pVB->Unlock();

	m_iPrimitiveIndicesSize = 36 * sizeof(WORD);
	m_eIndexFormat = D3DFMT_INDEX16;
	m_iNumPrimitive = 1;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	//FACEINDICES16* pIndices = nullptr;

	WORD* i = 0;

	m_pIB->Lock(0, 0, (void**)&i, 0);

	// fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Ani_Cube::Set_Component()
{
	return S_OK;
}

HRESULT CVIBuffer_Ani_Cube::Setup_RenderState()
{
	Set_Texture(TEXT("../Bin/Resources/Ani/CKMan.jpg"));

	return S_OK;
}

HRESULT CVIBuffer_Ani_Cube::Rlease_RenderState()
{
	Release_Texture();

	return S_OK;
}

HRESULT CVIBuffer_Ani_Cube::NativeConstruct(void * pArg)
{
	D3DLIGHT9 light;

	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	light.Direction = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	m_pGraphic_Device->SetLight(0, &light);
	m_pGraphic_Device->LightEnable(0, true);


	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	m_pGraphic_Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_pGraphic_Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_iNumVertices = 24;
	m_iStride = sizeof(tagAniVertex);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;

	m_dwFVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);

	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	CUBE_BODY_INFO* InitInfo = (CUBE_BODY_INFO*)pArg;


	D3DXCreateTextureFromFile(
		m_pGraphic_Device,
		InitInfo->strFilePath,
		&m_pTexture);

	//if (FAILED(__super::Create_VertexBuffer()))
	//	return E_FAIL;

	m_pGraphic_Device->CreateVertexBuffer(
		24 * sizeof(tagAniVertex),
		D3DUSAGE_WRITEONLY,
		m_dwFVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		0);

	tagAniVertex* pVertices = nullptr;

	if (InitInfo)
	{
		m_eBody = InitInfo->eCube;

		float fWidth = InitInfo->fWidth;
		float fTopHeight = InitInfo->fTopHeight;
		float fBottomHeight = InitInfo->fBottomHeight;
		float fPos = InitInfo->fPos;

		m_stShape.fWidth = fWidth;
		m_stShape.fTop = fTopHeight;
		m_stShape.fBottom = fBottomHeight;
		m_stShape.fPos = fPos;

		switch (InitInfo->eCube)
		{
		case CUBE_HEAD:
			if (FAILED(Set_Head_Vertex()))
			{
				MSGBOX(L"Failed to Create Set_Head_Vertex");
			}
			break;
		case CUBE_BODY:
			if (FAILED(Set_Body_Vertex()))
			{
				MSGBOX(L"Failed to Create Set_Body_Vertex");
			}
			break;
		case CUBE_LEFTARM:
		case CUBE_RIGHTARM:
			if (FAILED(Set_Arm_Vertex()))
			{
				MSGBOX(L"Failed to Create Set_Arm_Vertex");
			}
			break;
		case CUBE_LEFTLEG:
		case CUBE_RIGHTLEG:
			if (FAILED(Set_Leg_Vertex()))
			{
				MSGBOX(L"Failed to Create Set_Leg_Vertex");
			}
			break;
		case CUBE_LEFTELBOW:
		case CUBE_RIGHTELBOW:
			if (FAILED(Set_Elbow_Vertex()))
			{
				MSGBOX(L"Failed to Create Set_Elbow_Vertex");
			}
			break;
		case CUBE_LEFTANKLE:
		case CUBE_RIGHTANKLE:
			if (FAILED(Set_Ankle_Vertex()))
			{
				MSGBOX(L"Failed to Create Set_Ankle_Vertex");
			}
			break;
		default:
			if (FAILED(Set_Default_Vertex()))
			{
				MSGBOX(L"Failed to Create Set_Default_Vertex");
			}
		}

	}

	return S_OK;
}

HRESULT CVIBuffer_Ani_Cube::Set_Vertices(_float width, _float topheight, _float bottom, _float pos)
{


	tagAniVertex* pVertices = nullptr;

	//float fPos = pRadius[3];aswwaw
	tagAniVertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);
	//TEST
	//face 
	//// build box
	//// fill in the front face vertex data
	////������ �Ʒ�
	//4
	v[0] = tagAniVertex(-0.05f, -0.05f, -0.05f, 0.0f, 0.0f, -1.0f, UV_HEAD_UNIT * 2, UV_HEAD_UNIT * 2);
	//��������
	v[1] = tagAniVertex(-0.05f, 0.05f, -0.05f, 0.0f, 0.0f, -1.0f, UV_HEAD_UNIT * 2, UV_HEAD_UNIT * 1);
	//������
	v[2] = tagAniVertex(0.05f, 0.05f, -0.05f, 0.0f, 0.0f, -1.0f, UV_HEAD_UNIT * 1, UV_HEAD_UNIT * 1);
	//���ʾƷ�
	v[3] = tagAniVertex(0.05f, -0.05f, -0.05f, 0.0f, 0.0f, -1.0f, UV_HEAD_UNIT * 1, UV_HEAD_UNIT * 2);

	// fill in the back face vertex data
	v[4] = tagAniVertex(-0.05f, -0.05f, 0.05f, 0.0f, 0.0f, 1.0f, UV_HEAD_UNIT * 3, UV_HEAD_UNIT * 2);
	v[5] = tagAniVertex(0.05f, -0.05f, 0.05f, 0.0f, 0.0f, 1.0f, UV_HEAD_UNIT * 4, UV_HEAD_UNIT * 2);
	v[6] = tagAniVertex(0.05f, 0.05f, 0.05f, 0.0f, 0.0f, 1.0f, UV_HEAD_UNIT * 4, UV_HEAD_UNIT * 1);
	v[7] = tagAniVertex(-0.05f, 0.05f, 0.05f, 0.0f, 0.0f, 1.0f, UV_HEAD_UNIT * 3, UV_HEAD_UNIT * 1);

	// fill in the top face vertex data
	v[8] = tagAniVertex(-0.05f, 0.05f, -0.05f, 0.0f, 1.0f, 0.0f, UV_HEAD_UNIT, UV_HEAD_UNIT);
	v[9] = tagAniVertex(-0.05f, 0.05f, 0.05f, 0.0f, 1.0f, 0.0f, UV_HEAD_UNIT * 2, UV_HEAD_UNIT);
	v[10] = tagAniVertex(0.05f, 0.05f, 0.05f, 0.0f, 1.0f, 0.0f, UV_HEAD_UNIT * 2, 0.0f);
	v[11] = tagAniVertex(0.05f, 0.05f, -0.05f, 0.0f, 1.0f, 0.0f, UV_HEAD_UNIT, 0.0f);

	// fill in the bottom face vertex data
	//x�� ������ �� ����
	//z�� ������ �� ��

	//���� ��
	v[12] = tagAniVertex(-0.05f, -0.05f, -0.05f, 0.0f, -1.0f, 0.0f, UV_HEAD_UNIT * 2, 0.f);
	//������ ��
	v[13] = tagAniVertex(0.05f, -0.05f, -0.05f, 0.0f, -1.0f, 0.0f, UV_HEAD_UNIT * 3, 0.f);
	//������ �Ʒ�
	v[14] = tagAniVertex(0.05f, -0.05f, 0.05f, 0.0f, -1.0f, 0.0f, UV_HEAD_UNIT * 3, UV_HEAD_UNIT * 1);
	//���� �Ʒ�
	v[15] = tagAniVertex(-0.05f, -0.05f, 0.05f, 0.0f, -1.0f, 0.0f, UV_HEAD_UNIT * 2, UV_HEAD_UNIT * 1);

	// fill in the left face vertex data
	//���� �Ʒ�
	v[16] = tagAniVertex(-0.05f, -0.05f, 0.05f, -1.0f, 0.0f, 0.0f, 0.f, UV_HEAD_UNIT * 2);
	//���� ��
	v[17] = tagAniVertex(-0.05f, 0.05f, 0.05f, -1.0f, 0.0f, 0.0f, 0.f, UV_HEAD_UNIT * 1);
	//������ ��
	v[18] = tagAniVertex(-0.05f, 0.05f, -0.05f, -1.0f, 0.0f, 0.0f, UV_HEAD_UNIT, UV_HEAD_UNIT);
	//������ �Ʒ�
	v[19] = tagAniVertex(-0.05f, -0.05f, -0.05f, -1.0f, 0.0f, 0.0f, UV_HEAD_UNIT, UV_HEAD_UNIT * 2);

	// fill in the right face vertex data
	//������ �Ʒ�
	v[20] = tagAniVertex(0.05f, -0.05f, 0.05f, 1.0f, 0.0f, 0.0f, UV_HEAD_UNIT * 3, UV_HEAD_UNIT * 2);
	//���� �Ʒ�
	v[21] = tagAniVertex(0.05f, -0.05f, -0.05f, 1.0f, 0.0f, 0.0f, UV_HEAD_UNIT * 2, UV_HEAD_UNIT * 2);
	//���� ��
	v[22] = tagAniVertex(0.05f, 0.05f, -0.05f, 1.0f, 0.0f, 0.0f, UV_HEAD_UNIT * 2, UV_HEAD_UNIT * 1);
	//������ ��
	v[23] = tagAniVertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, UV_HEAD_UNIT * 3, UV_HEAD_UNIT * 1);

	m_pVB->Unlock();

	m_iPrimitiveIndicesSize = 36 * sizeof(WORD);
	m_eIndexFormat = D3DFMT_INDEX16;
	m_iNumPrimitive = 1;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	//FACEINDICES16* pIndices = nullptr;

	WORD* i = 0;

	m_pIB->Lock(0, 0, (void**)&i, 0);

	// fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	m_pIB->Unlock();
	return S_OK;
}

void CVIBuffer_Ani_Cube::Set_VerticesWidth(void* pArg)
{
	float* pRadius = (float*)pArg;

	if (!pRadius)
		MSGBOX(TEXT("Set_Vertex radius pointer is null"));

	m_stShape.fWidth = pRadius[0];

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition.x = -1.f * m_stShape.fWidth;
	pVertices[0].vPosition.z = -1.f * m_stShape.fWidth;

	pVertices[1].vPosition.x = 1.f * m_stShape.fWidth;
	pVertices[1].vPosition.z = -1.f * m_stShape.fWidth;


	pVertices[2].vPosition.x = 1.f * m_stShape.fWidth;
	pVertices[2].vPosition.z = -1.f * m_stShape.fWidth;

	pVertices[3].vPosition.x = -1.f * m_stShape.fWidth;
	pVertices[3].vPosition.z = -1.f * m_stShape.fWidth;

	pVertices[4].vPosition.x = -1.f * m_stShape.fWidth;
	pVertices[4].vPosition.z = 1.f * m_stShape.fWidth;

	pVertices[5].vPosition.x = 1.f * m_stShape.fWidth;
	pVertices[5].vPosition.z = 1.f * m_stShape.fWidth;

	pVertices[6].vPosition.x = 1.f * m_stShape.fWidth;
	pVertices[6].vPosition.z = 1.f * m_stShape.fWidth;

	pVertices[7].vPosition.x = -1.f * m_stShape.fWidth;
	pVertices[7].vPosition.z = 1.f * m_stShape.fWidth;

	m_pVB->Unlock();
}

CVIBuffer_Ani_Cube* CVIBuffer_Ani_Cube::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CVIBuffer_Ani_Cube* pInstance = new CVIBuffer_Ani_Cube(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CVIBuffer_Ani_Cube::Clone(void * pArg)
{
	CVIBuffer_Ani_Cube* pInstance = new CVIBuffer_Ani_Cube(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

HRESULT CVIBuffer_Ani_Cube::Render(_float4x4 worldmatrix)
{
	m_pGraphic_Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_pGraphic_Device->SetRenderState(D3DRS_SPECULARENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &worldmatrix);
	m_pGraphic_Device->BeginScene();
	m_pGraphic_Device->SetTexture(0, m_pTexture);
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(tagAniVertex));
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->SetFVF(m_dwFVF);
	m_pGraphic_Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		24,
		0,
		12);
	return S_OK;
}

void CVIBuffer_Ani_Cube::Set_VerticesHeight(void* pArg)
{

	if (!pArg)
		MSGBOX(TEXT("Set_Vertex radius pointer is null"));

	m_stShape.fTop = ((float*)pArg)[0];
	m_stShape.fBottom = ((float*)pArg)[1];
	m_stShape.fPos = ((float*)pArg)[2];

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);
	//fHeight = fHeight + -1.f;
	pVertices[0].vPosition.y = m_stShape.fTop + m_stShape.fPos;

	pVertices[1].vPosition.y = m_stShape.fTop + m_stShape.fPos;

	pVertices[2].vPosition.y = m_stShape.fBottom + m_stShape.fPos;

	pVertices[3].vPosition.y = m_stShape.fBottom + m_stShape.fPos;

	pVertices[4].vPosition.y = m_stShape.fTop + m_stShape.fPos;

	pVertices[5].vPosition.y = m_stShape.fTop + m_stShape.fPos;

	pVertices[6].vPosition.y = m_stShape.fBottom + m_stShape.fPos;

	pVertices[7].vPosition.y = m_stShape.fBottom + m_stShape.fPos;

	m_pVB->Unlock();
}

void CVIBuffer_Ani_Cube::Free()
{
	__super::Free();
}

HRESULT CVIBuffer_Ani_Cube::Set_Head_Vertex()
{
	//float m_stShape.fPos = pRadius[3];aswwaw
	tagAniVertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	//������ �Ʒ�
	v[0] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth,
		0.0f, 0.0f, -m_stShape.fWidth, UV_HEAD_UNIT * 2, UV_HEAD_UNIT * 2);
	//��������
	v[1] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth,
		0.0f, 0.0f, -m_stShape.fWidth, UV_HEAD_UNIT * 2, UV_HEAD_UNIT * 1);
	//������
	v[2] = tagAniVertex(1 * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth,
		0.0f, 0.0f, -m_stShape.fWidth, UV_HEAD_UNIT * 1, UV_HEAD_UNIT * 1);
	//���ʾƷ�
	v[3] = tagAniVertex(1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth,
		0.0f, 0.0f, -m_stShape.fWidth, UV_HEAD_UNIT * 1, UV_HEAD_UNIT * 2);

	// fill in the back face vertex data
	v[4] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth,
		0.0f, 0.0f, m_stShape.fWidth, UV_HEAD_UNIT * 3, UV_HEAD_UNIT * 2);
	v[5] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth,
		0.0f, 0.0f, m_stShape.fWidth, UV_HEAD_UNIT * 4, UV_HEAD_UNIT * 2);
	v[6] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth,
		0.0f, 0.0f, m_stShape.fWidth, UV_HEAD_UNIT * 4, UV_HEAD_UNIT * 1);
	v[7] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth,
		0.0f, 0.0f, m_stShape.fWidth, UV_HEAD_UNIT * 3, UV_HEAD_UNIT * 1);

	// fill in the top face vertex data
	v[8] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth,
		0.0f, m_stShape.fTop, 0.0f, UV_HEAD_UNIT, UV_HEAD_UNIT);
	v[9] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth,
		0.0f, m_stShape.fTop, 0.0f, UV_HEAD_UNIT * 2, UV_HEAD_UNIT);
	v[10] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth,
		0.0f, m_stShape.fTop, 0.0f, UV_HEAD_UNIT * 2, 0.0f);
	v[11] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth,
		0.0f, m_stShape.fTop, 0.0f, UV_HEAD_UNIT, 0.0f);

	// fill in the bottom face vertex data
	//x�� ������ �� ����
	//z�� ������ �� ��

	//���� ��
	v[12] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth,
		0.0f, -m_stShape.fBottom, 0.0f, UV_HEAD_UNIT * 2, 0.f);
	//������ ��
	v[13] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth,
		0.0f, -m_stShape.fBottom, 0.0f, UV_HEAD_UNIT * 3, 0.f);
	//������ �Ʒ�
	v[14] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth,
		0.0f, -m_stShape.fBottom, 0.0f, UV_HEAD_UNIT * 3, UV_HEAD_UNIT * 1);
	//���� �Ʒ�
	v[15] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth,
		0.0f, -m_stShape.fBottom, 0.0f, UV_HEAD_UNIT * 2, UV_HEAD_UNIT * 1);

	// fill in the left face vertex data
	//���� �Ʒ�
	v[16] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth,
		-m_stShape.fWidth, 0.0f, 0.0f, 0.f, UV_HEAD_UNIT * 2);
	//���� ��
	v[17] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth,
		-m_stShape.fWidth, 0.0f, 0.0f, 0.f, UV_HEAD_UNIT * 1);
	//������ ��
	v[18] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth,
		-m_stShape.fWidth, 0.0f, 0.0f, UV_HEAD_UNIT, UV_HEAD_UNIT);
	//������ �Ʒ�
	v[19] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth,
		-m_stShape.fWidth, 0.0f, 0.0f, UV_HEAD_UNIT, UV_HEAD_UNIT * 2);

	// fill in the right face vertex data
	//������ �Ʒ�
	v[20] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth,
		m_stShape.fWidth, 0.0f, 0.0f, UV_HEAD_UNIT * 3, UV_HEAD_UNIT * 2);
	//���� �Ʒ�
	v[21] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth,
		m_stShape.fWidth, 0.0f, 0.0f, UV_HEAD_UNIT * 2, UV_HEAD_UNIT * 2);
	//���� ��
	v[22] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth,
		m_stShape.fWidth, 0.0f, 0.0f, UV_HEAD_UNIT * 2, UV_HEAD_UNIT * 1);
	//������ ��
	v[23] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth,
		m_stShape.fWidth, 0.0f, 0.0f, UV_HEAD_UNIT * 3, UV_HEAD_UNIT * 1);

	m_pVB->Unlock();

	m_iPrimitiveIndicesSize = 36 * sizeof(WORD);
	m_eIndexFormat = D3DFMT_INDEX16;
	m_iNumPrimitive = 1;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	//FACEINDICES16* pIndices = nullptr;

	WORD* i = 0;

	m_pIB->Lock(0, 0, (void**)&i, 0);

	// fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT   CVIBuffer_Ani_Cube::Set_Body_Vertex()
{

	//float m_stShape.fPos = pRadius[3];aswwaw
	tagAniVertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	//������ �Ʒ�
	v[0] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 5 + UV_BODY_WIDHT_CENTER, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_BODY_HEIGHT_CENTER);
	//��������
	v[1] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 5 + UV_BODY_WIDHT_CENTER, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	//������
	v[2] = tagAniVertex(1 * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 5, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	//���ʾƷ�
	v[3] = tagAniVertex(1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 5, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_BODY_HEIGHT_CENTER);

	// fill in the back face vertex data
	v[4] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 5 + UV_BODY_WIDHT_CENTER + UV_ARM_MID, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_BODY_HEIGHT_CENTER);
	v[5] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 5 + UV_BODY_WIDHT_CENTER + UV_ARM_MID + UV_BODY_WIDHT_CENTER, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_BODY_HEIGHT_CENTER);
	v[6] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 5 + UV_BODY_WIDHT_CENTER + UV_ARM_MID + UV_BODY_WIDHT_CENTER, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	v[7] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 5 + UV_BODY_WIDHT_CENTER + UV_ARM_MID, UV_HEAD_UNIT * 2 + UV_ARM_MID);

	// fill in the top face vertex data
	v[8] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, m_stShape.fTop, 0.0f,
		UV_ARM_MID * 5, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	v[9] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, m_stShape.fTop, 0.0f,
		UV_ARM_MID * 5 + UV_BODY_WIDHT_CENTER, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	v[10] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, m_stShape.fTop, 0.0f,
		UV_ARM_MID * 5 + UV_BODY_WIDHT_CENTER, UV_HEAD_UNIT * 2);
	v[11] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, m_stShape.fTop, 0.0f,
		UV_ARM_MID * 5, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_ARM_MID);

	// fill in the bottom face vertex data
	//x�� ������ �� ����
	//z�� ������ �� ��

	//���� ��
	v[12] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, -m_stShape.fBottom, 0.0f,
		UV_ARM_MID * 5, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	//������ ��
	v[13] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, -m_stShape.fBottom, 0.0f,
		UV_ARM_MID * 5 + UV_BODY_WIDHT_CENTER, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	//������ �Ʒ�
	v[14] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, -m_stShape.fBottom, 0.0f,
		UV_ARM_MID * 5 + UV_BODY_WIDHT_CENTER, UV_HEAD_UNIT * 2);
	//���� �Ʒ�
	v[15] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, -m_stShape.fBottom, 0.0f,
		UV_ARM_MID * 5, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_ARM_MID);

	// fill in the left face vertex data
	//���� �Ʒ�
	v[16] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 4, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_BODY_HEIGHT_CENTER);
	//���� ��
	v[17] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 4, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	//������ ��
	v[18] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 4 + UV_ARM_MID, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	//������ �Ʒ�
	v[19] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 4 + UV_ARM_MID, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_BODY_HEIGHT_CENTER);

	// fill in the right face vertex data
	//������ �Ʒ�
	v[20] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 6 + UV_BODY_WIDHT_CENTER, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_BODY_HEIGHT_CENTER);
	//���� �Ʒ�
	v[21] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 5 + UV_BODY_WIDHT_CENTER, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_BODY_HEIGHT_CENTER);
	//���� ��
	v[22] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 5 + UV_BODY_WIDHT_CENTER, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	//������ ��
	v[23] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 6 + UV_BODY_WIDHT_CENTER, UV_HEAD_UNIT * 2 + UV_ARM_MID);

	m_pVB->Unlock();

	m_iPrimitiveIndicesSize = 36 * sizeof(WORD);
	m_eIndexFormat = D3DFMT_INDEX16;
	m_iNumPrimitive = 1;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	//FACEINDICES16* pIndices = nullptr;

	WORD* i = 0;

	m_pIB->Lock(0, 0, (void**)&i, 0);

	// fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT    CVIBuffer_Ani_Cube::Set_Leg_Vertex()
{

	//float m_stShape.fPos = pRadius[3];aswwaw
	tagAniVertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	//������ �Ʒ�
	v[0] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_ARM_BIG);
	//��������
	v[1] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	//������
	v[2] = tagAniVertex(1 * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 1, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	//���ʾƷ�
	v[3] = tagAniVertex(1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 1, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_ARM_BIG);

	// fill in the back face vertex data
	v[4] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 3, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_ARM_BIG);
	v[5] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 4, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_ARM_BIG);
	v[6] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 4, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	v[7] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 3, UV_HEAD_UNIT * 2 + UV_ARM_MID);

	// fill in the top face vertex data
	v[8] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, m_stShape.fTop, 0.0f,
		UV_ARM_MID, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	v[9] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, m_stShape.fTop, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	v[10] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, m_stShape.fTop, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	v[11] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, m_stShape.fTop, 0.0f,
		UV_ARM_MID, UV_HEAD_UNIT * 2);

	// fill in the bottom face vertex data
	//x�� ������ �� ����
	//z�� ������ �� ��

	//���� ��
	v[12] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, -m_stShape.fBottom, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	//������ ��
	v[13] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, -m_stShape.fBottom, 0.0f,
		UV_ARM_MID * 3, UV_HEAD_UNIT * 2);
	//������ �Ʒ�
	v[14] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, -m_stShape.fBottom, 0.0f,
		UV_ARM_MID * 3, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	//���� �Ʒ�
	v[15] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, -m_stShape.fBottom, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID);

	// fill in the left face vertex data
	//���� �Ʒ�
	v[16] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		0.f, UV_HEAD_UNIT + UV_ARM_MID * 1 + UV_ARM_BIG);
	//���� ��
	v[17] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		0.f, UV_HEAD_UNIT * 2 + UV_ARM_MID * 1);
	//������ ��
	v[18] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID, UV_HEAD_UNIT * 2 + UV_ARM_MID * 1);
	//������ �Ʒ�
	v[19] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID, UV_HEAD_UNIT + UV_ARM_MID * 1 + UV_ARM_BIG);

	// fill in the right face vertex data
	//������ �Ʒ�
	v[20] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 4, UV_HEAD_UNIT * 2 + UV_ARM_MID * 1 + UV_ARM_BIG);
	//���� �Ʒ�
	v[21] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 3, UV_HEAD_UNIT * 2 + UV_ARM_MID * 1 + UV_ARM_BIG);
	//���� ��
	v[22] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 3, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	//������ ��
	v[23] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 4, UV_HEAD_UNIT * 2 + UV_ARM_MID);

	m_pVB->Unlock();

	m_iPrimitiveIndicesSize = 36 * sizeof(WORD);
	m_eIndexFormat = D3DFMT_INDEX16;
	m_iNumPrimitive = 1;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	//FACEINDICES16* pIndices = nullptr;

	WORD* i = 0;

	m_pIB->Lock(0, 0, (void**)&i, 0);

	// fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT    CVIBuffer_Ani_Cube::Set_Arm_Vertex()
{

	//float m_stShape.fPos = pRadius[3];aswwaw
	tagAniVertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	//������ �Ʒ�
	v[0] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 8 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_BODY_HEIGHT_CENTER);
	//��������
	v[1] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 8 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	//������
	v[2] = tagAniVertex(1 * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 7 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	//���ʾƷ�
	v[3] = tagAniVertex(1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 7 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_BODY_HEIGHT_CENTER);

	// fill in the back face vertex data
	v[4] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 7 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_BODY_HEIGHT_CENTER);
	v[5] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 8 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_BODY_HEIGHT_CENTER);
	v[6] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 8 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	v[7] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 7 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID);

	// fill in the top face vertex data
	v[8] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 1.f, 0.0f,
		UV_ARM_MID * 7 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	v[9] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 1.f, 0.0f,
		UV_ARM_MID * 7 + UV_BODY_WIDHT_CENTER * 2 + UV_ARM_MID, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	v[10] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 1.f, 0.0f,
		UV_ARM_MID * 7 + UV_BODY_WIDHT_CENTER * 2 + UV_ARM_MID, UV_HEAD_UNIT * 2);
	v[11] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 1.f, 0.0f,
		UV_ARM_MID * 7 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2);

	// fill in the bottom face vertex data
	//x�� ������ �� ����
	//z�� ������ �� ��

	//���� ��
	v[12] = tagAniVertex(-1.0f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.0f * m_stShape.fWidth, 0.0f, -1.0f, 0.0f, 0, 0);
	//������ ��
	v[13] = tagAniVertex(1.0f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.0f * m_stShape.fWidth, 0.0f, -1.0f, 0.0f, 0, 0);
	//������ �Ʒ�
	v[14] = tagAniVertex(1.0f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.0f * m_stShape.fWidth, 0.0f, -1.0f, 0.0f, 0, 0);
	//���� �Ʒ�
	v[15] = tagAniVertex(-1.0f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.0f * m_stShape.fWidth, 0.0f, -1.0f, 0.0f, 0, 0);

	// fill in the left face vertex data
	//���� �Ʒ�
	v[16] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 6 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_BODY_HEIGHT_CENTER);
	//���� ��
	v[17] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 6 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	//������ ��
	v[18] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 7 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	//������ �Ʒ�
	v[19] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 7 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_BODY_HEIGHT_CENTER);

	// fill in the right face vertex data
	//������ �Ʒ�
	v[20] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 9 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_BODY_HEIGHT_CENTER);
	//���� �Ʒ�
	v[21] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 8 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID + UV_BODY_HEIGHT_CENTER);
	//���� ��
	v[22] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 8 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID);
	//������ ��
	v[23] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 9 + UV_BODY_WIDHT_CENTER * 2, UV_HEAD_UNIT * 2 + UV_ARM_MID);

	m_pVB->Unlock();

	m_iPrimitiveIndicesSize = 36 * sizeof(WORD);
	m_eIndexFormat = D3DFMT_INDEX16;
	m_iNumPrimitive = 1;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	//FACEINDICES16* pIndices = nullptr;

	WORD* i = 0;

	m_pIB->Lock(0, 0, (void**)&i, 0);

	// fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Ani_Cube::Set_Default_Vertex()
{
	m_iNumVertices = 24;
	m_iStride = sizeof(tagAniVertex);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;

	m_dwFVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	// Create texture.

	D3DXCreateTextureFromFile(
		m_pGraphic_Device,
		L"../Bin/Resources/Ani/CKMan.jpg",
		&m_pTexture);

	tagAniVertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	// build box
	// fill in the front face vertex data
	//������ �Ʒ�
	v[0] = tagAniVertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.f, 0.f * 2);
	//��������
	v[1] = tagAniVertex(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.f, 0.f);
	//������
	v[2] = tagAniVertex(0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.f, 0.f);
	//���ʾƷ�
	v[3] = tagAniVertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, 0.f, 0.f);

	// fill in the back face vertex data
	v[4] = tagAniVertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.f, 0.f);
	v[5] = tagAniVertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.f, 0.f);
	v[6] = tagAniVertex(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.f, 0.f);
	v[7] = tagAniVertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.f, 0.f);

	// fill in the top face vertex data
	v[8] = tagAniVertex(-0.5f, 0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 0.f, 0.f);
	v[9] = tagAniVertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f, 0.f, 0.f);
	v[10] = tagAniVertex(0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f, 0.f, 0.0f);
	v[11] = tagAniVertex(0.5f, 0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 0.f, 0.0f);

	// fill in the bottom face vertex data
	//x�� ������ �� ����
	//z�� ������ �� ��

	//���� ��
	v[12] = tagAniVertex(-0.5f, -0.5f, -0.5f, 0.0f, -0.5f, 0.0f, 0.f, 0.f);
	//������ ��
	v[13] = tagAniVertex(0.5f, -0.5f, -0.5f, 0.0f, -0.5f, 0.0f, 0.f, 0.f);
	//������ �Ʒ�
	v[14] = tagAniVertex(0.5f, -0.5f, 0.5f, 0.0f, -0.5f, 0.0f, 0.f, 0.f);
	//���� �Ʒ�
	v[15] = tagAniVertex(-0.5f, -0.5f, 0.5f, 0.0f, -0.5f, 0.0f, 0.f, 0.f);

	// fill in the left face vertex data
	//���� �Ʒ�
	v[16] = tagAniVertex(-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.f, 0.f);
	//���� ��
	v[17] = tagAniVertex(-0.5f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.f, 0.f);
	//������ ��
	v[18] = tagAniVertex(-0.5f, 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.f, 0.f);
	//������ �Ʒ�
	v[19] = tagAniVertex(-0.5f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.f, 0.f);

	// fill in the right face vertex data
	//������ �Ʒ�
	v[20] = tagAniVertex(0.5f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.f, 0.f);
	//���� �Ʒ�
	v[21] = tagAniVertex(0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.f, 0.f);
	//���� ��
	v[22] = tagAniVertex(0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.f, 0.f);
	//������ ��
	v[23] = tagAniVertex(0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.f, 0.f);

	m_pVB->Unlock();

	m_iPrimitiveIndicesSize = 36 * sizeof(WORD);
	m_eIndexFormat = D3DFMT_INDEX16;
	m_iNumPrimitive = 1;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	//FACEINDICES16* pIndices = nullptr;

	WORD* i = 0;

	m_pIB->Lock(0, 0, (void**)&i, 0);

	// fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT	   CVIBuffer_Ani_Cube::Set_Elbow_Vertex()
{
	//float m_stShape.fPos = pRadius[3];aswwaw
	tagAniVertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	//������ �Ʒ�
	v[0] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	//��������
	v[1] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	//������
	v[2] = tagAniVertex(1 * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	//���ʾƷ�
	v[3] = tagAniVertex(1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);

	// fill in the back face vertex data
	v[4] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	v[5] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	v[6] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	v[7] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);

	// fill in the top face vertex data
	v[8] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, m_stShape.fTop, 0.0f,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	v[9] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, m_stShape.fTop, 0.0f,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	v[10] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, m_stShape.fTop, 0.0f,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	v[11] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, m_stShape.fTop, 0.0f,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);

	// fill in the bottom face vertex data
	//x�� ������ �� ����
	//z�� ������ �� ��

	//���� ��
	v[12] = tagAniVertex(-1.0f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.0f * m_stShape.fWidth, 0.0f, -1.0f, 0.0f,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	//������ ��
	v[13] = tagAniVertex(1.0f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.0f * m_stShape.fWidth, 0.0f, -1.0f, 0.0f,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	//������ �Ʒ�
	v[14] = tagAniVertex(1.0f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.0f * m_stShape.fWidth, 0.0f, -1.0f, 0.0f,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	//���� �Ʒ�
	v[15] = tagAniVertex(-1.0f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.0f * m_stShape.fWidth, 0.0f, -1.0f, 0.0f,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);

	// fill in the left face vertex data
	//���� �Ʒ�
	v[16] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	//���� ��
	v[17] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	//������ ��
	v[18] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	//������ �Ʒ�
	v[19] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);

	// fill in the right face vertex data
	//������ �Ʒ�
	v[20] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	//���� �Ʒ�
	v[21] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	//���� ��
	v[22] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);
	//������ ��
	v[23] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 12, UV_HEAD_UNIT * 2);

	m_pVB->Unlock();

	m_iPrimitiveIndicesSize = 36 * sizeof(WORD);
	m_eIndexFormat = D3DFMT_INDEX16;
	m_iNumPrimitive = 1;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	//FACEINDICES16* pIndices = nullptr;

	WORD* i = 0;

	m_pIB->Lock(0, 0, (void**)&i, 0);

	// fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT	   CVIBuffer_Ani_Cube::Set_Ankle_Vertex()
{
	//float m_stShape.fPos = pRadius[3];aswwaw
	tagAniVertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	//������ �Ʒ�
	v[0] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	//��������
	v[1] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	//������
	v[2] = tagAniVertex(1 * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	//���ʾƷ�
	v[3] = tagAniVertex(1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, 0.0f, -m_stShape.fWidth,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);

	// fill in the back face vertex data
	v[4] = tagAniVertex(-1 * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	v[5] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	v[6] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	v[7] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, 0.0f, m_stShape.fWidth,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);

	// fill in the top face vertex data
	v[8] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, m_stShape.fTop, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	v[9] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, m_stShape.fTop, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	v[10] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, 0.0f, m_stShape.fTop, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	v[11] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, 0.0f, m_stShape.fTop, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);

	// fill in the bottom face vertex data
	//x�� ������ �� ����
	//z�� ������ �� ��

	//���� ��
	v[12] = tagAniVertex(-1.0f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.0f * m_stShape.fWidth, 0.0f, -1.0f, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	//������ ��
	v[13] = tagAniVertex(1.0f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.0f * m_stShape.fWidth, 0.0f, -1.0f, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	//������ �Ʒ�
	v[14] = tagAniVertex(1.0f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.0f * m_stShape.fWidth, 0.0f, -1.0f, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	//���� �Ʒ�
	v[15] = tagAniVertex(-1.0f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.0f * m_stShape.fWidth, 0.0f, -1.0f, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);

	// fill in the left face vertex data
	//���� �Ʒ�
	v[16] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	//���� ��
	v[17] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	//������ ��
	v[18] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	//������ �Ʒ�
	v[19] = tagAniVertex(-1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, -m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);

	// fill in the right face vertex data
	//������ �Ʒ�
	v[20] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, 1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	//���� �Ʒ�
	v[21] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fBottom + m_stShape.fPos, -1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	//���� ��
	v[22] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, -1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);
	//������ ��
	v[23] = tagAniVertex(1.f * m_stShape.fWidth, m_stShape.fTop + m_stShape.fPos, 1.f * m_stShape.fWidth, m_stShape.fWidth, 0.0f, 0.0f,
		UV_ARM_MID * 2, UV_HEAD_UNIT * 2);

	m_pVB->Unlock();

	m_iPrimitiveIndicesSize = 36 * sizeof(WORD);
	m_eIndexFormat = D3DFMT_INDEX16;
	m_iNumPrimitive = 1;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	//FACEINDICES16* pIndices = nullptr;

	WORD* i = 0;

	m_pIB->Lock(0, 0, (void**)&i, 0);

	// fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	m_pIB->Unlock();

	return S_OK;
}