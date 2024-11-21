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

	// 투영행렬까지 거치면 모든 3차원 월드좌표의 점은 (-1,-1,0) ~ (1,1,1)사이의 값으로 바뀐다.
	// m_vtx에 이 동차공간의 경계값을 넣어둔다.
	m_vtx[0].x = -1.0f;     m_vtx[0].y = -1.0f;     m_vtx[0].z = 0.0f; //바닥 평면의 아래 선분

	m_vtx[1].x = 1.0f;     m_vtx[1].y = -1.0f;     m_vtx[1].z = 0.0f;

	m_vtx[2].x = 1.0f;     m_vtx[2].y = -1.0f;     m_vtx[2].z = 1.0f; //바닥 평면의 위 선분

	m_vtx[3].x = -1.0f;     m_vtx[3].y = -1.0f;     m_vtx[3].z = 1.0f;

	m_vtx[4].x = -1.0f;     m_vtx[4].y = 1.0f;     m_vtx[4].z = 0.0f; //천정 평면의 아래 선분

	m_vtx[5].x = 1.0f;     m_vtx[5].y = 1.0f;     m_vtx[5].z = 0.0f;

	m_vtx[6].x = 1.0f;     m_vtx[6].y = 1.0f;     m_vtx[6].z = 1.0f; //천정 평면의 위 선분

	m_vtx[7].x = -1.0f;     m_vtx[7].y = 1.0f;     m_vtx[7].z = 1.0f;

	// view * proj의 역행렬을 구한다.
	D3DXMatrixInverse(&matInv, NULL, m_pMatViewProj);

	// Vertex_최종 = Vertex_local * Matrix_world * Matrix_view * Matrix_Proj 인데,

	// Vertex_world = Vertex_local * Matrix_world이므로,

	// Vertex_최종 = Vertex_world * Matrix_view * Matrix_Proj 이다.

	// Vertex_최종 = Vertex_world * ( Matrix_view * Matrix_Proj ) 에서

	// 역행렬( Matrix_view * Matrix_Proj )^-1를 양변에 곱하면

	// Vertex_최종 * 역행렬( Matrix_view * Matrix_Proj )^-1 = Vertex_World 가 된다.

	// 그러므로, m_vtx * matInv = Vertex_world가 되어, 월드좌표계의 프러스텀 좌표를 얻을 수 있다.

	//뷰 공간상의 프러스텀 좌표를 설정하고, 그 공간을 월드로 옮긴다.
	for (i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&m_vtx[i], &m_vtx[i], &matInv);

	m_vPos = (m_vtx[0] + m_vtx[5]) / 2.0f;

	// 얻어진 월드좌표로 프러스텀 평면을 만든다

	// 벡터가 프러스텀 안쪽에서 바깥쪽으로 나가는 평면들이다.

	D3DXPlaneFromPoints(&m_plane[0], m_vtx + 4, m_vtx + 7, m_vtx + 6);    // 상 평면(top)

	D3DXPlaneFromPoints(&m_plane[1], m_vtx, m_vtx + 1, m_vtx + 2);    // 하 평면(bottom)

	D3DXPlaneFromPoints(&m_plane[2], m_vtx, m_vtx + 4, m_vtx + 5);    // 근 평면(near)

	D3DXPlaneFromPoints(&m_plane[3], m_vtx + 2, m_vtx + 6, m_vtx + 7);    // 원 평면(far)

	D3DXPlaneFromPoints(&m_plane[4], m_vtx, m_vtx + 3, m_vtx + 7);    // 좌 평면(left)

	D3DXPlaneFromPoints(&m_plane[5], m_vtx + 1, m_vtx + 5, m_vtx + 6);    // 우 평면(ri
}
/// 한점 v가 프러스텀안에 있으면 TRUE를 반환, 아니면 FALSE를 반환한다.

_bool ZFrustum_Manager::IsIn(_float3* pv)

{
	float           fDist;
	int             i;

	// 현재는 left, right, far plane만 적용한다.
	for (i = 0; i < 6; i++)
	{
		fDist = D3DXPlaneDotCoord(&m_plane[0], pv);
		if (fDist > PLANE_EPSILON) return FALSE;       // plane의 normal벡터가 far로 향하고 있으므로 양수이면 프러스텀의 바깥쪽
		fDist = D3DXPlaneDotCoord(&m_plane[1], pv);
		if (fDist > PLANE_EPSILON) return FALSE;       // plane의 normal벡터가 left로 향하고 있으므로 양수이면 프러스텀의 왼쪽
		fDist = D3DXPlaneDotCoord(&m_plane[2], pv);
		if (fDist > PLANE_EPSILON) return FALSE;       // plane의 normal벡터가 right로 향하고 있으므로 양수이면 프러스텀의 오른쪽
		fDist = D3DXPlaneDotCoord(&m_plane[3], pv);
		if (fDist > PLANE_EPSILON) return FALSE;       // plane의 normal벡터가 far로 향하고 있으므로 양수이면 프러스텀의 바깥쪽
		fDist = D3DXPlaneDotCoord(&m_plane[4], pv);
		if (fDist > PLANE_EPSILON) return FALSE;       // plane의 normal벡터가 left로 향하고 있으므로 양수이면 프러스텀의 왼쪽
		fDist = D3DXPlaneDotCoord(&m_plane[5], pv);
		if (fDist > PLANE_EPSILON) return FALSE;       // plane의 normal벡터가 right로 향하고 있으므로 양수이면 프러스텀의 오른쪽
	}

	//cout << "Is In Run" << endl;
	return TRUE;

}

BOOL ZFrustum_Manager::IsInSphere(D3DXVECTOR3* pv, float radius)

{
	float           fDist;

	fDist = D3DXPlaneDotCoord(&m_plane[3], pv);

	if (fDist > (radius + PLANE_EPSILON)) return FALSE;      // 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음

	fDist = D3DXPlaneDotCoord(&m_plane[4], pv);

	if (fDist > (radius + PLANE_EPSILON)) return FALSE;      // 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음

	fDist = D3DXPlaneDotCoord(&m_plane[5], pv);

	if (fDist > (radius + PLANE_EPSILON)) return FALSE;      // 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음

	return TRUE;

}

void	ZFrustum_Manager::Tick()
{
	int i = 0;
	D3DXMATRIXA16 matInv;

	m_vtx[0].x = -1.0f;     m_vtx[0].y = -1.0f;     m_vtx[0].z = 0.0f; //바닥 평면의 아래 선분

	m_vtx[1].x = 1.0f;     m_vtx[1].y = -1.0f;     m_vtx[1].z = 0.0f;

	m_vtx[2].x = 1.0f;     m_vtx[2].y = -1.0f;     m_vtx[2].z = 1.0f; //바닥 평면의 위 선분

	m_vtx[3].x = -1.0f;     m_vtx[3].y = -1.0f;     m_vtx[3].z = 1.0f;

	m_vtx[4].x = -1.0f;     m_vtx[4].y = 1.0f;     m_vtx[4].z = 0.0f; //천정 평면의 아래 선분

	m_vtx[5].x = 1.0f;     m_vtx[5].y = 1.0f;     m_vtx[5].z = 0.0f;

	m_vtx[6].x = 1.0f;     m_vtx[6].y = 1.0f;     m_vtx[6].z = 1.0f; //천정 평면의 위 선분

	m_vtx[7].x = -1.0f;     m_vtx[7].y = 1.0f;     m_vtx[7].z = 1.0f;

	if (!m_pMatViewProj)
		return;

	D3DXMatrixInverse(&matInv, NULL, m_pMatViewProj);

	for (i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&m_vtx[i], &m_vtx[i], m_pMatViewProj);

	m_vPos = (m_vtx[0] + m_vtx[5]) / 2.0f;

	D3DXPlaneFromPoints(&m_plane[0], m_vtx + 4, m_vtx + 7, m_vtx + 6);    // 상 평면(top)

	D3DXPlaneFromPoints(&m_plane[1], m_vtx, m_vtx + 1, m_vtx + 2);    // 하 평면(bottom)

	D3DXPlaneFromPoints(&m_plane[2], m_vtx, m_vtx + 4, m_vtx + 5);    // 근 평면(near)

	D3DXPlaneFromPoints(&m_plane[3], m_vtx + 2, m_vtx + 6, m_vtx + 7);    // 원 평면(far)

	D3DXPlaneFromPoints(&m_plane[4], m_vtx, m_vtx + 3, m_vtx + 7);    // 좌 평면(left)

	D3DXPlaneFromPoints(&m_plane[5], m_vtx + 1, m_vtx + 5, m_vtx + 6);    // 우 평면(ri

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



		// 파란색으로 상,하 평면을 그린다.

		//m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
		mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
		m_pGraphic_Device->SetMaterial(&mtrl);

		m_pGraphic_Device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));

		// 녹색으로 좌,우 평면을 그린다.
		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
		mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
		m_pGraphic_Device->SetMaterial(&mtrl);
		m_pGraphic_Device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 4 * 3, D3DFMT_INDEX16, vtx, sizeof(vtx[0]));

		// 붉은색으로 원,근 평면을 그린다.

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

