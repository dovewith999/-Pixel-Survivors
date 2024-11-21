#pragma once


namespace Engine
{
	typedef unsigned char				_ubyte;
	typedef signed char					_byte;

	typedef unsigned short				_ushort;
	typedef signed short				_short;	

	typedef unsigned long				_ulong;
	typedef signed long					_long;

	typedef unsigned int				_uint;
	typedef signed int					_int;

	typedef float						_float;
	typedef double						_double;

	typedef bool						_bool;
	typedef wchar_t						_tchar;

	typedef D3DXVECTOR2					_float2;

	//typedef struct tagVector3_Derived : public D3DXVECTOR3
	//{
	//	tagVector3_Derived() : D3DXVECTOR3() {};
	//	tagVector3_Derived(CONST FLOAT * pIn) : D3DXVECTOR3(pIn) {}
	//	tagVector3_Derived(CONST D3DVECTOR& In) : D3DXVECTOR3(In) {}
	//	tagVector3_Derived(CONST D3DXFLOAT16 * pIn) : D3DXVECTOR3(pIn) {}
	//	tagVector3_Derived(FLOAT x, FLOAT y, FLOAT z) : D3DXVECTOR3(x, y, z) {}

	//	float Get_Length()  { 
	//		return sqrtf(x * x + y * y + z * z);
	//	}

	//	float Get_Distance(const tagVector3_Derived* pTarget) {
	//		tagVector3_Derived		vResult(pTarget->x - x, pTarget->y - y, pTarget->z - z);
	//		return vResult.Get_Length();			
	//	}

	//	tagVector3_Derived Get_Look(const tagVector3_Derived* pTarget) {
	//		return tagVector3_Derived(pTarget->x - x, pTarget->y - y, pTarget->z - z);
	//	}

	//	tagVector3_Derived Get_TransfromCoord(const D3DXMATRIX* pTransformMatrix) {
	//		tagVector3_Derived		vResult;
	//		return *D3DXVec3TransformCoord(&vResult, this, pTransformMatrix);
	//	}
	//	tagVector3_Derived Get_TransfromNormal(const D3DXMATRIX* pTransformMatrix) {
	//		tagVector3_Derived		vResult;
	//		return *D3DXVec3TransformNormal(&vResult, this, pTransformMatrix);
	//	}
	//}_float3;		

	typedef D3DXVECTOR3					_float3;

	typedef D3DXVECTOR4					_float4;

	typedef D3DXMATRIX					_float4x4;
}