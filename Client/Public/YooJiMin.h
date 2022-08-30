#pragma once
#ifndef __CLIENT_YOOJIMIN_H__
#define __CLIENT_YOOJIMIN_H__


#include "ModelObject.h"
BEGIN(Client)
class CYooJiMin final : public CModelObject
{
private:
	CYooJiMin(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CYooJiMin(const CYooJiMin& rhs);
	virtual ~CYooJiMin() DEFAULT;

public:
	virtual HRESULT			NativeConstruct() override;
	virtual HRESULT			Clone_Construct(void* pArg) override;
	virtual _int			Tick(_double dTimeDelta) override;
	virtual _int			LateTick(_double dTimeDelta) override;
	virtual HRESULT			Render() override;

public:
	void					Set_DeadAnim() { m_pModel->Set_Animation(1); }

private:
	virtual HRESULT			Set_Components() override;



	virtual void			Interaction() override;
	virtual void			Event() override;
	virtual void			Collect_Event() override;

private:
	HRESULT					Update(CGameInstance* pGameInstance, _double dTimeDelta);
	_uint					m_iTalkCount = 0;

public:
	static	CYooJiMin*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END
#endif // !__CLIENT_YOOJIMIN_H__