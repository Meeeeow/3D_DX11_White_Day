#pragma once
#ifndef __CLIENT_DEFINES_COMPONENT_H__
#define __CLIENT_DEFINES_COMPONENT_H__

namespace Client {

	/* Component's Prototype Tag */
	static const wchar_t*	PrototypeRenderer = L"Prototype_Renderer";
	static const wchar_t*	PrototypeFrustum = L"Prototype_Frustum";
	static const wchar_t*	PrototypeTransform = L"Prototype_Transform";
	static const wchar_t*	PrototypeRectXY = L"Prototype_VIBuffer_RectXY";
	static const wchar_t*	PrototypeCube = L"Prototype_VIBuffer_Cube";
	static const wchar_t*	PrototypeAABB = L"Prototype_Collider_AABB";
	static const wchar_t*	PrototypeOBB = L"Prototype_Collider_OBB";
	static const wchar_t*	PrototypeSphere = L"Prototype_Collider_Sphere";
	static const wchar_t*	PrototypeTerrain = L"Prototype_VIBuffer_Terrain_Bone";
	static const wchar_t*	PrototypeMouseBuffer = L"Prototype_Mouse_Pointer_Buffer";
	static const wchar_t*	PrototypeAStar = L"Prototype_AStar";

	/* Component Map Tag */
	static const wchar_t*	ComRenderer = L"Component_Renderer";
	static const wchar_t*	ComTransform = L"Component_Transform";
	static const wchar_t*	ComCollider = L"Component_Collider";
	static const wchar_t*	ComFrustum = L"Component_Frustum";
	static const wchar_t*	ComVIBuffer = L"Component_VIBuffer";
	static const wchar_t*	ComTexture = L"Component_Texture";
	static const wchar_t*	ComAStar = L"Component_AStar";

}

#endif // !__CLIENT_DEFINES_COMPONENT_H__
