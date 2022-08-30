#pragma once
#ifndef __CLIENT_STRUCT_H__
#define __CLIENT_STRUCT_H__

#include "GameInstance.h"

BEGIN(Client)
typedef struct tagNavigationMeshInfo {
	_float3 vPoints[3];
}NAV_INFO;

typedef struct tagObjectInfo {
	tagObjectInfo() {
		ZeroMemory(this, sizeof(tagObjectInfo));
		XMStoreFloat4x4(&matWorld, XMMatrixIdentity());
		fCullRadius = 1.f;
	}
	enum OBJBUFFERTYPE { VIB_MODEL, VIB_CUBE, VIB_RECT, VIB_TERRAIN, VIB_NAV, VIB_UI
		, VINS_POINT, VINS_MODEL, VIB_END};
	enum COLLIDERTYPE { COLLIDER_AABB, COLLIDER_OBB, COLLIDER_SPHERE, COLLIDER_END };

	/* Default Info */
	OBJBUFFERTYPE			eBufferType;
	
	/* Cull Info */
	float					fCullRadius = 1.f;

	/* Identify Object */
	wchar_t					szObjName[MAX_PATH];

	/* When use Model Buffer Path */
	char					szFilePath[MAX_PATH];
	/* When use Model Buffer Name (.fbx) */
	char					szFileName[MAX_PATH];

	/* When use except Model Buffer */
	wchar_t					szTextureName[MAX_PATH]; 

	/* Necessary. Shader Path, Technique Name */
	wchar_t					szShaderFilePath[MAX_PATH];
	char					szTehcniqueName[MAX_PATH];
	unsigned int			iNumPass;

	/* If Using Instance Buffer */
	unsigned int			iInstanceNum;

	/* Collider Info */
	COLLIDERTYPE			eColliderType;
	CCollider::DESC			tColliderDesc;

	/* Transform Info */
	XMFLOAT4X4				matWorld;
	XMVECTOR				vRotation;
	XMVECTOR				vScale;

	/* VIBuffer Info */
	unsigned int			iNumVerticesX, iNumVerticesY, iNumVerticesZ;
	float					fIntervalX, fIntervalY, fIntervalZ;
	float					fTexU, fTexV, fTexW;
	XMFLOAT3				vStartPos;

	/* Navigation Info */
	XMFLOAT3				vNaviPoints[3];

	// Function
	XMVECTOR		Get_State(CTransform::STATE eState) 
	{
		return XMLoadFloat4((XMFLOAT4*)&matWorld.m[eState][0]); 
	}
	FXMMATRIX		Get_WorldXM() const 
	{
		return XMLoadFloat4x4(&matWorld); 
	}
	XMFLOAT4X4		Get_World() const 
	{
		return matWorld; 
	}
	XMFLOAT4X4		Get_WorldTranspose() const 
	{
		XMFLOAT4X4		fMatrix;
		XMStoreFloat4x4(&fMatrix, XMMatrixTranspose(XMLoadFloat4x4(&matWorld)));
		return fMatrix; 
	}
	void			Set_State(CTransform::STATE eState, FXMVECTOR vState) 
	{
		XMFLOAT4	vStateDesc;
		XMStoreFloat4(&vStateDesc, vState);
		memcpy((XMFLOAT4*)&matWorld.m[eState][0], &vStateDesc, sizeof(XMFLOAT4)); 
	}

	wchar_t*		Get_TextureName() 
	{
		return szTextureName;
	}

	CVIBuffer::DESC	Get_VIBufferInfo(CVIBuffer::DESC tVIBufferDesc) 
	{
		tVIBufferDesc.iNumVerticesX = iNumVerticesX; tVIBufferDesc.iNumVerticesY = iNumVerticesY; tVIBufferDesc.iNumVerticesZ = iNumVerticesZ;
		tVIBufferDesc.fTexU = fTexU; tVIBufferDesc.fTexV = fTexV; tVIBufferDesc.fTexW = fTexW;
		tVIBufferDesc.fIntervalX = fIntervalX; tVIBufferDesc.fIntervalY = fIntervalY; tVIBufferDesc.fIntervalZ = fIntervalZ;
		tVIBufferDesc.vStartPos = vStartPos;
		wcscpy_s(tVIBufferDesc.pShaderFilePath, szShaderFilePath); strcpy_s(tVIBufferDesc.pTechniqueName, szTehcniqueName);
		return tVIBufferDesc;
	}
	NAV_INFO		Get_NavInfo(NAV_INFO tNavInfo)
	{
		tNavInfo.vPoints[0] = vNaviPoints[0];
		tNavInfo.vPoints[1] = vNaviPoints[1];
		tNavInfo.vPoints[2] = vNaviPoints[2];
		return tNavInfo;
	}
}OBJ_INFO;


typedef struct tagPickingEventObject {
	CGameObject*	pGameObject = nullptr;
	_float			fDistance = 0.f;
}PEO;

typedef struct tagMeshFilePathInfo {
	wstring	wstrFilePath = TEXT("");
	wstring	wstrFileName = TEXT("");
}MESHPATHINFO;

typedef struct tagLoadFiles {
	wstring				wstrSceneName;
	LEVEL				eLevel;
	size_t				Start;
	size_t				End;
}LOADFILE;

typedef struct tagLoadFileForTool {
	size_t				Start;
	size_t				End;
}TOOLLOADFILE;




END


#endif // !__CLIENT_STRUCT_H__
