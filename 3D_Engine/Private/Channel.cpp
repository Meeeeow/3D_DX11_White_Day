#include "..\Public\Channel.h"



CChannel::CChannel()
{
}

HRESULT CChannel::NativeConstruct(const char* pName)
{
	strcpy_s(m_szName, pName);

	return S_OK;
}

HRESULT CChannel::Add_KeyFrame(KEYFRAME * pKeyFrame)
{
	if (pKeyFrame == nullptr)
		return E_FAIL;

	m_vecKeyFrames.push_back(pKeyFrame);

	return S_OK;
}

CChannel * CChannel::Create(const char* pName)
{
	CChannel* pInstance = new CChannel();
	if (FAILED(pInstance->NativeConstruct(pName)))
		Safe_Release(pInstance);
	return pInstance;
}

void CChannel::Free()
{
	for (auto& iter : m_vecKeyFrames)
		Safe_Delete(iter);

	m_vecKeyFrames.clear();
}

