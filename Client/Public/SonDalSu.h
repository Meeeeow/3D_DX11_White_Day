#pragma once
#ifndef __CLIENT_SONDALSU_H__
#define __CLIENT_SONDALSU_H__


#include "ModelObject.h"
BEGIN(Client)
class CSonDalSu final : public CModelObject
{
private:
	CSonDalSu(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevcie);
	explicit CSonDalSu(const CSonDalSu& rhs);
	virtual ~CSonDalSu() DEFAULT;

public:
	virtual	HRESULT			NativeConstruct();
	virtual HRESULT			Clone_Construct(void* pArg);
	virtual _int			Tick(_double dTimeDelta);
	virtual _int			LateTick(_double dTimeDelta);
	virtual HRESULT			Render();


	HRESULT					Set_Components() override;
	void					Move(CGameInstance* pGameInstance, _double dTimeDelta);

public:
	HRESULT					Dead();

private:

	_bool					m_bChase = false;

	_bool					m_bAlpha = false;
	_float					m_fAlpha = 1.0f;

	_bool					m_bDead = false;


public:
	static	CSonDalSu*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;

};
END
#endif // !__CLIENT_SONDALSU_H__