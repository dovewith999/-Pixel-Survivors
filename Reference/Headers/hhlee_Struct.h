#pragma once

#include "hhlee_Define.h"
#include <string>

namespace Engine
{
	struct tagAniVertex
	{
		tagAniVertex() {}
		tagAniVertex(
			float x, float y, float z,
			float nx, float ny, float nz,
			float u, float v)
		{
			_x = x;  _y = y;  _z = z;
			_nx = nx; _ny = ny; _nz = nz;
			_u = u;  _v = v;
		}
		float _x, _y, _z;
		float _nx, _ny, _nz;
		float _u, _v; // texture coordinates
	};

	typedef struct tagTestStruct
	{
		unsigned long uMotion = 0;

		unsigned long uDir[ANIMATION_STATE_END];

		float fAngle[ANIMATION_STATE_END];
		unsigned long uFlag[ANIMATION_STATE_END];
		D3DXVECTOR3 m_varr[ANIMATION_STATE_END];
		float m_farr[ANIMATION_STATE_END];

		tagTestStruct::tagTestStruct()
		{
			for (int i = 0; i < ANIMATION_STATE_END; ++i)
			{
				uDir[i] = 0x00;
				fAngle[i] = 0.f;
				uFlag[i] = 0x00;
				m_varr[i] = D3DXVECTOR3{ 0.f, 0.f, 0.f };
				m_farr[i] = 0.f;
			}
		}
	}TEST_STRUCT;

	typedef struct tagAnimationDesc
	{
		float      fDir = 1.f;
		float		fMaxAngle = 0.f;
		float		fLocalAngle = 0.f;//비저장 데이터
		D3DXVECTOR3		vLocalPos;//비저장데이터
		D3DXVECTOR3     vAxis = D3DXVECTOR3{ 1.f, 0.f, 0.f };//비저장데이터
		float		fInitAngle = 0.f;
		float		fSpeedPerSec = 0.f;
		float		fRotationPerSec = 0.f;
		D3DXMATRIX   LocalMatrix;
		D3DXMATRIX   WorldMatrix;
		TCHAR*      szKey = NULL;

		tagAnimationDesc::tagAnimationDesc()
		{
			fDir = 0.f;
			fMaxAngle = 0.f;
			fLocalAngle = 0.f;
			vLocalPos = D3DXVECTOR3{ 0.f, 0.f, 0.f };
			vAxis = D3DXVECTOR3{ 0.f, 0.f, 0.f };
			fSpeedPerSec = 0.f;
			fRotationPerSec = 0.f;
			//ZeroMemory(&LocalMatrix, sizeof(D3DXMATRIX));
			//ZeroMemory(&WorldMatrix, sizeof(D3DXMATRIX));
			szKey = NULL;
		}
	}ANIMATIONDESC;

	typedef struct tagShape
	{
		float fWidth = 0.f;
		float fTop = 0.f;
		float fBottom = 0.f;
		float fPos = 0.f;

		tagShape::tagShape()
		{
			fWidth = 0.f;
			fTop = 0.f;
			fBottom = 0.f;
			fPos = 0.f;
		}
	}STSHAPE;
}