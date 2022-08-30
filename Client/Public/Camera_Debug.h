#pragma once
#ifndef __CLIENT_CAMERA_DEBUG_H__
#define __CLIENT_CAMERA_DEBUG_H__

#include "Camera.h"
BEGIN(Client)
class CCamera_Debug final : public CCamera
{
private:
	CCamera_Debug(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CCamera_Debug(const CCamera_Debug& rhs);
	virtual ~CCamera_Debug() DEFAULT;

public:
	enum TARGETLIST { TARGET_HEEMIN, TARGET_BONGGOO, TARGET_DALSU, TARGET_FUSEBOX, TARGET_VALVE0, TARGET_VALVE1, TARGET_END };

public:
	virtual	HRESULT			NativeConstruct() override;
	virtual HRESULT			Clone_Construct(void* pArg) override;
	virtual _int			Tick(_double dTimeDelta) override;
	virtual _int			LateTick(_double dTimeDelta) override;

private:
	HRESULT					Center(CGameInstance* pGameInstance, _double dTimeDelta);
	HRESULT					Trace(CGameInstance* pGameInstance, _double dTimeDelta);
	HRESULT					Check_MouseButtonKey(CGameInstance* pGameInstance, _double dTimeDelta);
	HRESULT					LookOrtho(_fvector& vLook);
	HRESULT					UpOrtho(_fvector& vUp);

	void					BongGooTrace(CGameInstance* pGameInstance, _double dTimeDelta);
	void					DalSuTrace(CGameInstance* pGameInstance, _double dTimeDelta);

	HRESULT					FuseBody(CGameInstance* pGameInstance, _double dTimeDelta);
	
	void					Valve0(CGameInstance* pGameInstance, _double dTimeDelta);
	void					Valve1(CGameInstance* pGameInstance, _double dTimeDelta);

public:
	void					Set_TargetOn(TARGETLIST eTarget);
	_bool					Get_TargetOn(TARGETLIST eTarget);
	_float4x4				Get_CameraMatrix();

private:
	void					Mouse_MoveControl();

private:
	_bool					m_bTarget[TARGET_END];
	_double					m_dwMouseX = 0.0;


public:
	static CCamera_Debug*	Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END
#endif // !__CLIENT_CAMERA_DEBUG_H__