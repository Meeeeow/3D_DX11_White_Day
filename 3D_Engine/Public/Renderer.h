#pragma once
#ifndef __ENGINE_RENDERER_H__
#define __ENGINE_RENDERER_H__

#include "Component.h"
#include "RenderTargetManager.h"
#include "LightManager.h"
#include "VIBuffer_Viewport.h"

BEGIN(Engine)
class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_TEST, RENDER_NONLIGHT, RENDER_BLEND
		, RENDER_UI_FIRST, RENDER_UI_SECOND, RENDER_UI_THIRD, RENDER_END };
	typedef list<class CGameObject*>	RENDEROBJECTS;
	enum DEBUGGROUP { DEBUG_COLLIDER, DEBUG_NAVI, DEBUG_END };
	typedef list<class CComponent*>		DEBUGOBJECTS;
private:
	CRenderer(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CRenderer() DEFAULT;

public:
	virtual HRESULT			NativeConstruct() override;
	virtual HRESULT			NativeConstruct(const _tchar* pShaderFilePath, const char* pTechniqueName);
	virtual HRESULT			Clone_Construct(void* pArg) override;

public:
	HRESULT					Add_RenderGroup(RENDERGROUP eRenderSequence, class CGameObject* pRenderObject);
	HRESULT					Add_DebugGroup(DEBUGGROUP eDebugSequence, class CComponent* pDebugObjects);
	HRESULT					Draw_RenderGroup();

private:
	HRESULT					Render(RENDERGROUP eRenderSequence);
	HRESULT					Sequence_Priority();
	HRESULT					Sequence_Test();
	HRESULT					Sequence_Blend();
	HRESULT					Sequence_UI_First();
	HRESULT					Sequence_UI_Second();
	HRESULT					Sequence_UI_Third();

private:
	HRESULT					Sequence_Light();
	HRESULT					Sequence_Result();

private:
	RENDEROBJECTS			m_listRenderObjects[RENDER_END];
	DEBUGOBJECTS			m_listDebugObjects[DEBUG_END];

	CRenderTargetManager*	m_pRenderTargetManager = nullptr;
	CLightManager*			m_pLightManager = nullptr;
	CVIBuffer_Viewport*		m_pVIBuffer = nullptr;

public:
	static	CRenderer*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	static	CRenderer*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, const _tchar* pShaderFilePath, const char* pTechniqueName);
	virtual CComponent*		Clone(void* pArg) override;
	virtual void			Free() override;
};

END
#endif // !__ENGINE_RENDERER_H__
