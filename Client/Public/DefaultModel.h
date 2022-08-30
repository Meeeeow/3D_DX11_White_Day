#pragma once

#include "ModelObject.h"
BEGIN(Client)
class CDefaultModel final : public CModelObject
{
private:
	CDefaultModel(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CDefaultModel(const CDefaultModel& rhs);
	virtual ~CDefaultModel() DEFAULT;

public:
	virtual HRESULT				NativeConstruct();
	virtual HRESULT				NativeConstruct(OBJ_INFO tObjInfo);
	virtual HRESULT				Clone_Construct(void* pArg) override;
	virtual _int				Tick(_double dTimeDelta) override;
	virtual _int				LateTick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	virtual HRESULT				Set_Components() override;

public:
	static	CDefaultModel*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	static	CDefaultModel*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, OBJ_INFO tObjInfo);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free() override;
	virtual void				Compute_Distance() override;
};

END