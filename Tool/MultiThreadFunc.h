#pragma once

#include "ToolBase.h"
#include "MainFrm.h"
#include "ToolDoc.h"
#include "ToolView.h"
#include "Form.h"
#include <io.h>

class CMultiThreadFunc final : public CBase
{
	DECLARE_SINGLETON(CMultiThreadFunc)
public:
	CMultiThreadFunc();
	virtual ~CMultiThreadFunc() DEFAULT;


public:
	HRESULT					Init(CToolView* pToolView, CForm* pForm, _pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	CRITICAL_SECTION		Get_CS() { return m_CS; }

	void					Load_MaterialFiles();
	HRESULT					LoadingProcess();
	HRESULT					LoadFunc(_uint iStart, _uint iEnd);
	void					Call_LoadButton();
	_bool					Get_Load() { return m_bLoad; }

private:
	HANDLE					m_hThread = 0;

	CRITICAL_SECTION		m_CS;
	TOOLLOADFILE*			m_pLoadFile = nullptr;
	vector<OBJ_INFO*>		m_vecSceneObject;
	CToolView*				m_pToolView;
	CForm*					m_pForm;
	_bool					m_bLoad = false;
	vector<_tchar*>			m_vecDynamicTag;
	_pGraphicDevice*		m_pGraphicDevice = nullptr;
	_pContextDevice*		m_pContextDevice = nullptr;



public:
	virtual void			Free() override;
};

