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

	//큐브 전체의 사이즈를 로드한 데이터로 만드는 함수
	HRESULT Set_CubeSize(D3DXVECTOR3 vCubeSize);

	HRESULT Set_CubeSizePlus(D3DXVECTOR3 vCubeSize);

	//큐브 전체의 버텍스를 로드한 벡터만큼 이동시키는 함수
	HRESULT Set_CubeVertexPos(D3DXVECTOR3 vCubeVertexPos);

	//큐브 전체를 특정 축으로 회전시킴
	HRESULT Set_CubeTurn(D3DXVECTOR3 vAxis, float fTimeDelta);

	//큐브 전체를 축기준으로 Angle각도만큼 회전시킴
	HRESULT Set_CubeTurnAngle(D3DXVECTOR3 vAxis,float fAngle);

	void Set_BindTransform(CTransform* pTransform);

	//클라이언트에서 자주 사용될 기능
public:
	//큐브 모습 보임 설정
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
	//리스트가 아닌 맵으로 만든 이유는 마우스 우클릭으로 큐브를 정해서 지워줄껀데 리스트면 그 큐브의 위치만의 리스트를 지우는 방도가없다


	//안에 들어갈수 있는 큐브 갯수를 정하는 사이즈
	D3DXVECTOR3 m_vSize;

	//총 전체 마스터큐브와 하위큐브들의 사이즈
	D3DXVECTOR3 m_vCubeSize{ 1.f, 1.f, 1.f };

	//버텍스들의 이동위치
	D3DXVECTOR3 m_vVertexPos{ 0.f,0.f,0.f };

	//안에 들어가 있는 큐브 갯수를 저장
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

