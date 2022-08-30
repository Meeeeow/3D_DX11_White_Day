#pragma once
#ifndef __ENGINE_HIERARCHYNODE_H__
#define __ENGINE_HIERARCHYNODE_H__

#include "Base.h"
#include "Channel.h"

BEGIN(Engine)
class CHierarchyNode final : public CBase
{
public:
	typedef vector<CChannel*>		CHANNELS;

private:
	explicit CHierarchyNode();
	virtual ~CHierarchyNode() DEFAULT;

public:
	HRESULT					NativeConstruct(const char* pName, _fmatrix fmatTransformation, CHierarchyNode* pParentNode
		, _uint iDepth);
	void					Update_CombinedTransformMatrix(_uint iCurredAnimIndex);
	void					Update_CombinedTransformMatrix();

	HRESULT					Reserve_Channel(_uint iNumAnimations) {
		m_vecAnimationChannels.resize(iNumAnimations); return S_OK;
	}
	void					Add_Channel(_uint iAnimIndex, CChannel* pChannel) {
		m_vecAnimationChannels[iAnimIndex] = pChannel; Safe_AddRef(pChannel);
	}

public:
	_uint					Get_Depth() const {
		return m_iDepth;
	}
	const char*				Get_Name() const {
		return m_szName;
	}
	_matrix					Get_OffsetMatrix() const {
		return XMLoadFloat4x4(&m_matOffset);
	}
	_matrix					Get_CombinedTransformationMatrix() const {
		return XMLoadFloat4x4(&m_matCombinedTransformation);
	}

public:
	void					Set_OffSetMatrix(_fmatrix fmatOffSet) {
		XMStoreFloat4x4(&m_matOffset, fmatOffSet);
	}

public:
	void					Add_AnimationChannel(_uint iAnimIndex, CChannel* pChannel) {
		m_vecAnimationChannels[iAnimIndex] = pChannel; Safe_AddRef(pChannel);
	}

private:
	CHierarchyNode*			m_pParentNode = nullptr;
	char					m_szName[MAX_PATH] = "";
	_float4x4				m_matOffset;
	_float4x4				m_matTransformation;
	_float4x4				m_matCombinedTransformation;
	_uint					m_iDepth = 0;

private:
	CHANNELS				m_vecAnimationChannels;

public:
	static CHierarchyNode*	Create(const char* pName, _fmatrix fmatTransformation, CHierarchyNode* pParentNode
		, _uint iDepth);
	virtual	void			Free() override;
};
END
#endif // !__ENGINE_HIERARCHYNODE_H__
