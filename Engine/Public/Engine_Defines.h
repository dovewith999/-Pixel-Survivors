#pragma once

#pragma warning (disable : 4251)

#include <d3d9.h>
#include <d3dx9.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <iostream>
//hhlee header
#include <fstream>
#include <atlstr.h> 
//hhlee Define for Culling
#define PLANE_EPSILON     5.0f

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

#include "Engine_Macro.h"
#include "Engine_Function.h"
#include "Engine_TypeDef.h"
#include "Engine_Struct.h"
#include "Engine_FileInfo.h"
using namespace Engine;
using namespace std;

#define TERRAIN_CUBE_SIZE 1.f

/* Engine Enum */
enum TERRAINGROUP {
	GROUP_TERRAIN,
	GROUP_LT,
	GROUP_RT,
	GROUP_LB,
	GROUP_RB,
	GROUP_END = 800
};

typedef struct tagGameObjectFildInfo
{
	vector<vector<ANIMATIONDESC>> vecBoneTransform;
	vector<STSHAPE> vecAniCube;
	vector<TEST_STRUCT> vecAniInfo;
}ST_GAMEOBJECT_FILE_INFO;

typedef struct tagCubeBodyInfo
{
	float fWidth;
	float fTopHeight;
	float fBottomHeight;
	float fPos;
	ANI_CUBE eCube;
	CString strFilePath;
}CUBE_BODY_INFO;

typedef struct tagAniCubeInfo
{
	CUBE_BODY_INFO tCube_Body_Info;
	CString strImagePath;

	tagAniCubeInfo::tagAniCubeInfo()
	{

	}
}ANICUBEINFO;