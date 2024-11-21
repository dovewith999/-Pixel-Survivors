#include "Component.h"
#include "Client_Defines.h"

namespace Engine
{
	class CVIBuffer_Cube;
	class CAniTransform;
}
BEGIN(Client)
class CBone : public CComponent
{
private:
	explicit CBone(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBone(const CBone& rhs);
	virtual ~CBone() = default;

public:
private:
	LPDIRECT3DDEVICE9 m_pGraphicDev = NULL;
	map<const TCHAR*,  CAniTransform*>* m_mapBone = NULL;
	typedef map<const TCHAR*,  CAniTransform*> MAP_BONE;
	map<const TCHAR*, CVIBuffer_Cube*>* m_mapCube = NULL;
	typedef map<const TCHAR*, CVIBuffer_Cube*> MAP_CUBE;
private:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
public:
	HRESULT Render();
public:
	HRESULT Add_Bone(BONE_LAYER _Layer, const TCHAR* _key, ANIMATIONDESC& Rhs, _float* fRadius = NULL);
	HRESULT Add_Bone(BONE_LAYER _Layer, const TCHAR* _key, ANIMATIONDESC& Rhs);

	CComponent* Find_Bone(BONE_LAYER _Layer, const TCHAR* _key);

	CComponent* Find_Cube(BONE_LAYER _Layer, const TCHAR* _key);

	HRESULT EraseBone(BONE_LAYER _Layer, const TCHAR* _key);

	HRESULT Setting_ParentBone(BONE_LAYER _Layer, const TCHAR* _parentkey, const TCHAR* _childkey);

	HRESULT Clear_BoneAngle(BONE_LAYER _Layer);
public:
	static CComponent * Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};
END