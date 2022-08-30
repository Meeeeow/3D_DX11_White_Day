#pragma once
#ifndef __3D_ENGINE_DEFINES_H__
#define __3D_ENGINE_DEFINES_H__

#pragma warning (disable : 4251)


// DX 11 //////////////////////////
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <d3dcompiler.h>
#include <DirectXCollision.h>
#include <DirectXTK/Effects.h>
#include <DirectXTK/PrimitiveBatch.h>
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>
#include "d3dx11effect.h"
#include "DirectXTex.h"

using namespace DirectX;
using namespace PackedVector;
///////////////////////////////////

// Direct Input ///////////////////
#define DIRECTINPUT_VERSION	0x0800

#include <dinput.h>
///////////////////////////////////

#include <string>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <list>
#include <vector>
#include <typeinfo.h>
#include <process.h>
#include <map>

// Assimp /////////////////////////
#include "assimp\scene.h"
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
///////////////////////////////////

using namespace std;

#include "3DEngine_Macro.h"
#include "3DEngine_Typedef.h"
#include "3DEngine_Struct.h"
#include "3DEngine_Function.h"
#include "3DEngine_Number.h"

#ifndef _AFX
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif
#endif // _DEBUG
#endif // !_AFX

using namespace Engine;


#endif // !__3D_ENGINE_DEFINES_H__