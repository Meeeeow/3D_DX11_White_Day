#pragma once
#ifndef __ENGINE_LAYER_H__
#define __ENGINE_LAYER_H__

#include "GameObject.h"

BEGIN(Engine)
class CLayer final : public CBase
{
public:
	typedef list<CGameObject*>			OBJECTS;

private:
	explicit CLayer();
	virtual ~CLayer() DEFAULT;

public:
	CGameObject*		Get_GameObject(_uint iIndex);

public:
	HRESULT				NativeConstruct();
	HRESULT				Add_GameObject(CGameObject* pGameObject);
	_int				Tick(_double dTimeDelta);
	_int				LateTick(_double dTimeDelta);
	_int				Collect_Event();
	OBJECTS*			Get_Objects() { return &m_listObjects; }
private:
	OBJECTS				m_listObjects;

public:
	static	CLayer*		Create();
	virtual	void		Free() override;

};
END
#endif // !__ENGINE_LAYER_H__