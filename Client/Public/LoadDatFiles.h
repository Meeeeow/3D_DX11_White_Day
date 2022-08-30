#pragma once
#ifndef __CLIENT_LOAD_DAT_FILES_H__
#define __CLIENT_LOAD_DAT_FILES_H__

#include "Base.h"
#include "DefaultModel.h"

#include <io.h>

BEGIN(Client)
class CLoadDatFiles final : public CBase
{
	DECLARE_SINGLETON(CLoadDatFiles)
private:
	explicit CLoadDatFiles();
	virtual ~CLoadDatFiles() DEFAULT;

public:
	HRESULT					Init(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	HRESULT					Load_DatFiles();
	size_t					Get_ObjVectorSize(wstring wstrSceneName);
	const OBJ_INFO*			Get_ObjectInfo(wstring wstrSceneName, _uint iIndex);

public:
	HRESULT					TextureLoadingProcess();
	HRESULT					Load_MaterialFiles();

public:
	HRESULT					LoadingProcess(wstring wstrSceneName, LEVEL eLevel, _pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	HRESULT					LoadFunc(LEVEL eLevel, _uint Start, _uint End, wstring wstrSceneName);
	

private:

public:
	CRITICAL_SECTION		Get_CS() { return m_CS; }
	_pGraphicDevice*		Get_GraphicDevice() { return m_pGraphicDevice; }
	_pContextDevice*		Get_ContextDevice() { return m_pContextDevice; }

public:
	_bool					Get_TextureLoad() { return m_bTextureLoading; }
	_bool					Get_TextureLoadStart() { return m_bTextureLoadStart; }

	_bool					Get_ModelLoading() { return m_bModelLoading; }

public:
	void					Set_ModelLoading() { m_bModelLoading = true; }
	void					Set_NewModelLoading() { m_bModelLoading = false; }

private:
	unordered_map<wstring, vector<OBJ_INFO*>>	m_mapSceneObjects;
	vector<_tchar*>								m_vecDynamicTag;

	HANDLE										m_hThread[4] = {0};
	
	CRITICAL_SECTION							m_CS;

	_pGraphicDevice*							m_pGraphicDevice = nullptr;
	_pContextDevice*							m_pContextDevice = nullptr;
	LOADFILE*									m_pLoadFiles = nullptr;

	_bool										m_bTextureLoading = false;
	_bool										m_bTextureLoadStart = false;
	_bool										m_bModelLoading = false;


public:
	virtual void			Free() override;

public:
	

};
END
#endif // !__CLIENT_LOAD_DAT_FILES_H__