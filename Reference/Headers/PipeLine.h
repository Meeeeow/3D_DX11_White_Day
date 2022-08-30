#pragma once
#ifndef __ENGINE_PIPE_LINE_H__
#define __ENGINE_PIPE_LINE_H__



#include "Base.h"
BEGIN(Engine)
class CPipeLine final : public CBase
{
	DECLARE_SINGLETON(CPipeLine)
public:
	enum PIPESTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };

private:
	CPipeLine();
	virtual ~CPipeLine() DEFAULT;

public:
	HRESULT				Update_Variables();

	void				Set_Transform(PIPESTATE eState, _fmatrix matTransform)	{
		eState == D3DTS_VIEW ? XMStoreFloat4x4(&m_matView, matTransform) : XMStoreFloat4x4(&m_matProj, matTransform);
	}
	void				Set_Far(_float fFar) { m_fFar = fFar; }

public:
	_vector				Get_CamPos() {
		return XMLoadFloat3(&m_vCamPos);	}

	_matrix				Get_Transform(PIPESTATE eState) {
		return eState == D3DTS_VIEW ? XMLoadFloat4x4(&m_matView) : XMLoadFloat4x4(&m_matProj);	}

	_matrix				Get_TransposeTransform(PIPESTATE eState) {
		return XMMatrixTranspose(Get_Transform(eState));	}

	_matrix				Get_InverseTransform(PIPESTATE eState) {
		return eState == D3DTS_VIEW ? XMLoadFloat4x4(&m_matInverseView) : XMLoadFloat4x4(&m_matInverseProj);	}
	_float				Get_Far() { return m_fFar; }


private:
	_float4x4			m_matView, m_matInverseView;
	_float4x4			m_matProj, m_matInverseProj;
	_float3				m_vCamPos;
	_float				m_fFar;

public:
	virtual void		Free() override;
};
END
#endif // !__ENGINE_PIPE_LINE_H__