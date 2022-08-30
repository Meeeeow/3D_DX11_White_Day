#pragma once
#ifndef __ENGINE_VIBUFFER_MODEL_H__
#define __ENGINE_VIBUFFER_MODEL_H__

#include "VIBuffer.h"
#include "MeshContainer.h"
#include "HierarchyNode.h"
#include "Animation.h"

BEGIN(Engine)
class ENGINE_DLL CModel final : public CVIBuffer
{
public:
	typedef vector<CMeshContainer*>								MESHCONTAINERS;
	typedef vector<MESHMATERIAL*>								MESHMATERIALS;
	typedef vector<CHierarchyNode*>								HIERARCHYNODES;
	typedef vector<CAnimation*>									ANIMATIONS;
	typedef unordered_map<const _tchar*, CHierarchyNode*>		REFNODES;
	typedef vector<const char*>									ANIMATIONNAMES;

private:
	CModel(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CModel(const CModel& rhs);
	virtual ~CModel() DEFAULT;

public:
	virtual	HRESULT		NativeConstruct(const char* pModelFilePath,
		const char*		pModelFileName, const _tchar* pShaderFilePath, const char* pTechniqueName, _fmatrix matPivot);
	virtual	HRESULT		Clone_Construct(void* pArg);
	HRESULT				Construct_ForClient(const char* pModelFilePath,
		const char*		pModelFileName, const _tchar* pShaderFilePath, const char* pTechniqueName, _fmatrix matPivot);
	HRESULT				Construct_ForTool(const char* pModelFilePath,
		const char*		pModelFileName, const _tchar* pShaderFilePath, const char* pTechniqueName, _fmatrix matPivot);
	virtual HRESULT		Render(_uint iMeshIndex, _uint iPassIndex);

public:
	_uint				Get_MeshContainersSize() const { return (_uint)m_vecMeshContainers.size(); }
	_matrix				Get_BoneMatrix(const _tchar* pNodeTag);

public:
	HRESULT				Bind_Buffer();
	HRESULT				Compile_Shader(const _tchar* pShaderFilePath, const char* pTechinqueName);
	HRESULT				Bind_MatrixTexture(_uint iMeshContainderIndex, aiTextureType eTextureType, char* pConstantName);
	
public:
	HRESULT				Set_Animation(_uint iAnimIndex);
	HRESULT				Set_MaterialsIndex(_uint iNumMeshIndex, aiTextureType eType, char* pContantName);
	void				Update_CombinedTransformationMatrix(_double dTimeDelta);

public:
	HRESULT				Debug_Animation_P();
	HRESULT				Debug_Animation_M();

public:
	_bool				Picking(_float3* pOut, _matrix matWorld, _float& fDist);

public:
	HRESULT				Create_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParentNode, _uint iDepth);
	HRESULT				Create_SkinnedMesh();
	HRESULT				Create_HierarchyNodeForMeshContainer();
	HRESULT				Create_Animation();
	HRESULT				Create_MeshContainer();
	HRESULT				Create_VIBuffer();
	HRESULT				Create_Materials(const char* pMeshFilePath);
	HRESULT				Clone_Materials(const char* pMeshFilePath);
	HRESULT				Create_RefNode(const _tchar* pNodeTag, const char* pBoneName);
	CHierarchyNode*		Find_HierarchyNode(const char* pBoneName);

public:
	_uint				Get_AnimationCount() { return m_iNumAnimations; }
	const char*			Get_AnimationNames(_uint iIndex) { return m_vecAnimationNames[iIndex]; }
	_bool				Get_HasAnimation() { return m_bHasAnimation; }
	_fvector			Get_OnePoint();
	_matrix				Get_AnimMatrix();

private:
	_uint				m_iNumAnimations = 0;
	ANIMATIONNAMES		m_vecAnimationNames;


private:
	const	aiScene*	m_pScene = nullptr;
	Assimp::Importer	m_cImporter;

	/* Be made of Mesh Container Vector Container */
	MESHCONTAINERS		m_vecMeshContainers;
	/* Be made of Mesh Material Struct ( include CTexture array ) Vector Contaier */
	MESHMATERIALS		m_vecMeshMaterials;
	/* Be made of HierarchyNode Vector Container */
	HIERARCHYNODES		m_vecHierarchyNodes;
	/* Be made of Animation Vector Container */
	ANIMATIONS			m_vecAnimations;
	/* Be made of HierarchyNode, Node's Tag Map Container, If Use Attach Other Mesh to Bone */
	REFNODES			m_mapRefNodes;

private:
	_float4x4			m_matPivot;
	_uint				m_iCurrentAnimationIndex = 0;

private:
	_bool				m_bHasAnimation;
	_bool				m_bForClient = false;

public:
	static	CModel*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice
		, const char* pModelFilePath, const char* pModelFileName, const _tchar* pShaderFilePath, const char* pTechniqueName, _fmatrix matPivot = XMMatrixIdentity());
	static	CModel*		Create_ForClient(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice
		, const char* pModelFilePath, const char* pModelFileName, const _tchar* pShaderFilePath, const char* pTechniqueName, _fmatrix matPivot = XMMatrixIdentity());
	static	CModel*		Create_ForTool(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice
		, const char* pModelFilePath, const char* pModelFileName, const _tchar* pShaderFilePath, const char* pTechniqueName, _fmatrix matPivot = XMMatrixIdentity());
	virtual	CComponent*	Clone(void* pArg) override;
	virtual void		Free() override;
};

END
#endif // !__ENGINE_VIBUFFER_MODEL_H__
