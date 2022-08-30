#pragma once
#ifndef __CLIENT_WOMAN_TOILET_LADDER_H__
#define __CLIENT_WOMAN_TOILET_LADDER_H__

#include "ModelObject.h"
BEGIN(Client)
class CWomanToiletLadder final : public CModelObject
{
public:
	CWomanToiletLadder(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CWomanToiletLadder(const CWomanToiletLadder& rhs);
	virtual ~CWomanToiletLadder() DEFAULT;

public:
	virtual	HRESULT						NativeConstruct();
	virtual HRESULT						Clone_Construct(void* pArg);
	virtual _int						Tick(_double dTimeDelta);
	virtual _int						LateTick(_double dTimeDelta);
	virtual HRESULT						Render();

public:
	virtual void						Interaction() override;
	virtual void						Event() override;
	virtual void						Collect_Event() override;

	_bool								m_bAction = false;

public:
	static	CWomanToiletLadder*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual CGameObject*				Clone(void* pArg) override;
	virtual void						Free() override;
};
END
#endif // !__CLIENT_WOMAN_TOILET_LADDER_H__
