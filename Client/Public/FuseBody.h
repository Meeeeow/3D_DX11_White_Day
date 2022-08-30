#pragma once
#ifndef __CLIENT_FUSE_BODY_H__
#define __CLIENT_FUSE_BODY_H__
#include "ModelObject.h"
BEGIN(Client)
class CFuseBody final : public CModelObject
{
private:
	CFuseBody(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CFuseBody() DEFAULT;
public:
	enum FUSENUM { FUSE_FIRST, FUSE_SECOND, FUSE_THIRD, FUSE_END };

public:
	virtual HRESULT				NativeConstruct(OBJ_INFO tObjInfo);
	virtual _int				Tick(_double dTimeDelta) override;
	virtual _int				LateTick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;

	
	void						Check_TrueFuse(CGameObject* pGameObject);
	void						Check_FalseFuse(CGameObject* pGameObject);

	_bool						m_bAction = false;
	void						TimeAccumulation(_double dTimeDelta);

private:
	CCollider*					m_pOnFuse[FUSE_END];
	_bool						m_bTrueFuse[FUSE_END];
	_bool						m_bDead = false;
	_double						m_dTime = 0.0;
public:
	static	CFuseBody*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, OBJ_INFO tObjInfo);
	virtual void				Free() override;
};
END
#endif // !__CLIENT_FUSE_BODY_H__