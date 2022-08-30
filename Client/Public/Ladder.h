#pragma once
#ifndef __CLIENT_LADDER_H__
#define __CLIENT_LADDER_H__

#include "ModelObject.h"
BEGIN(Client)
class CLadder final : public CModelObject
{
private:
	CLadder(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CLadder(const CLadder& rhs);
	virtual ~CLadder() DEFAULT;

public:
	virtual	HRESULT			NativeConstruct() override;
	virtual HRESULT			Clone_Construct(void* pArg) override;
	virtual _int			Tick(_double dTimeDelta) override;
	virtual _int			LateTick(_double dTimeDelta) override;
	virtual HRESULT			Render() override;

public:
	virtual void			Interaction(CGameInstance* pGameInstance);
	virtual void			Event() override;
	virtual void			Collect_Event() override;

private:
	void					Carry(CGameInstance* pGameInstance, _double dTimeDelta);

private:
	_bool					m_bActive = false;


public:
	static	CLadder*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END
#endif // !__CLIENT_LADDER_H__