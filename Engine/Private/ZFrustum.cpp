#include "..\Public\ZFrustum.h"

IMPLEMENT_SINGLETON(ZFrustum_Manager)

ZFrustum_Manager::ZFrustum_Manager()
	: CBase()
{
	ZeroMemory(m_vtx, sizeof(m_vtx[0]) * 8);
	ZeroMemory(m_plane, sizeof(m_plane[0]) * 6);
}

void ZFrustum_Manager::Set_Camera(_float4x4* pArg)
{
	m_pMatViewProj = pArg;

	int i = 0;
	D3DXMATRIXA16 matInv;

	// ������ı��� ��ġ�� ��� 3���� ������ǥ�� ���� (-1,-1,0) ~ (1,1,1)������ ������ �ٲ��.
	// m_vtx�� �� ���������� ��谪�� �־�д�.
	m_vtx[0].x = -1.0f;     m_vtx[0].y = -1.0f;     m_vtx[0].z = 0.0f; //�ٴ� ����� �Ʒ� ����

	m_vtx[1].x = 1.0f;     m_vtx[1].y = -1.0f;     m_vtx[1].z = 0.0f;

	m_vtx[2].x = 1.0f;     m_vtx[2].y = -1.0f;     m_vtx[2].z = 1.0f; //�ٴ� ����� �� ����

	m_vtx[3].x = -1.0f;     m_vtx[3].y = -1.0f;     m_vtx[3].z = 1.0f;

	m_vtx[4].x = -1.0f;     m_vtx[4].y = 1.0f;     m_vtx[4].z = 0.0f; //õ�� ����� �Ʒ� ����

	m_vtx[5].x = 1.0f;     m_vtx[5].y = 1.0f;     m_vtx[5].z = 0.0f;

	m_vtx[6].x = 1.0f;     m_vtx[6].y = 1.0f;     m_vtx[6].z = 1.0f; //õ�� ����� �� ����

	m_vtx[7].x = -1.0f;     m_vtx[7].y = 1.0f;     m_vtx[7].z = 1.0f;

	// view * proj�� ������� ���Ѵ�.
	D3DXMatrixInverse(&matInv, NULL, m_pMatViewProj);

	// Vertex_���� = Vertex_local * Matrix_world * Matrix_view * Matrix_Proj �ε�,

	// Vertex_world = Vertex_local * Matrix_world�̹Ƿ�,

	// Vertex_���� = Vertex_world * Matrix_view * Matrix_Proj �̴�.

	// Vertex_���� = Vertex_world * ( Matrix_view * Matrix_Proj ) ����

	// �����( Matrix_view * Matrix_Proj )^-1�� �纯�� ���ϸ�

	// Vertex_���� * �����( Matrix_view * Matrix_Proj )^-1 = Vertex_World �� �ȴ�.

	// �׷��Ƿ�, m_vtx * matInv = Vertex_world�� �Ǿ�, ������ǥ���� �������� ��ǥ�� ���� �� �ִ�.

	//�� �������� �������� ��ǥ�� �����ϰ�, �� ������ ����� �ű��.
	for (i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&m_vtx[i], &m_vtx[i], &matInv);

	m_vPos = (m_vtx[0] + m_vtx[5]) / 2.0f;

	// ����� ������ǥ�� �������� ����� �����

	// ���Ͱ� �������� ���ʿ��� �ٱ������� ������ �����̴�.

	D3DXPlaneFromPoints(&m_plane[0], m_vtx + 4, m_vtx + 7, m_vtx + 6);    // �� ���(top)

	D3DXPlaneFromPoints(&m_plane[1], m_vtx, m_vtx + 1, m_vtx + 2);    // �� ���(bottom)

	D3DXPlaneFromPoints(&m_plane[2], m_vtx, m_vtx + 4, m_vtx + 5);    // �� ���(near)

	D3DXPlaneFromPoints(&m_plane[3], m_vtx + 2, m_vtx + 6, m_vtx + 7);    // �� ���(far)

	D3DXPlaneFromPoints(&m_plane[4], m_vtx, m_vtx + 3, m_vtx + 7);    // �� ���(left)

	D3DXPlaneFromPoints(&m_plane[5], m_vtx + 1, m_vtx + 5, m_vtx + 6);    // �� ���(ri
}
/// ���� v�� �������Ҿȿ� ������ TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.

_bool ZFrustum_Manager::IsIn(_float3* pv)

{
	float           fDist;
	int             i;

	// ����� left, right, far plane�� �����Ѵ�.
	for (i = 0; i < 6; i++)
	{
		fDist = D3DXPlaneDotCoord(&m_plane[0], pv);
		if (fDist > PLANE_EPSILON) return FALSE;       // plane�� normal���Ͱ� far�� ���ϰ� �����Ƿ� ����̸� ���������� �ٱ���
		fDist = D3DXPlaneDotCoord(&m_plane[1], pv);
		if (fDist > PLANE_EPSILON) return FALSE;       // plane�� normal���Ͱ� left�� ���ϰ� �����Ƿ� ����̸� ���������� ����
		fDist = D3DXPlaneDotCoord(&m_plane[2], pv);
		if (fDist > PLANE_EPSILON) return FALSE;       // plane�� normal���Ͱ� right�� ���ϰ� �����Ƿ� ����̸� ���������� ������
		fDist = D3DXPlaneDotCoord(&m_plane[3], pv);
		if (fDist > PLANE_EPSILON) return FALSE;       // plane�� normal���Ͱ� far�� ���ϰ� �����Ƿ� ����̸� ���������� �ٱ���
		fDist = D3DXPlaneDotCoord(&m_plane[4], pv);
		if (fDist > PLANE_EPSILON) return FALSE;       // plane�� normal���Ͱ� left�� ���ϰ� �����Ƿ� ����̸� ���������� ����
		fDist = D3DXPlaneDotCoord(&m_plane[5], pv);
		if (fDist > PLANE_EPSILON) return FALSE;       // plane�� normal���Ͱ� right�� ���ϰ� �����Ƿ� ����̸� ���������� ������
	}

	//cout << "Is In Run" << endl;
	return TRUE;

}

BOOL ZFrustum_Manager::IsInSphere(D3DXVECTOR3* pv, float radius)

{
	float           fDist;

	fDist = D3DXPlaneDotCoord(&m_plane[3], pv);

	if (fDist > (radius + PLANE_EPSILON)) return FALSE;      // ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����

	fDist = D3DXPlaneDotCoord(&m_plane[4], pv);

	if (fDist > (radius + PLANE_EPSILON)) return FALSE;      // ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����

	fDist = D3DXPlaneDotCoord(&m_plane[5], pv);

	if (fDist > (radius + PLANE_EPSILON)) return FALSE;      // ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����

	return TRUE;

}

void	ZFrustum_Manager::Tick()
{
	int i = 0;
	D3DXMATRIXA16 matInv;

	m_vtx[0].x = -1.0f;     m_vtx[0].y = -1.0f;     m_vtx[0].z = 0.0f; //�ٴ� ����� �Ʒ� ����

	m_vtx[1].x = 1.0f;     m_vtx[1].y = -1.0f;     m_vtx[1].z = 0.0f;

	m_vtx[2].x = 1.0f;     m_vtx[2].y = -1.0f;     m_vtx[2].z = 1.0f; //�ٴ� ����� �� ����

	m_vtx[3].x = -1.0f;     m_vtx[3].y = -1.0f;     m_vtx[3].z = 1.0f;

	m_vtx[4].x = -1.0f;     m_vtx[4].y = 1.0f;     m_vtx[4].z = 0.0f; //õ�� ����� �Ʒ� ����

	m_vtx[5].x = 1.0f;     m_vtx[5].y = 1.0f;     m_vtx[5].z = 0.0f;

	m_vtx[6].x = 1.0f;     m_vtx[6].y = 1.0f;     m_vtx[6].z = 1.0f; //õ�� ����� �� ����

	m_vtx[7].x = -1.0f;     m_vtx[7].y = 1.0f;     m_vtx[7].z = 1.0f;

	if (!m_pMatViewProj)
		return;

	D3DXMatrixInverse(&matInv, NULL, m_pMatViewProj);

	for (i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&m_vtx[i], &m_vtx[i], m_pMatViewProj);

	m_vPos = (m_vtx[0] + m_vtx[5]) / 2.0f;

	D3DXPlaneFromPoints(&m_plane[0], m_vtx + 4, m_vtx + 7, m_vtx + 6);    // �� ���(top)

	D3DXPlaneFromPoints(&m_plane[1], m_vtx, m_vtx + 1, m_vtx + 2);    // �� ���(bottom)

	D3DXPlaneFromPoints(&m_plane[2], m_vtx, m_vtx + 4, m_vtx + 5);    // �� ���(near)

	D3DXPlaneFromPoints(&m_plane[3], m_vtx + 2, m_vtx + 6, m_vtx + 7);    // �� ���(far)

	D3DXPlaneFromPoints(&m_plane[4], m_vtx, m_vtx + 3, m_vtx + 7);    // �� ���(left)

	D3DXPlaneFromPoints(&m_plane[5], m_vtx + 1, m_vtx + 5, m_vtx + 6);    // �� ���(ri

}

HRESULT ZFrustum_Manager::Render()
{
	if (GetAsyncKeyState(VK_TAB) < 0)
	{
		WORD index[] = { 0, 1, 2,
			0, 2, 3,
			4, 7, 6,
			4, 6, 5,
			1, 5, 6,
			1, 6, 2,
			0, 3, 7,
			0, 7, 4,
			0, 4, 5,
			0, 5, 1,
			3, 7, 6,
			3, 6, 2 };

		D3DMATERIAL9 mtrl;

		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));

		typedef struct tagVTX
		{

			D3DXVECTOR3     p;

		} VTX;

		VTX             vtx[8];

		for (int i = 0; i < 8; i++)
			vtx[i].p = m_vtx[i];

		m_pGraphic_Device->SetFVF(D3DFVF_XYZ);
		m_pGraphic_Device->SetStreamSource(0, NULL, 0, sizeof(VTX));
		m_pGraphic_Device->SetTexture(0, NULL);
		m_pGraphic_Device->SetIndices(0);
		/*
		Disables output from this texture stage and all stages with a higher index. To disable texture mapping, set this as the color operation for the first texture stage (stage 0). Alpha operations cannot be disabled when color operations are enabled. Setting the alpha operation to D3DTOP_DISABLE when color blending is enabled causes undefined behavior.
		*/
		//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		//m_pGraphic_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);



		// �Ķ������� ��,�� ����� �׸���.

		//m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
		mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
		m_pGraphic_Device->SetMaterial(&mtrl);

		m_pGraphic_Device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));

		// ������� ��,�� ����� �׸���.
		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
		mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
		m_pGraphic_Device->SetMaterial(&mtrl);
		m_pGraphic_Device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 4 * 3, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));

		// ���������� ��,�� ����� �׸���.

		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
		mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
		m_pGraphic_Device->SetMaterial(&mtrl);
		m_pGraphic_Device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 8 * 3, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

		//cout << "Render run" << endl;

	}
	return TRUE;
}

