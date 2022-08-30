#include "stdafx.h"
#include "..\Public\PositionManager.h"

IMPLEMENT_SINGLETON(CPositionManager)

CPositionManager::CPositionManager()
{

}

void CPositionManager::Add_Time(_double dTime)
{
	if(m_bLeeBongGooDead == false)
		m_pLeeBongGoo->Add_ManagerTime(dTime);
}

void CPositionManager::LeeBongGooDead()
{
	m_bLeeBongGooDead = true;
	m_pLeeBongGoo = nullptr;
	m_pLeeBongGooCollider = nullptr;
	m_pLeeBongGooTransform = nullptr;
}

void CPositionManager::SonDalSuDead()
{
	m_bSonDalSuDead = true;
	m_pSonDalSu = nullptr;
	m_pSonDalSuCollider = nullptr;
	m_pSonDalSuTransform = nullptr;
}

void CPositionManager::Set_Camera(CCamera_Debug * pCamera)
{
	m_pCamera = pCamera;
}

void CPositionManager::Set_Player(CLeeHeeMin * pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayerTransform = dynamic_cast<CTransform*>(m_pPlayer->Get_Component(ComTransform));
	m_pPlayerCollider = dynamic_cast<CCollider*>(m_pPlayer->Get_Component(ComCollider));
}

void CPositionManager::Set_LeeBongGoo(CLeeBongGoo * pLeeBongGoo)
{
	m_pLeeBongGoo = pLeeBongGoo;
	m_pLeeBongGooTransform = dynamic_cast<CTransform*>(m_pLeeBongGoo->Get_Component(ComTransform));
	m_pLeeBongGooCollider = dynamic_cast<CCollider*>(m_pLeeBongGoo->Get_Component(ComCollider));
}

void CPositionManager::Set_SonDalSu(CSonDalSu * pSonDalSu)
{
	m_pSonDalSu = pSonDalSu;
	m_pSonDalSuTransform = dynamic_cast<CTransform*>(m_pSonDalSu->Get_Component(ComTransform));
	m_pSonDalSuCollider = dynamic_cast<CCollider*>(m_pSonDalSu->Get_Component(ComCollider));
}

void CPositionManager::Set_FuseBox(CFuseBody * pFuseBody)
{
	m_pFuseBody = pFuseBody;
}

void CPositionManager::Set_Valve0(CValve * pValve0)
{
	m_pValve0 = pValve0;
}

void CPositionManager::Set_Valve1(CValve * pValve1)
{
	m_pValve1 = pValve1;
}

void CPositionManager::Set_YooJiMin(CYooJiMin * pYooJiMin)
{
	m_pYooJiMin = pYooJiMin;
}

void CPositionManager::Free()
{

}
