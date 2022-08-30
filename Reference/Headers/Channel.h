#pragma once
#ifndef __ENGINE_CHANNEL_H__
#define __ENGINE_CHANNEL_H__

#include "Base.h"
BEGIN(Engine)
class CChannel final : public CBase
{
public:
	typedef	vector<KEYFRAME*>		KEYFRAMES;

private:
	explicit CChannel();
	virtual ~CChannel() DEFAULT;

public:
	HRESULT				NativeConstruct(const char* pName);
	HRESULT				Add_KeyFrame(KEYFRAME* pKeyFrame);

public:
	_uint				Get_NumKeyFrames() {
		return (_uint)m_vecKeyFrames.size();
	}
	_uint				Get_CurrentKeyFrameIndex() {
		return m_iCurrentKeyFrame;
	}
	KEYFRAMES			Get_KeyFrames() {
		return m_vecKeyFrames;
	}
	_matrix				Get_TransformationMatrix() {
		return XMLoadFloat4x4(&m_matTransformation);
	}

public:
	void				Set_CurrentKeyFrameIndex(_uint iKeyFrameIndex) {
		m_iCurrentKeyFrame = iKeyFrameIndex;
	}
	void				Set_TransformationMatrix(_fmatrix matTransform) {
		XMStoreFloat4x4(&m_matTransformation, matTransform);
	}

private:
	char				m_szName[MAX_PATH] = "";
	KEYFRAMES			m_vecKeyFrames;

private:
	_float4x4			m_matTransformation;
	_uint				m_iCurrentKeyFrame = 0;

public:
	static CChannel*	Create(const char* pName);
	virtual	void		Free() override;
};
END
#endif // !__ENGINE_CHANNEL_H__
