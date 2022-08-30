#pragma once
#ifndef __CLIENT_LEVEL_QASMOKE_H__
#define __CLIENT_LEVEL_QASMOKE_H__

#include "Level.h"
BEGIN(Client)
class CLevel_Qasmoke final : public CLevel
{
private:
	CLevel_Qasmoke(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CLevel_Qasmoke() DEFAULT;

public:
	virtual	HRESULT					NativeConstruct(_uint iLevelID) override;
	virtual _int					Tick(_double dTimeDelta) override;
	virtual HRESULT					Render() override;

public:
	CCollider*						Get_Event1() { return m_pEvent1; }

private:
	HRESULT							Ready_Light();
	HRESULT							Ready_Camera(const _tchar* pLayerTag);
	HRESULT							Ready_Playable(const _tchar* pLayerTag);
	HRESULT							Ready_NonPlayable(const _tchar* pLayerTag);
	HRESULT							Ready_BackGround(const _tchar* pLayerTag);

public:
	void							Set_VentIndex(_fvector vPosition);
	void							Collision();

private:
	_float							m_fTimeAcc = 0.f;
	_double							m_dTimeAcc = 0.0;
	_bool							m_bReverse = false;
	CCollider*						m_pEvent1 = nullptr;
	CCollider*						m_pEvent2 = nullptr;

public:
	static	CLevel_Qasmoke*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, _uint iLevelID);
	virtual	void					Free() override;
};
END
#endif // !__CLIENT_LEVEL_QASMOKE_H__