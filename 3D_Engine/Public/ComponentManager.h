#pragma once
#ifndef __ENGINE_COMPONENT_MANAGER_H__
#define __ENGINE_COMPONENT_MANAGER_H__

#include "Renderer.h"
#include "Collider.h"
#include "NavigationMesh.h"

#include "Transform.h"
#include "Texture.h"

#include "VIBuffer_RectXY.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Triangle.h"
#include "VIBuffer_Viewport.h"
#include "VIBufferCube.h"

#include "VIBuffer_Instance.h"
#include "InstanceBuffer_Rect.h"
#include "InstanceBuffer_Point.h"

#include "Model.h"
#include "AStar.h"


BEGIN(Engine)
class CComponentManager final : public CBase
{
	DECLARE_SINGLETON(CComponentManager)
public:
	typedef unordered_map<const _tchar*, CComponent*>		PROTOTYPES;

private:
	explicit CComponentManager();
	virtual ~CComponentManager() DEFAULT;

public:
	HRESULT				Reserve_Container(_uint iNumLevels);
	HRESULT				Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pInstance);
	CComponent*			Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);
	_bool				Prototype_NameCheck(_uint iLevelIndex, const _tchar* pPrototypeTag);
	void				Clear(_uint iLevelIndex);

private:
	PROTOTYPES*			m_pPrototypes = nullptr;
	_uint				m_iNumLevels = 0;

public:
	virtual void		Free() override;
};

END
#endif // !__ENGINE_COMPONENT_MANAGER_H__