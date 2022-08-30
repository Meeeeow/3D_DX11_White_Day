#pragma once
#ifndef __CLIENT_PUZZLE_FIRST_H__
#define __CLIENT_PUZZLE_FIRST_H__

#include "ModelObject.h"
BEGIN(Client)
class CPuzzleFirst final : public CModelObject
{
public:
	CPuzzleFirst(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CPuzzleFirst() DEFAULT;

public:
	virtual HRESULT					NativeConstruct(OBJ_INFO tObjInfo);
	virtual _int					Tick(_double dTimeDelta);
	virtual _int					LateTick(_double dTimeDelta);
	virtual HRESULT					Render() override;
	
public:
	virtual void					Interaction() override;
	virtual void					Event() override;
	virtual void					Collect_Event() override;
	void							Set_Locked() { m_bLocked = true; }

	void							Correct_Position(_vector vPosition);

private:
	_bool							m_bLocked = false;
	_float4							m_vInitialPos;

public:
	static	CPuzzleFirst*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, OBJ_INFO tObjInfo);
	virtual void					Free() override;

};
END
#endif // !__CLIENT_PUZZLE_FIRST_H__