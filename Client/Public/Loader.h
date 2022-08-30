#pragma once
#ifndef __CLIENT_LOADER_H__
#define __CLIENT_LOADER_H__

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CLoader final : public CBase
{
private:
	CLoader(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CLoader() DEFAULT;

public:
	CRITICAL_SECTION			Get_CS() { return m_CS; }
	LEVEL						Get_NextLevel() const { return m_eNextLevel; }
	_bool						Get_Finished() const { return m_bIsFinished; }
	const _tchar*				Get_LoadingText() const { return m_szLoading; }

public:
	HRESULT						NativeConstruct(LEVEL eNextLevel);
	HRESULT						Loading_QASmoke();
	HRESULT						Loading_Maze();

public:

private:
	_pGraphicDevice*			m_pGraphicDevice = nullptr;
	_pContextDevice*			m_pContextDevice = nullptr;

	LEVEL						m_eNextLevel = LEVEL_END;
	HANDLE						m_hThread = 0;
	CRITICAL_SECTION			m_CS;

	_tchar						m_szLoading[MAX_PATH] = TEXT("");
	_bool						m_bIsFinished = false;

public:
	static	CLoader*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, LEVEL eNextLevel);
	virtual void				Free() override;

};
END
#endif // !__CLIENT_LOADER_H__