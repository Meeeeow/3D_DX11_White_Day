#include "..\Public\Animation.h"



CAnimation::CAnimation()
{
}

HRESULT CAnimation::NativeConstruct(_double dDuration, _double dPlayerSpeedPerSec)
{
	m_dDuration = dDuration;
	m_dPlaySpeedPerSec = dPlayerSpeedPerSec;

	return S_OK;
}

HRESULT CAnimation::Add_Channel(CChannel * pChannel)
{
	if (pChannel == nullptr)
		return E_FAIL;

	m_vecChannels.push_back(pChannel);

	return S_OK;
}
/*	�� �����Ӹ��� ȣ���ϸ�, ���� �ִϸ��̼��� ��� �ð��� �����Ѵ� (����) 
	���� �ִϸ��̼� ��� �ð��� ���� Ű������ ������ ����� �����Ѵ� (��������) */
void CAnimation::Update_TransformationMatrix(_double dTimeDelta)
{
	m_dTimeAcc += m_dPlaySpeedPerSec * dTimeDelta;

	if (m_dTimeAcc >= m_dDuration)
	{
		m_bIsFinished = true;
		m_dTimeAcc = 0.0;
	}
	else
		m_bIsFinished = false;

	_uint		iNumChannel = (_uint)m_vecChannels.size();
	for (_uint i = 0; i < iNumChannel; ++i)
	{
		CChannel*			pChannel = m_vecChannels[i];
		if (pChannel == nullptr)
			return;

		_uint				iNumKeyFrames = pChannel->Get_NumKeyFrames();
		_uint				iCurrentKeyFrameIndex = pChannel->Get_CurrentKeyFrameIndex();

		if (m_bIsFinished)
		{
			iCurrentKeyFrameIndex = 0;
			pChannel->Set_CurrentKeyFrameIndex(iCurrentKeyFrameIndex);
		}
		vector<KEYFRAME*>	vecKeyFrames = pChannel->Get_KeyFrames();

		_vector				vScale, vRotation, vPosition;
		vScale = XMVectorSet(1.f, 1.f, 1.f, 0.f);
		vRotation = XMVectorZero();
		vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);

		if (m_dTimeAcc <= vecKeyFrames[0]->dTime)
		{ /* ó�� ���� */
			vScale = XMLoadFloat4(&vecKeyFrames[0]->vScale);
			vRotation = XMLoadFloat4(&vecKeyFrames[0]->vRotation);
			vPosition = XMLoadFloat4(&vecKeyFrames[0]->vPosition);
		}
		else if (m_dTimeAcc >= vecKeyFrames[iNumKeyFrames - 1]->dTime)
		{ /* ������ */
			vScale = XMLoadFloat4(&vecKeyFrames[0]->vScale);
			vRotation = XMLoadFloat4(&vecKeyFrames[0]->vRotation);
			vPosition = XMLoadFloat4(&vecKeyFrames[0]->vPosition);
		}
		else
		{ /* ������ */
			while (m_dTimeAcc >= vecKeyFrames[iCurrentKeyFrameIndex + 1]->dTime)
				pChannel->Set_CurrentKeyFrameIndex(++iCurrentKeyFrameIndex);

			_double dRatio = (m_dTimeAcc - vecKeyFrames[iCurrentKeyFrameIndex]->dTime) /
				(vecKeyFrames[iCurrentKeyFrameIndex + 1]->dTime - vecKeyFrames[iCurrentKeyFrameIndex]->dTime);

			vScale = XMVectorLerp(XMLoadFloat4(&vecKeyFrames[iCurrentKeyFrameIndex]->vScale)
				, XMLoadFloat4(&vecKeyFrames[iCurrentKeyFrameIndex + 1]->vScale)
				, (_float)dRatio);
			vPosition = XMVectorLerp(XMLoadFloat4(&vecKeyFrames[iCurrentKeyFrameIndex]->vPosition)
				, XMLoadFloat4(&vecKeyFrames[iCurrentKeyFrameIndex + 1]->vPosition)
				, (_float)dRatio);
			vRotation = XMQuaternionSlerp(XMLoadFloat4(&vecKeyFrames[iCurrentKeyFrameIndex]->vRotation)
				, XMLoadFloat4(&vecKeyFrames[iCurrentKeyFrameIndex + 1]->vRotation)
				, (_float)dRatio);
		}

		_matrix		matTransformation = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

		pChannel->Set_TransformationMatrix(matTransformation);
	}


}

_matrix CAnimation::Get_CurrentAnimMatrix()
{
	return m_vecChannels[0]->Get_TransformationMatrix();
}

CAnimation * CAnimation::Create(_double dDuration, _double dPlayerSpeedPerSec)
{
	CAnimation* pInstance = new CAnimation();
	if (FAILED(pInstance->NativeConstruct(dDuration, dPlayerSpeedPerSec)))
		Safe_Release(pInstance);
	return pInstance;
}

void CAnimation::Free()
{
	for (auto& iter : m_vecChannels)
		Safe_Release(iter);
	m_vecChannels.clear();
}