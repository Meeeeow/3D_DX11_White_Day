#pragma once
#ifndef __ENGINE_PICKING_H__
#define __ENGINE_PICKING_H__

#include "Base.h"

BEGIN(Engine)
class CPicking final : public CBase
{
	DECLARE_SINGLETON(CPicking)
private:
	CPicking();
	virtual ~CPicking() DEFAULT;

public:
	HRESULT				NativeConstruct(_pContextDevice* pContextDevice, HWND hWnd);

public:
	_fvector			Get_Ray() { return XMLoadFloat4(&m_vMouseRay); }
	_fvector			Get_Pivot() { return XMLoadFloat4(&m_vMousePivot); }

public:
	HRESULT				PreConvert();
	_bool				Picking(_fvector v0, _fvector v1, _fvector v2, _float& fDistance, _fmatrix matWorld);
	

private:
	_pContextDevice*	m_pContextDevice = nullptr;
	HWND				m_hWnd;

	_float4				m_vMouseRay;
	_float4				m_vMousePivot;

public:
	virtual	void		Free() override;
};
END
#endif // !__ENGINE_PICKING_H__