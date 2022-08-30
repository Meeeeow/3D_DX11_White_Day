#pragma once

#include "ToolBase.h"

class CToolModel final : public CToolBase
{
public:
	CToolModel(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevie);
	virtual ~CToolModel() DEFAULT;

public:
	virtual HRESULT			NativeConstruct(const char* pModelFilePath,
		const char*		pModelFileName, const _tchar* pShaderFilePath, const char* pTechniqueName, _fmatrix matPivot);
	virtual HRESULT			NativeConstruct(OBJ_INFO tObjInfo, _fmatrix matPivot);
	virtual HRESULT			NativeConstructForTool(OBJ_INFO tObjInfo, _fmatrix matPivot);
	virtual _int			Tick(_double dTimeDelta);
	virtual _int			LateTick(_double dTimeDelta);
	virtual HRESULT			Render();

public:
	virtual HRESULT			Set_ConstantTable();
	void					Set_Animation(_uint iIndex);

public:
	_uint					Get_AnimationCount();
	const char*				Get_AnimationName(_uint iIndex);
	_bool					Get_HasAnimation() { return m_pModel->Get_HasAnimation(); }
	virtual _fvector		Get_OnePoint();
	virtual void			Picking();
	_bool					Picking(_float3* pOut, _float& fDistance);
	virtual void Compute_Distance() override;

private:
	CModel*					m_pModel = nullptr;
	
public:
	static	CToolModel*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice
		, const char* pModelFilePath, const char* pModelFileName, const _tchar* pShaderFilePath, const char* pTechniqueName, _fmatrix matPivot = XMMatrixIdentity());
	static	CToolModel*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice
		, OBJ_INFO tObjInfo, _fmatrix matPivot = XMMatrixIdentity());
	static	CToolModel*		Create_ForLoad(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice
		, OBJ_INFO tObjInfo, _fmatrix matPivot = XMMatrixIdentity());
	virtual	void			Free() override;
};

