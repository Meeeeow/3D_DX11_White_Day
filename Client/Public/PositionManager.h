#pragma once
#ifndef __CLIENT_POSITION_MANAGER_H__
#define __CLIENT_POSITION_MANAGER_H__

#include "Camera_Debug.h"
#include "LeeBongGoo.h"
#include "LeeHeeMin.h"
#include "SonDalSu.h"
#include "FuseBody.h"
#include "Valve.h"
#include "YooJiMin.h"

BEGIN(Client)
class CPositionManager final : public CBase
{
	DECLARE_SINGLETON(CPositionManager)
private:
	CPositionManager();
	virtual ~CPositionManager() DEFAULT;

public:
	void					Add_Time(_double dTime);

public:
	CCamera_Debug*			Get_Camera() { return m_pCamera; }

	_fvector				Get_PlayerPosition() { return XMLoadFloat4(&m_vPlayerPosition); }
	CLeeHeeMin*				Get_Player() { return m_pPlayer; }
	CTransform*				Get_PlayerTransform() { return m_pPlayerTransform; }
	CCollider*				Get_PlayerCollider() { return m_pPlayerCollider; }

	CLeeBongGoo*			Get_LeeBongGoo() { return m_pLeeBongGoo; }
	CTransform*				Get_LeeBongGooTransform() { return m_pLeeBongGooTransform; }
	CCollider*				Get_LeeBongGooCollider() { return m_pLeeBongGooCollider; }

	CSonDalSu*				Get_SonDalSu() { return m_pSonDalSu; }
	CTransform*				Get_SonDalSuTransform() { return m_pSonDalSuTransform; }
	CCollider*				Get_SonDalSuCollider() { return m_pSonDalSuCollider; }

	CFuseBody*				Get_FuseBody() { return m_pFuseBody; }
	_bool					Get_FuseClear() { return m_bFuseClear; }
	void					Set_FuseClear() { m_bFuseClear = true; }

	CValve*					Get_Valve0() { return m_pValve0; }
	CValve*					Get_Valve1() { return m_pValve1; }

	CYooJiMin*				Get_YooJiMin() { return m_pYooJiMin; }

	void					LeeBongGooDead();
	void					SonDalSuDead();

	_bool					LeeBongGooAlive() { return m_bLeeBongGooDead; }
	_bool					SonDalSuAlive() { return m_bSonDalSuDead; }

public:
	void					Set_Camera(CCamera_Debug* pCamera);
	void					Set_Player(CLeeHeeMin* pPlayer);
	void					Set_PlayerPosition(_fvector vPosition) { XMStoreFloat4(&m_vPlayerPosition, vPosition); }
	void					Set_LeeBongGoo(CLeeBongGoo* pLeeBongGoo);
	void					Set_SonDalSu(CSonDalSu* pSonDalSu);
	void					Set_FuseBox(CFuseBody* pFuseBody);
	void					Set_Valve0(CValve* pValve0);
	void					Set_Valve1(CValve* pValve1);
	void					Set_YooJiMin(CYooJiMin* pYooJiMin);


public:
	_float4					m_vPlayerPosition;

	CCamera_Debug*			m_pCamera = nullptr;

	CLeeHeeMin*				m_pPlayer = nullptr;
	CTransform*				m_pPlayerTransform = nullptr;
	CCollider*				m_pPlayerCollider = nullptr;

	CLeeBongGoo*			m_pLeeBongGoo = nullptr;
	CTransform*				m_pLeeBongGooTransform = nullptr;
	CCollider*				m_pLeeBongGooCollider = nullptr;
	_bool					m_bLeeBongGooDead = false;

	CSonDalSu*				m_pSonDalSu = nullptr;
	CTransform*				m_pSonDalSuTransform = nullptr;
	CCollider*				m_pSonDalSuCollider = nullptr;
	_bool					m_bSonDalSuDead = false;

	CFuseBody*				m_pFuseBody = nullptr;
	_bool					m_bFuseClear = false;

	CValve*					m_pValve0 = nullptr;
	_bool					m_bValve0Clear = false;
	CValve*					m_pValve1 = nullptr;
	_bool					m_bValve1Clear = false;

	CYooJiMin*				m_pYooJiMin = nullptr;


public:
	virtual void			Free();

};
END
#endif // !__CLIENT_POSITION_MANAGER_H__