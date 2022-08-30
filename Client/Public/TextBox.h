#pragma once
#ifndef __CLIENT_TEXT_BOX_H__
#define __CLIENT_TEXT_BOX_H__

#include "RectXYObject.h"
BEGIN(Client)
class CTextBox final : public CRectXYObject
{
private:
	CTextBox(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CTextBox() DEFAULT;

public:
	enum TEXTTYPE { JM_TEXT, DOOR_TEXT, DARK_TEXT, CONTROL_TEXT, ENTER_TEXT ,END_TEXT };

public:
	virtual HRESULT			NativeConstruct(TEXTTYPE eType);
	virtual _int			Tick(_double dTimeDelta);
	virtual _int			LateTick(_double dTimeDelta);
	virtual HRESULT			Render() override;

private:
	_float					m_fAlpha = 1.0f;
	TEXTTYPE				m_eType;
	_float4x4				m_matProj;

public:
	static	CTextBox*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, TEXTTYPE eType);
	virtual void			Free() override;

};
END
#endif // !__CLIENT_TEXT_BOX_H__