#pragma once
#ifndef __CLIENT_LEEHEEMIN_H__
#define __CLIENT_LEEHEEMIN_H__

#include "ModelObject.h"
BEGIN(Client)
class CLeeHeeMin final : public CModelObject
{
public:
	enum FSM_KEY { FSM_IDLE, FSM_WALK, FSM_RUN, FSM_END};
	enum FSM_STATE { FSM_UP, FSM_ONECARRY, FSM_TWOCARRY, FSM_SIT, FSM_JIMINCARRY,FSMSTATE_END };

private:
	CLeeHeeMin(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CLeeHeeMin(const CLeeHeeMin& rhs);
	virtual ~CLeeHeeMin() DEFAULT;

public:
	virtual	HRESULT				NativeConstruct() override;
	virtual HRESULT				Clone_Construct(void* pArg) override;
	virtual _int				Tick(_double dTimeDelta) override;
	virtual _int				LateTick(_double dTimeDelta) override;
	virtual HRESULT				Render();

public:
	void						Lighter(_double dTimeDelta);


public:
	HRESULT						Animation();
	BoundingOrientedBox*		Get_CurrentOBB();


public:
	void						TwoCarry_On() { m_bTwoCarry = true; }
	void						TwoCarry_Off() { m_bTwoCarry = false; }

	void						OneCarry_On() { m_bOneCarry = true; }
	void						OneCarry_Off() { m_bOneCarry = false; }

	_bool						Get_Run() { return m_bRun; }
	_bool						Get_Sit() { return m_bSit; }

	_bool						Get_Key() { return m_bKey; }
	void						Add_Key() { m_bKey = true; }

	_bool						Get_Lighter() { return m_bLighter; }
	void						Add_Lighter() { m_bGetLighter = true; }

	_bool						Get_UseTalisman() { return m_bUseTalisman; }
	void						Use_Talisman() { m_bUseTalisman = true; }

	_bool						Get_AddTalisman() { return m_bGetTalisman; }
	void						Add_Talisman() { m_bGetTalisman = true; }

	_bool						Get_Poison() { return m_bGetPoison; }
	void						Add_Poison() { m_bGetPoison = true; }

	_matrix						Get_BoneMatrix(const _tchar* pBoneTag);
	_matrix						Get_CurrentAnimMatrix();

	void						CanControl() { m_bCantControl = false; }
	void						CantControl() { m_bCantControl = true; }

	_bool						Get_Loading() { return m_bLoading; }
	void						Set_Loading(_bool bLoad) { m_bLoading = bLoad; }
	
	void						Set_JiminCarry() { m_bJiMinCarry = true; }

private:
	_bool						m_bRun = false;
	_bool						m_bTwoCarry = false;
	_bool						m_bOneCarry = false;
	_bool						m_bSit = false;
	_bool						m_bVent = false;

	_bool						m_bKey = false;
	_bool						m_bGetLighter = false;
	_bool						m_bLighter = false;
	_bool						m_bUseTalisman = false;
	_bool						m_bGetTalisman = false;
	_bool						m_bGetPoison = false;

	_bool						m_bCantControl = false;

	_bool						m_bHit = false;
	_double						m_dHitLimit = 0.0;

	_uint						m_iAnimation = 0;

	CCollider*					m_pSitCollider = nullptr;

	_bool						m_bLoading = false;

	_bool						m_bJiMinCarry = false;


public:
	HRESULT						Hit();

public:
	_bool						Get_Vent() { return m_bVent; }
	void						On_Vent() { m_bVent = true; }
	void						Off_Vent() { m_bVent = false; }

private:
	virtual HRESULT				Set_Components();

private:
	HRESULT						Check_Keyboard(CGameInstance* pGameInstance, _double dTimeDelta);
	HRESULT						Move(CGameInstance* pGameInstance, _double dTimeDelta);
	HRESULT						Move_Vent(CGameInstance* pGameInstance, _double dTimeDelta);
	HRESULT						Act(CGameInstance* pGameInstance, _double dTimeDelta);
	HRESULT						Mouse(CGameInstance* pGameInstance, _double dTimeDelta);

	HRESULT						State_Up();
	HRESULT						State_OneCarry();
	HRESULT						State_TwoCarry();
	HRESULT						State_Sit();
	HRESULT						State_Jimin();

private:
	HRESULT						Set_State(FSM_STATE eState, FSM_KEY eKey);

private:
	FSM_KEY						m_eKey;
	FSM_STATE					m_eState;

public:
	static	CLeeHeeMin*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual	CGameObject*		Clone(void* pArg) override;
	virtual	void				Free() override;

	// CModelObject을(를) 통해 상속됨
};
END
#endif // !__CLIENT_LEEHEEMIN_H__