#pragma once

#include "ToolBase.h"
class CToolTerrain final : public CToolBase
{
private:
	CToolTerrain(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CToolTerrain() DEFAULT;

public:
	virtual HRESULT				NativeConstruct(OBJ_INFO tObjInfo);
	virtual _int				Tick(_double dTimeDelta) override;
	virtual _int				LateTick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;

public:
	HRESULT						Change_VIBuffer();
	HRESULT						Change_Texture(const _tchar* pTextureFilePath);
	

public:
	virtual HRESULT				Set_ConstantTable() override;
	HRESULT						Set_Texture(CTexture* pTexture);
	virtual void				Picking();
	_bool						Picking(_float3* pOut, _float& fDistance);

public: /* Inline */
	_fvector					Get_State(CTransform::STATE eState) {
		return m_pTransform->Get_State(eState);	}
	_fmatrix					Get_WorldXM() const {
		return m_pTransform->Get_WorldXM();	}
	_float4x4					Get_World() const {
		return m_pTransform->Get_World();	}
	_float4x4					Get_WorldTranspose() const {
		return m_pTransform->Get_WorldTranspose();	}

public: /* Inline */

private:
	CVIBuffer_Terrain*			m_pVIBuffer = nullptr;
	CTexture*					m_pTexture = nullptr;


public:
	static	CToolTerrain*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, OBJ_INFO tObjInfo);
	virtual void				Free() override;

};

