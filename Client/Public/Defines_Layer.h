#pragma once
#ifndef __CLIENT_DEFINES_LAYER_H__
#define __CLIENT_DEFINES_LAYER_H__

namespace Client {
	/* Layer Tag */
	static const wchar_t*	LayerCamera = L"Layer_Camera";
	static const wchar_t*	LayerPlayer = L"Layer_Player";
	static const wchar_t*	LayerNonPlayable = L"Layer_NonPlayable";
	static const wchar_t*	LayerBackGround = L"Layer_BackGround";
	static const wchar_t*	LayerCollision = L"Layer_Collision";

	/* Shader - Client */
#ifdef _AFX
	static const wchar_t*	Shader_Model		= L"../Client/Bin/ShaderFiles/Shader_Model.hlsl";
	static const wchar_t*	Shader_Terrain		= L"../Client/Bin/ShaderFiles/Shader_Terrain.hlsl";
	static const wchar_t*	Shader_Rect			= L"../Client/Bin/ShaderFiles/Shader_Rect.hlsl";
	static const wchar_t*	Shader_Navigation	= L"../Client/Bin/ShaderFiles/Shader_NavigationMesh.hlsl";
	static const wchar_t*	Shader_Viewport		= L"../Client/Bin/ShaderFiles/Shader_Viewport.hlsl";
#else
	static const wchar_t*	Shader_Model		= L"../Bin/ShaderFiles/Shader_Model.hlsl";
	static const wchar_t*	Shader_Terrain		= L"../Bin/ShaderFiles/Shader_Terrain.hlsl";
	static const wchar_t*	Shader_Rect			= L"../Bin/ShaderFiles/Shader_Rect.hlsl";
	static const wchar_t*	Shader_Navigation	= L"../Bin/ShaderFiles/Shader_NavigationMesh.hlsl";
	static const wchar_t*	Shader_Viewport		= L"../Bin/ShaderFiles/Shader_Viewport.hlsl";
#endif // !_AFX
	/* Technique*/
	static const char*		DefaultTechnique = "Technique_Default";
}


#endif // !__CLIENT_DEFINES_LAYER_H__