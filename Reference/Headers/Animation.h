#pragma once
#ifndef __ENGINE_ANIMATION_H__
#define __ENGINE_ANIMATION_H__

#include "Channel.h"

BEGIN(Engine)
class CAnimation final : public CBase
{
public:
	/* 각 애니메이션에서 애니메이션의 상태(채널)를 들고 있는다.*/
	typedef vector<CChannel*>		CHANNELS;

private:
	explicit CAnimation();
	virtual ~CAnimation() DEFAULT;

public:
	HRESULT				NativeConstruct(_double dDuration, _double dPlayerSpeedPerSec);
	HRESULT				Add_Channel(CChannel* pChannel);

public:
	void				Update_TransformationMatrix(_double dTimeDelta);
	_matrix				Get_CurrentAnimMatrix();

private:
	CHANNELS			m_vecChannels;

private:
	_double				m_dDuration = 0.0;
	_double				m_dPlaySpeedPerSec = 0.0;
	_double				m_dTimeAcc = 0.0;
	_bool				m_bIsFinished = false;

public:
	static	CAnimation*	Create(_double dDuration, _double dPlayerSpeedPerSec);
	virtual void		Free() override;
};

END
#endif // !__ENGINE_ANIMATION_H__
