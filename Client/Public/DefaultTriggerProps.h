#pragma once
#ifndef __CLIENT_DEFAULT_TRIGGER_PROPS_H__
#define __CLIENT_DEFAULT_TRIGGER_PROPS_H__

#include "ModelObject.h"
BEGIN(Client)
class CDefaultTriggerProps final : public CModelObject
{
private:
	CDefaultTriggerProps(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CDefaultTriggerProps() DEFAULT;

public:
	enum TRIGGERTYPE { TRG_LADDER, TRG_POISON, TRG_LIGHTER, TRG_KEY, TRG_TALISMAN, TRG_FUSEBOX
		, TRG_END };

public:
	virtual	HRESULT						NativeConstruct(OBJ_INFO tObjInfo, TRIGGERTYPE eType);
	virtual	_int						Tick(_double dTimeDelta);
	virtual _int						LateTick(_double dTimeDelta);
	virtual HRESULT						Render() override;

public:
	virtual void						Interaction() override;
	virtual void						Event() override;
	virtual void						Collect_Event() override;

private:
	TRIGGERTYPE							m_eType;
	_bool								m_bAction = false;
	_bool								m_bDead = false;

public:
	static	CDefaultTriggerProps*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice
		, OBJ_INFO tObjInfo, TRIGGERTYPE eType);
	virtual	void						Free() override;
};
END
#endif // !__CLIENT_DEFAULT_TRIGGER_PROPS_H__