#pragma once
#ifndef __ENGINE_FRUSTUM_MANAGER_H__
#define __ENGINE_FRUSTUM_MANAGER_H__

#include "Transform.h"

BEGIN(Engine)
class CFrustum final : public CBase
{
	DECLARE_SINGLETON(CFrustum)

private:
	CFrustum();
	virtual ~CFrustum() DEFAULT;

public:
	HRESULT				Make_Frustum(float fFar);

public:
	_bool				Culling_Radius(CTransform* pTransform, _float fRadius);
	_bool				Culling_PointRadius(_fvector vPos, _matrix matWorld,_float fRadius);
private:
	_float4				m_vPlane[6];

public:
	virtual void		Free() override;
};

END
#endif // !__ENGINE_FRUSTUM_MANAGER_H__