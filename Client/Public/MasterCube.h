#pragma once
#include "GameObject.h"
#include "Cube.h"
#include "Client_Defines.h"
#include "GameInstance.h"

BEGIN(Engine)
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CVIBuffer_ColorCube;
END

class CMasterCube : public CGameObject
{
public:
	explicit CMasterCube(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CMasterCube(const CMasterCube& rhs);
	virtual ~CMasterCube() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(float fTimeDelta) override;
	virtual void Late_Tick(float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Positon(D3DXVECTOR3 vPosition);
	void Move_Position(D3DXVECTOR3 vPosition);
	HRESULT Set_VertexPos(D3DXVECTOR3 vVertexPos);

	HRESULT Set_WorldSize(D3DXVECTOR3 vCubeSize);

	//ť�� ��ü�� ����� �ε��� �����ͷ� ����� �Լ�
	HRESULT Set_CubeSize(D3DXVECTOR3 vCubeSize);

	HRESULT Set_CubeSizePlus(D3DXVECTOR3 vCubeSize);

	//ť�� ��ü�� ���ؽ��� �ε��� ���͸�ŭ �̵���Ű�� �Լ�
	HRESULT Set_CubeVertexPos(D3DXVECTOR3 vCubeVertexPos);

	//ť�� ��ü�� Ư�� ������ ȸ����Ŵ
	HRESULT Set_CubeTurn(D3DXVECTOR3 vAxis, float fTimeDelta);

	//ť�� ��ü�� ��������� Angle������ŭ ȸ����Ŵ
	HRESULT Set_CubeTurnAngle(D3DXVECTOR3 vAxis,float fAngle);

	void Set_BindTransform(CTransform* pTransform);

	//Ŭ���̾�Ʈ���� ���� ���� ���
public:
	//ť�� ��� ���� ����
	void Set_CubeRender(bool bRender) {
		for (auto iter : m_mapCube)
		{
			iter.second->Set_CubeRender(bRender);
		} 
	}

public:
	void Add_Cube(int iIndex, D3DXVECTOR3 vSize, CCube* pCubeObj);
	int Cal_Index(D3DXVECTOR3 vLocation, D3DXVECTOR3 vSize);
	HRESULT Save_Data(HANDLE& hFile, DWORD& dwByte);

public:
	D3DXVECTOR3 Get_Position(void);
	D3DXVECTOR3 Get_Size(void) { return m_vSize; }
	int Get_CubeCount(void) { return (int)m_mapCube.size(); }

private:
	map<int, CCube*>	m_mapCube;
	typedef map<int, CCube*> CUBELIST;

	map<int, int>	m_CubeFill;
	//����Ʈ�� �ƴ� ������ ���� ������ ���콺 ��Ŭ������ ť�긦 ���ؼ� �����ٲ��� ����Ʈ�� �� ť���� ��ġ���� ����Ʈ�� ����� �浵������


	//�ȿ� ���� �ִ� ť�� ������ ���ϴ� ������
	D3DXVECTOR3 m_vSize;

	//�� ��ü ������ť��� ����ť����� ������
	D3DXVECTOR3 m_vCubeSize{ 1.f, 1.f, 1.f };

	//���ؽ����� �̵���ġ
	D3DXVECTOR3 m_vVertexPos{ 0.f,0.f,0.f };

	//�ȿ� �� �ִ� ť�� ������ ����
	int m_iCreateCount = 0;

private:
	CTransform* m_pTransformComponent = nullptr;
	CRenderer* m_pRendererComponent = nullptr;
	CVIBuffer_ColorCube* m_pVIBufferComponent = nullptr;
	//CGameInstance*		m_pGameInstance = nullptr;

private:
	HRESULT Set_Component();
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CMasterCube* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CMasterCube* Clone(void* pArg) override;
	virtual void Free() override;
};

