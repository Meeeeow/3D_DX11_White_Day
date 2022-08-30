#pragma once
#ifndef __CLIENT_LEEBONGGOO_H__
#define __CLIENT_LEEBONGGOO_H__

#include "ModelObject.h"
BEGIN(Client)
class CLeeBongGoo final : public CModelObject
{
private:
	CLeeBongGoo(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CLeeBongGoo(const CLeeBongGoo& rhs);
	virtual ~CLeeBongGoo() DEFAULT;

public:
	enum BG_FSM_KEY { BG_KEY_IDLE, BG_KEY_WALK, BG_KEY_RUN, BG_KEY_ATTACK, BG_KEY_END };
	enum BG_FSM_STATE { BG_STATE_PATROL, BG_STATE_CHASE, BG_STATE_AMBUSH, BG_STATE_END };

public:
	virtual HRESULT			NativeConstruct() override;
	virtual HRESULT			Clone_Construct(void* pArg) override;
	virtual _int			Tick(_double dTimeDelta) override;
	virtual _int			LateTick(_double dTimeDelta) override;
	virtual HRESULT			Render() override;

public:
	void					View_Check(CGameInstance* pGameInstance, _double dTimeDelta);

private:
	HRESULT					Set_State(BG_FSM_STATE eState, BG_FSM_KEY eKey);

private:
	virtual HRESULT			Set_Components() override;

private:
	HRESULT					Move(CGameInstance* pGameInstance, _double dTimeDelta);

	HRESULT					DirectTrace(CGameInstance* pGameInstance, _double dTimeDelta);
	HRESULT					ManagerCall(CGameInstance* pGameInstance, _double dTimeDelta);
	HRESULT					Make_ManagerAStarRoute(CGameInstance* pGameInstance, _double dTimeDelta);

	HRESULT					RandomAStar(CGameInstance* pGameInstance, _double dTimeDelta);
	HRESULT					Make_RandomAStarRoute(CGameInstance* pGameInstance, _double dTimeDelta);

	HRESULT					Whistle(CGameInstance* pGameInstance, _double dTimeDelta);

	HRESULT					Attack(CGameInstance* pGameInstance, _double dTimeDelta);

public:
	HRESULT					Dead();

private:
	_uint					m_iAnimationIndex = 0;
	_float					m_fAlpha = 1.0f;
	_bool					m_bDead = false;
	_bool					m_bAlpha = false;

public:
	void					Add_ManagerTime(_double dTimeDelta);
	void					You_Collision() { m_bCollision = true; }

private:
	void					State_Patrol();
	void					State_Chase();
	void					State_Ambush();

private:
	CAStar*					m_pAStar = nullptr;

	BG_FSM_KEY				m_eKey = BG_KEY_END;
	BG_FSM_STATE			m_eState = BG_STATE_END;

	_bool					m_bAttack = false;
	_double					m_dAttackLimit = 0.0;

	_double					m_dManagerAStarTimeAcc = 0.0;
	_double					m_dManagerAStarLimitAcc = 0.0;
	_bool					m_bManagerAStar = false;
	_bool					m_bMakeManagerAStar = false;

	_double					m_dRandomAStarTimeAcc = 0.0;
	_double					m_dRandomAStarLimitAcc = 0.0;
	_bool					m_bRandomAStar = false;
	_bool					m_bMakeRandomAStar = false;

	_double					m_dTraceLimitAcc = 0.0;
	_bool					m_bFirstWhistle = false;
	_bool					m_bWhistle = false;
	_bool					m_bView = false;

	_bool					m_bCollision = false;

	_bool					m_bAmbush = false;
	_double					m_dAmbushLimit = 0.0;

	CCollider*				m_pViewCollider = nullptr;


public:
	static	CLeeBongGoo*	Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;


	// CModelObject을(를) 통해 상속됨
};
END
#endif // !__CLIENT_LEEBONGGOO_H__