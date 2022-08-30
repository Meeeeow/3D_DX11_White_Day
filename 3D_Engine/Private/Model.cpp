#include "..\Public\Model.h"
#include "Texture.h"
#include "Picking.h"
#include "PipeLine.h"
#include "GameInstance.h"
#include "ComponentManager.h"

CModel::CModel(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CVIBuffer(pGraphicDevice, pContextDevice)
{
}

CModel::CModel(const CModel & rhs)
	: CVIBuffer(rhs), m_pScene(rhs.m_pScene), m_vecMeshMaterials(rhs.m_vecMeshMaterials)
	, m_matPivot(rhs.m_matPivot), m_bHasAnimation(rhs.m_bHasAnimation), m_iNumAnimations(rhs.m_iNumAnimations)
	, m_vecAnimationNames(rhs.m_vecAnimationNames), m_bForClient(rhs.m_bForClient)
{
	/* �� 4���� ���� ������ ����� �����ϰ� ������ �����̳ʴ� ���� ���縦 �����Ѵ�. */
	/* +) HasAnimation�� �Ǵ��ϴ� bool ������ ���� ���� */

	for (auto& pPrototypeMeshContainer : rhs.m_vecMeshContainers)
	{ /* MeshContainer�� StartIndex�� ���� ������ �ʴ� ��������, ���� ��ü�� �������� �� �ٸ� �������� ���ϱ� ���� ���� ���縦 �����Ѵ�. */
		CMeshContainer* pMeshContainer = pPrototypeMeshContainer->Clone();
		m_vecMeshContainers.emplace_back(pMeshContainer);
	}
	m_vecMeshContainers.shrink_to_fit();



	for (auto& pMeshMaterials : m_vecMeshMaterials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(pMeshMaterials->pMaterialTextures[i]);
	}
}

HRESULT CModel::NativeConstruct(const char * pModelFilePath, const char * pModelFileName, const _tchar * pShaderFilePath, const char * pTechniqueName, _fmatrix matPivot)
{
	char		szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	m_pScene = m_cImporter.ReadFile(szFullPath, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	if (m_pScene == nullptr)
		FAILMSG("Model NativeConstruct - Importer's ReadFile");

	XMStoreFloat4x4(&m_matPivot, matPivot);

	m_bHasAnimation = m_pScene->HasAnimations();


	if (FAILED(Create_MeshContainer()))
		return E_FAIL;

	if (FAILED(Compile_Shader(pShaderFilePath, pTechniqueName)))
		return E_FAIL;

	if (FAILED(Create_Materials(pModelFilePath)))
		return E_FAIL;

	if (m_bHasAnimation == false)
	{
		if (FAILED(Create_VIBuffer()))
			return E_FAIL;

		return S_OK;
	}
	

	if (FAILED(Create_SkinnedMesh()))
		return E_FAIL;

	if (FAILED(Create_VIBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Clone_Construct(void * pArg)
{
	m_bForClient = false;

	if (!m_bHasAnimation)
		return S_OK;

	if (FAILED(Create_HierarchyNodes(m_pScene->mRootNode, nullptr, 0)))
		return E_FAIL;

	sort(m_vecHierarchyNodes.begin(), m_vecHierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest) {
		return pSour->Get_Depth() < pDest->Get_Depth();
	});

	if (FAILED(Create_HierarchyNodeForMeshContainer()))
		return E_FAIL;

	for (auto& pHierarchyNode : m_vecHierarchyNodes)
		pHierarchyNode->Update_CombinedTransformMatrix();

	if (FAILED(Create_Animation()))
		return E_FAIL;


	return S_OK;
}

HRESULT CModel::Construct_ForClient(const char * pModelFilePath, const char * pModelFileName, const _tchar * pShaderFilePath, const char * pTechniqueName, _fmatrix matPivot)
{
	m_bCloned = false;
	m_bForClient = true;

	char		szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	m_pScene = m_cImporter.ReadFile(szFullPath, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	if (m_pScene == nullptr)
		FAILMSG("Modle NativeConstruct - Importer's ReadFile");

	XMStoreFloat4x4(&m_matPivot, matPivot);

	m_bHasAnimation = m_pScene->HasAnimations();

	if (FAILED(Create_MeshContainer()))
		return E_FAIL;

	if (FAILED(Compile_Shader(pShaderFilePath, pTechniqueName)))
		return E_FAIL;

	if (FAILED(Clone_Materials(pModelFilePath)))
		return E_FAIL;

	if (m_bHasAnimation == false)
	{
		if (FAILED(Create_VIBuffer()))
			return E_FAIL;

		return S_OK;
	}


	if (FAILED(Create_SkinnedMesh()))
		return E_FAIL;

	if (FAILED(Create_VIBuffer()))
		return E_FAIL;

	if (FAILED(Create_HierarchyNodes(m_pScene->mRootNode, nullptr, 0)))
		return E_FAIL;

	sort(m_vecHierarchyNodes.begin(), m_vecHierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest) {
		return pSour->Get_Depth() < pDest->Get_Depth();
	});

	if (FAILED(Create_HierarchyNodeForMeshContainer()))
		return E_FAIL;

	for (auto& pHierarchyNode : m_vecHierarchyNodes)
		pHierarchyNode->Update_CombinedTransformMatrix();

	if (FAILED(Create_Animation()))
		return E_FAIL;
	m_pScene->mTextures[0]->mFilename;

	return S_OK;
}

HRESULT CModel::Construct_ForTool(const char * pModelFilePath, const char * pModelFileName, const _tchar * pShaderFilePath, const char * pTechniqueName, _fmatrix matPivot)
{
	char		szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	m_pScene = m_cImporter.ReadFile(szFullPath, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	if (m_pScene == nullptr)
		FAILMSG("Modle NativeConstruct - Importer's ReadFile");

	XMStoreFloat4x4(&m_matPivot, matPivot);

	m_bHasAnimation = m_pScene->HasAnimations();

	if (FAILED(Create_MeshContainer()))
		return E_FAIL;

	if (FAILED(Compile_Shader(pShaderFilePath, pTechniqueName)))
		return E_FAIL;

	if (FAILED(Create_Materials(pModelFilePath)))
		return E_FAIL;

	if (m_bHasAnimation == false)
	{
		if (FAILED(Create_VIBuffer()))
			return E_FAIL;

		return S_OK;
	}


	if (FAILED(Create_SkinnedMesh()))
		return E_FAIL;

	if (FAILED(Create_VIBuffer()))
		return E_FAIL;

	if (FAILED(Create_HierarchyNodes(m_pScene->mRootNode, nullptr, 0)))
		return E_FAIL;

	sort(m_vecHierarchyNodes.begin(), m_vecHierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest) {
		return pSour->Get_Depth() < pDest->Get_Depth();
	});

	if (FAILED(Create_HierarchyNodeForMeshContainer()))
		return E_FAIL;

	for (auto& pHierarchyNode : m_vecHierarchyNodes)
		pHierarchyNode->Update_CombinedTransformMatrix();

	if (FAILED(Create_Animation()))
		return E_FAIL;
	m_pScene->mTextures[0]->mFilename;

	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex, _uint iPassIndex)
{
	if (iPassIndex >= m_vecPassDesc.size())
		FAILMSG("iPassIndex is too big - Model Rneder");

	m_pContextDevice->IASetInputLayout(m_vecPassDesc[iPassIndex]->pInputLayOut);
	/*sort(m_vecMeshContainers.begin(), m_vecMeshContainers.end(), [](CMeshContainer* pSour, CMeshContainer* pDest) {
		return pSour->Get_BoneCount() > pDest->Get_BoneCount();
	});*/

	if (m_bHasAnimation)
	{
		_matrix			matBones[128];
		ZeroMemory(matBones, sizeof(_matrix) * 128);

		m_vecMeshContainers[iMeshIndex]->Set_CombinedMatrices(matBones, XMLoadFloat4x4(&m_matPivot));
		if (FAILED(Set_RawValue("g_tBoneMatrices", matBones, sizeof(_matrix) * 128)))
			return E_FAIL;
	}
	else if (!m_bHasAnimation)
	{
		_matrix			matBones[128];
		ZeroMemory(matBones, sizeof(_matrix) * 128);

		matBones[0] = XMMatrixIdentity();
		if (FAILED(Set_RawValue("g_tBoneMatrices", matBones, sizeof(_matrix) * 128)))
			return E_FAIL;
	}

	m_vecPassDesc[iPassIndex]->pEffectPass->Apply(0, m_pContextDevice);

	m_vecMeshContainers[iMeshIndex]->Render();

	return S_OK;
}

_matrix CModel::Get_BoneMatrix(const _tchar * pNodeTag)
{
	auto	iter = find_if(m_mapRefNodes.begin(), m_mapRefNodes.end(), CTagFinder(pNodeTag));
	if (iter == m_mapRefNodes.end())
		return XMMatrixIdentity();

	_matrix	matOffset = iter->second->Get_OffsetMatrix();
	_matrix	matCombined = iter->second->Get_CombinedTransformationMatrix();

	return matOffset * matCombined * XMLoadFloat4x4(&m_matPivot);
}

HRESULT CModel::Bind_Buffer()
{
	_uint	iOffSet = 0;

	m_pContextDevice->IASetVertexBuffers(0, 1, &m_pVB, &m_iStride, &iOffSet);
	m_pContextDevice->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContextDevice->IASetPrimitiveTopology(m_eRenderType);

	return S_OK;
}

HRESULT CModel::Compile_Shader(const _tchar * pShaderFilePath, const char * pTechinqueName)
{
	D3D11_INPUT_ELEMENT_DESC		tElementDesc[] = {
		{ "POSITION"   , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"     , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT      , 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT"    , 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDEX" , 0, DXGI_FORMAT_R32G32B32A32_UINT , 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(__super::Compile_Shader(tElementDesc, 6, pShaderFilePath, pTechinqueName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Bind_MatrixTexture(_uint iMeshContainderIndex, aiTextureType eTextureType, char * pConstantName)
{
	_uint		iMaterialIndex = m_vecMeshContainers[iMeshContainderIndex]->Get_MaterialIndex();

	CTexture*	pTexture = m_vecMeshMaterials[iMaterialIndex]->pMaterialTextures[eTextureType];
	if (pTexture == nullptr)
		return E_FAIL;
	
	return Set_Texture(pConstantName, pTexture->Get_SRV());
}

HRESULT CModel::Set_Animation(_uint iAnimIndex)
{
	if (m_vecAnimations.size() <= iAnimIndex)
		return E_FAIL;

	m_iCurrentAnimationIndex = iAnimIndex;

	return S_OK;
}

HRESULT CModel::Set_MaterialsIndex(_uint iNumMeshIndex, aiTextureType eType, char * pContantName)
{
	_uint iMaterialIndex = m_vecMeshContainers[iNumMeshIndex]->Get_MaterialIndex();

	CTexture* pTexture = m_vecMeshMaterials[iMaterialIndex]->pMaterialTextures[eType];
	if (pTexture == nullptr)
		return E_FAIL;

	return Set_Texture(pContantName, pTexture->Get_SRV());
}


void CModel::Update_CombinedTransformationMatrix(_double dTimeDelta)
{
	if (!m_bHasAnimation)
		return;

	/* ���� Transformation Matrix�� ���� ��Ų��. */
	m_vecAnimations[m_iCurrentAnimationIndex]->Update_TransformationMatrix(dTimeDelta);

	/* ���� ��� �������(iDepth) ����� ������Ų��. */
	for (auto& pHierarchyNode : m_vecHierarchyNodes)
		pHierarchyNode->Update_CombinedTransformMatrix(m_iCurrentAnimationIndex);
}

HRESULT CModel::Debug_Animation_P()
{
	if (++m_iCurrentAnimationIndex >= m_vecAnimations.size())
		m_iCurrentAnimationIndex = 0;

	return S_OK;
}

HRESULT CModel::Debug_Animation_M()
{
	if (--m_iCurrentAnimationIndex < 0)
		m_iCurrentAnimationIndex = (_uint)m_vecAnimations.size();

	return S_OK;
}

_bool CModel::Picking(_float3 * pOut, _matrix matWorld, _float & fDist)
{
	CPicking* pPicking = CPicking::Get_Instance();

	_uint iIndex = 0;
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		
		_float3	iIndices[] = 
		{
			((VTXMESH*)m_pVertices)[i].vPosition,
			((VTXMESH*)m_pVertices)[++i].vPosition,
			((VTXMESH*)m_pVertices)[++i].vPosition
		};

		for(_uint j= 0; j<3; ++j)
			XMStoreFloat3(&iIndices[j], XMVector3TransformCoord(XMLoadFloat3(&iIndices[j]), matWorld));

		if (pPicking->Picking(XMLoadFloat3(&iIndices[0]), XMLoadFloat3(&iIndices[1]), XMLoadFloat3(&iIndices[2]), fDist, matWorld))
		{
			_vector vPivot = pPicking->Get_Pivot();
			_vector vRay = pPicking->Get_Ray();
			XMStoreFloat3(pOut, vPivot + vRay * fDist);
			_float fX, fY, fZ, fW;
			fX = floor((XMVectorGetX(XMLoadFloat3(pOut)) * 100)) / 100;
			fY = floor((XMVectorGetY(XMLoadFloat3(pOut)) * 100)) / 100;
			fZ = floor((XMVectorGetZ(XMLoadFloat3(pOut)) * 100)) / 100;
			fW = floor((XMVectorGetW(XMLoadFloat3(pOut)) * 100)) / 100;
			XMStoreFloat3(pOut, XMVectorSet(fX, fY, fZ, fW));

			return true;
		}

	}
	return false;
}

HRESULT CModel::Create_HierarchyNodes(aiNode * pNode, CHierarchyNode * pParentNode, _uint iDepth)
{
	_matrix		matTransformation;
	memcpy(&matTransformation, &pNode->mTransformation, sizeof(_float4x4));

	/* 
	iDepth�� ���� �����̴�. iDepth�� �ʱ⿡ �θ�(0)�� ���� �ڽ��� ���������� ������ ��, �ʺ� Ž���� �ƴ� ���� Ž���� �ǹǷ�,
	���� Ž���� ���� ����(iDepth)�� �ο��ϰ�, Sorting �ϱ� ���ؼ� �����Ѵ�.
	*/
	CHierarchyNode* pHierarchyNode = CHierarchyNode::Create(pNode->mName.data, matTransformation, pParentNode, iDepth++);

	pHierarchyNode->Reserve_Channel(m_pScene->mNumAnimations);

	/* Nullptr üũ�� ���� �ʴ� ������, Create���� �ʱ�ȭ �������� return E_FAIL�� ���� ����. */
	m_vecHierarchyNodes.emplace_back(pHierarchyNode);


	for (_uint i = 0; i < pNode->mNumChildren; ++i)
	{
		if (FAILED(Create_HierarchyNodes(pNode->mChildren[i], pHierarchyNode, iDepth)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CModel::Create_SkinnedMesh()
{
	/* �޽��� ������ŭ ��ȸ�Ѵ�. */
	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		aiMesh*			pMesh = m_pScene->mMeshes[i];
		/* �޽� ������ �޽� �����̳� ������ �����ϱ� ������ ���� �޽���ȣ�� �޽������̳ʸ� �����;��Ѵ�. */
		CMeshContainer*	pMeshContainer = m_vecMeshContainers[i];
		pMeshContainer->Set_Bones(pMesh->mNumBones);
		/* �� �޽��� �� ������ŭ ��ȸ�Ѵ�. */
		for (_uint j = 0; j < pMesh->mNumBones; ++j)
		{
			/* ���� �޽��� �� ��ȣ��° ���� �޾ƿ´�. */
			aiBone*			pBone = pMesh->mBones[j];

			/* �ش� ��(j)�� ������ �ִ� ������ ������ŭ ��ȸ�Ѵ�. ��, �� ������ �ٸ� �������� �ߺ��� �� �ִ�. 
			�׷���, BlendWeight�� �ִ� 4������ ���� �����ϸ�, ����(x,y,z,w)��� �����Ѵ�. */
			for (_uint k = 0; k < pBone->mNumWeights; ++k)
			{
				/* ���� �޽�(i)�� �޽� �����̳��� ���� ������ ���� ��(j)�� ������ ��ġ��(k) ���� Vertex ID�� ���Ѵ�. */
				_uint	iVertexIndex = pMeshContainer->Get_StartVertexIndex() + pBone->mWeights[k].mVertexId;

				/* ������� �����Ѵ�. */
				if (((VTXMESH*)m_pVertices)[iVertexIndex].vBlendWeight.x == 0.0f)
				{
					/* ��(j)�� �ε����� �־��ش�. */
					((VTXMESH*)m_pVertices)[iVertexIndex].vBlendIndex.x = j;
					/* ���� ������ ��ġ��(k) ������ ������ �־��ش�. */
					((VTXMESH*)m_pVertices)[iVertexIndex].vBlendWeight.x = pBone->mWeights[k].mWeight;
				}
				else if (((VTXMESH*)m_pVertices)[iVertexIndex].vBlendWeight.y == 0.0f)
				{
					((VTXMESH*)m_pVertices)[iVertexIndex].vBlendIndex.y = j;
					((VTXMESH*)m_pVertices)[iVertexIndex].vBlendWeight.y = pBone->mWeights[k].mWeight;
				}
				else if (((VTXMESH*)m_pVertices)[iVertexIndex].vBlendWeight.z == 0.0f)
				{
					((VTXMESH*)m_pVertices)[iVertexIndex].vBlendIndex.z = j;
					((VTXMESH*)m_pVertices)[iVertexIndex].vBlendWeight.z = pBone->mWeights[k].mWeight;
				}
				else if (((VTXMESH*)m_pVertices)[iVertexIndex].vBlendWeight.w == 0.0f)
				{
					((VTXMESH*)m_pVertices)[iVertexIndex].vBlendIndex.w = j;
					((VTXMESH*)m_pVertices)[iVertexIndex].vBlendWeight.w = pBone->mWeights[k].mWeight;
				}
			}
		}
	}
	return S_OK;
}

HRESULT CModel::Create_HierarchyNodeForMeshContainer()
{
	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		aiMesh*			pMesh = m_pScene->mMeshes[i];
		CMeshContainer*	pMeshContainer = m_vecMeshContainers[i];

		if (pMeshContainer->Get_BonesCount() == 0)
		{
			CHierarchyNode*	pHierarchyNode = Find_HierarchyNode(pMesh->mName.data);
			if (pHierarchyNode == nullptr)
				FAILMSG("WARNING #1");
			pHierarchyNode->Set_OffSetMatrix(XMMatrixIdentity());
			pMeshContainer->Add_HierarchyNode(pHierarchyNode);
			continue;
		}

		for (_uint j = 0; j < pMesh->mNumBones; ++j)
		{
			aiBone*			pBone = pMesh->mBones[j];
			CHierarchyNode*	pHierarchyNode = Find_HierarchyNode(pBone->mName.data);
			
				/* �̰� nullptr�� ���Ѱ� */
			if (pHierarchyNode == nullptr)
				FAILMSG("Model Create HierarchyNode For MeshContainer - HierarchyNode is nullptr");

			_matrix			matOffSet;
			memcpy(&matOffSet, &pBone->mOffsetMatrix, sizeof(_float4x4));

			/* ���� OffSet��� HierarchyNode�� ��ġ�ؼ� �����ϱ� */
			pHierarchyNode->Set_OffSetMatrix(XMMatrixTranspose(matOffSet));
			/* ���� �޽� ��ȣ�� �޽� �����̳ʿ� ���� ��带 �־��ش�. */
			pMeshContainer->Add_HierarchyNode(pHierarchyNode);
		}

	}
	
	return S_OK;
}

HRESULT CModel::Create_Animation()
{
	m_vecAnimations.reserve(m_pScene->mNumAnimations);
	m_vecAnimationNames.reserve(m_pScene->mNumAnimations);
	for (_uint i = 0; i < m_pScene->mNumAnimations; ++i)
	{
		aiAnimation*	pAIAnimation = m_pScene->mAnimations[i];
		if (pAIAnimation == nullptr)
			FAILMSG("Model Create Animation - aiAnimation is nullptr");

		m_vecAnimationNames.push_back(pAIAnimation->mName.C_Str());

		CAnimation*		pAnimation = CAnimation::Create(pAIAnimation->mDuration,pAIAnimation->mTicksPerSecond);
		if (pAnimation == nullptr)
			FAILMSG("Model Create Animation - Animation Class is nullptr");

		for (_uint j = 0; j < pAIAnimation->mNumChannels; ++j)
		{
			aiNodeAnim*		pAINodeAnim = pAIAnimation->mChannels[j];

			CChannel*		pChannel = CChannel::Create(pAINodeAnim->mNodeName.data);
			CHierarchyNode*	pHierarchyNode = Find_HierarchyNode(pAINodeAnim->mNodeName.data);
			if (pHierarchyNode == nullptr)
				FAILMSG("Model Create Animation - Find HierarchyNode");

			/* �ϳ��� �ִϸ��̼��� ǥ���ϱ� ���� ���� ����. */
			_uint	iNumKeyFramesMax = max(pAIAnimation->mChannels[j]->mNumPositionKeys, pAIAnimation->mChannels[j]->mNumRotationKeys);
			iNumKeyFramesMax = max(iNumKeyFramesMax, pAIAnimation->mChannels[j]->mNumScalingKeys);

			_vector	vScale, vRotation, vPosition;
			vScale = XMVectorSet(1.f, 1.f, 1.f, 0.f);
			vRotation = XMVectorZero();
			vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);

			for (_uint k = 0; k < iNumKeyFramesMax; ++k)
			{
				KEYFRAME*		pKeyFrame = new KEYFRAME;

				if (pAIAnimation->mChannels[j]->mNumScalingKeys > k)
				{
					memcpy(&vScale, &pAINodeAnim->mScalingKeys[k].mValue,sizeof(_float3));

					pKeyFrame->dTime = pAINodeAnim->mScalingKeys[k].mTime;
				}
				if (pAIAnimation->mChannels[j]->mNumRotationKeys > k)
				{
					/* Assimp�� Rotation�� {W, X, Y, Z}�� ���ǵǾ� �ִ�. */
					vRotation = XMVectorSetX(vRotation, pAINodeAnim->mRotationKeys[k].mValue.x);
					vRotation = XMVectorSetY(vRotation, pAINodeAnim->mRotationKeys[k].mValue.y);
					vRotation = XMVectorSetZ(vRotation, pAINodeAnim->mRotationKeys[k].mValue.z);
					vRotation = XMVectorSetW(vRotation, pAINodeAnim->mRotationKeys[k].mValue.w);

					pKeyFrame->dTime = pAINodeAnim->mRotationKeys[k].mTime;
				}
				if (pAIAnimation->mChannels[j]->mNumPositionKeys > k)
				{
					memcpy(&vPosition, &pAINodeAnim->mPositionKeys[k].mValue, sizeof(_float3));
					vPosition = XMVectorSetW(vPosition, 1.f);

					pKeyFrame->dTime = pAINodeAnim->mPositionKeys[k].mTime;
				}
				XMStoreFloat4(&pKeyFrame->vScale, vScale);
				XMStoreFloat4(&pKeyFrame->vRotation, vRotation);
				XMStoreFloat4(&pKeyFrame->vPosition, vPosition);

				pChannel->Add_KeyFrame(pKeyFrame);
			}

			pAnimation->Add_Channel(pChannel);
			pHierarchyNode->Add_AnimationChannel(i, pChannel);
		}
		m_vecAnimations.push_back(pAnimation);
	}
	m_iNumAnimations = m_pScene->mNumAnimations;
	return S_OK;
}

HRESULT CModel::Create_MeshContainer()
{
	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		/* ��� �޽��� ������ �ε����� �����Ͽ� �ִ� ����, �ε��� ���� �����Ѵ�. */
		m_iNumVertices += m_pScene->mMeshes[i]->mNumVertices;
		m_iNumPrimitive += m_pScene->mMeshes[i]->mNumFaces;
	}
	/* �޽��� ��� �������� �ϳ��� ���ۿ� ��� ���� �����س��� ������ �ִ�� ���� �迭�� �����Ѵ�. */
	m_pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXMESH) * m_iNumVertices);

	/* �޽��� ��� �ε����� �ϳ��� ���ۿ� ��� ���� �����س��� �ε����� �ִ�� �ε��� �迭�� �����Ѵ�. */
	m_pIndices = new TRI32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(TRI32) * m_iNumPrimitive);

	/* iIndex :
	�޽��� �ִ� �������� ������ ��, �� �޽��� ���� ���� �ε����� ������ ���� ���� */
	_uint	iIndex = 0;
	/* iFaceIndex :
	�޽��� �ִ� �ε��� ���� ������ ��, �� �޽��� ���� �ε���(Polygon, Face)�� ������ ���� ���� */
	_uint	iFaceIndex = 0;


	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		aiMesh*		pMesh = m_pScene->mMeshes[i];
		if (pMesh == nullptr)
			FAILMSG("Model Create MeshContainer - pMesh is nullptr");

		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pGraphicDevice, m_pContextDevice
			, iIndex, pMesh->mNumVertices, iFaceIndex, pMesh->mNumFaces, pMesh->mMaterialIndex);

		for (_uint j = 0; j < pMesh->mNumVertices; ++j)
		{
			aiVector3D* pNullCheck = pMesh->mNormals;
			if (pNullCheck == nullptr)
				continue;

			memcpy(&((VTXMESH*)m_pVertices)[iIndex].vPosition, &pMesh->mVertices[j], sizeof(_float3));
			if(!m_bHasAnimation)
				XMStoreFloat3(&((VTXMESH*)m_pVertices)[iIndex].vPosition
					, XMVector3TransformCoord(XMLoadFloat3(&((VTXMESH*)m_pVertices)[iIndex].vPosition), XMLoadFloat4x4(&m_matPivot)));

			memcpy(&((VTXMESH*)m_pVertices)[iIndex].vNormal, &pMesh->mNormals[j], sizeof(_float3));
			if(!m_bHasAnimation)
				XMStoreFloat3(&((VTXMESH*)m_pVertices)[iIndex].vNormal
					, XMVector3TransformNormal(XMLoadFloat3(&((VTXMESH*)m_pVertices)[iIndex].vNormal), XMLoadFloat4x4(&m_matPivot)));

			memcpy(&((VTXMESH*)m_pVertices)[iIndex].vTexUV, &pMesh->mTextureCoords[0][j], sizeof(_float2));
			memcpy(&((VTXMESH*)m_pVertices)[iIndex].vTangent, &pMesh->mTangents[j], sizeof(_float3));
			++iIndex;
		}

		for (_uint k = 0; k < pMesh->mNumFaces; ++k)
		{
			((TRI32*)m_pIndices)[iFaceIndex]._1 = pMesh->mFaces[k].mIndices[0];
			((TRI32*)m_pIndices)[iFaceIndex]._2 = pMesh->mFaces[k].mIndices[1];
			((TRI32*)m_pIndices)[iFaceIndex]._3 = pMesh->mFaces[k].mIndices[2];
			++iFaceIndex;
		}

		m_vecMeshContainers.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::Create_VIBuffer()
{
	m_iNumIndicesPerPT = 3;
	m_iStride = sizeof(VTXMESH);
	m_iFaceIndexSize = sizeof(TRI32);
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eRenderType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_tVB_Desc, sizeof(D3D11_BUFFER_DESC));

	m_tVB_Desc.ByteWidth = m_iStride * m_iNumVertices;
	m_tVB_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	m_tVB_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tVB_Desc.CPUAccessFlags = 0;
	m_tVB_Desc.MiscFlags = 0;
	m_tVB_Desc.StructureByteStride = m_iStride;

	ZeroMemory(&m_tIB_Desc, sizeof(D3D11_BUFFER_DESC));

	m_tIB_Desc.ByteWidth = m_iFaceIndexSize * m_iNumPrimitive;
	m_tIB_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	m_tIB_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tIB_Desc.CPUAccessFlags = 0;
	m_tIB_Desc.MiscFlags = 0;

	ZeroMemory(&m_tVB_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tVB_SubResourceData.pSysMem = m_pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	ZeroMemory(&m_tIB_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tIB_SubResourceData.pSysMem = m_pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Create_Materials(const char * pMeshFilePath)
{
	_uint	iNumMaterials = m_pScene->mNumMaterials;

	m_vecMeshMaterials.reserve(iNumMaterials);

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		aiMaterial*		pMaterial = m_pScene->mMaterials[i];
		if (pMaterial == nullptr)
			FAILMSG("Model Bind_MatrixTexture - Material nullptr");
		/* Texture Class �� �迭�� ���� �ִ� �޽� ��Ƽ���� */
		MESHMATERIAL*	pTagMeshMaterial = new MESHMATERIAL;
		ZeroMemory(pTagMeshMaterial, sizeof(MESHMATERIAL));

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			aiString		strTexturePath;
			if (FAILED(pMaterial->GetTexture(aiTextureType(j), 0, &strTexturePath)))
				continue;

			char szFileName[MAX_PATH] = "";
			char szFileExt[MAX_PATH] = "";

			_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szFileExt, MAX_PATH);

			char szFullPath[MAX_PATH] = "";
			strcpy_s(szFullPath, pMeshFilePath);
			strcat_s(szFullPath, szFileName);
			if (!strcmp(szFileExt, ""))
				strcat_s(szFullPath, ".png");
			else
				strcat_s(szFullPath, szFileExt);

			_tchar	szMaterialFullPath[MAX_PATH] = TEXT("");
			MultiByteToWideChar(CP_ACP, 0, szFullPath, (int)strlen(szFullPath), szMaterialFullPath, MAX_PATH);

			CTexture*		pMaterialTexture = CTexture::Create(m_pGraphicDevice, m_pContextDevice, szMaterialFullPath);
			if (pMaterialTexture == nullptr)
				return E_FAIL;

			pTagMeshMaterial->pMaterialTextures[j] = pMaterialTexture;
		}
		m_vecMeshMaterials.push_back(pTagMeshMaterial);
	}

	return S_OK;
}

HRESULT CModel::Clone_Materials(const char * pMeshFilePath)
{
	CComponentManager* pComponentManager = CComponentManager::Get_Instance();


	_uint	iNumMaterials = m_pScene->mNumMaterials;

	m_vecMeshMaterials.reserve(iNumMaterials);

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		aiMaterial*		pMaterial = m_pScene->mMaterials[i];
		if (pMaterial == nullptr)
			FAILMSG("Model Bind_MatrixTexture - Material nullptr");
		/* Texture Class �� �迭�� ���� �ִ� �޽� ��Ƽ���� */
		MESHMATERIAL*	pTagMeshMaterial = new MESHMATERIAL;
		ZeroMemory(pTagMeshMaterial, sizeof(MESHMATERIAL));

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			aiString		strTexturePath;
			if (FAILED(pMaterial->GetTexture(aiTextureType(j), 0, &strTexturePath)))
				continue;

			char szFileName[MAX_PATH] = "";
			char szFileExt[MAX_PATH] = "";

			_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, 256, szFileExt, 256);

			char szFullPath[MAX_PATH] = "";
			strcpy_s(szFullPath, szFileName);
			strcat_s(szFullPath, szFileExt);

			wstring wstr(&szFullPath[0], &szFullPath[260]);

			CTexture*		pMaterialTexture = (CTexture*)pComponentManager->Clone_Component(1, wstr.c_str() ,nullptr);
			if (pMaterialTexture == nullptr)
				return E_FAIL;
			pTagMeshMaterial->pMaterialTextures[j] = pMaterialTexture;
		}
		m_vecMeshMaterials.push_back(pTagMeshMaterial);
	}

	return S_OK;
}

HRESULT CModel::Create_RefNode(const _tchar * pNodeTag, const char * pBoneName)
{
	CHierarchyNode* pNode = Find_HierarchyNode(pBoneName);
	if (pNode == nullptr)
		return E_FAIL;

	m_mapRefNodes.emplace(pNodeTag, pNode);
	Safe_AddRef(pNode);

	return S_OK;
}

CHierarchyNode * CModel::Find_HierarchyNode(const char * pBoneName)
{
	auto iter = find_if(m_vecHierarchyNodes.begin(), m_vecHierarchyNodes.end(), [&](CHierarchyNode* pNode){
		return !strcmp(pNode->Get_Name(), pBoneName);
	});
	if (iter == m_vecHierarchyNodes.end())
		return nullptr;

	return (*iter);
}

_fvector CModel::Get_OnePoint()
{
	return XMLoadFloat3(&((VTXMESH*)m_pVertices)[0].vPosition);
}

_matrix CModel::Get_AnimMatrix()
{
	return m_vecAnimations[m_iCurrentAnimationIndex]->Get_CurrentAnimMatrix();
}



CModel * CModel::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const char * pModelFilePath, const char * pModelFileName, const _tchar * pShaderFilePath, const char * pTechniqueName, _fmatrix matPivot)
{
	CModel* pInstance = new CModel(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(pModelFilePath, pModelFileName, pShaderFilePath, pTechniqueName, matPivot)))
		Safe_Release(pInstance);

	return pInstance;
}

CModel * CModel::Create_ForClient(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const char * pModelFilePath, const char * pModelFileName, const _tchar * pShaderFilePath, const char * pTechniqueName, _fmatrix matPivot)
{
	CModel* pInstance = new CModel(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->Construct_ForClient(pModelFilePath, pModelFileName, pShaderFilePath, pTechniqueName, matPivot)))
		Safe_Release(pInstance);

	return pInstance;
}

CModel * CModel::Create_ForTool(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const char * pModelFilePath, const char * pModelFileName, const _tchar * pShaderFilePath, const char * pTechniqueName, _fmatrix matPivot)
{
	CModel* pInstance = new CModel(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->Construct_ForTool(pModelFilePath, pModelFileName, pShaderFilePath, pTechniqueName, matPivot)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel* pInstance = new CModel(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	if (!m_bForClient)
	{
		for (auto& pPair : m_mapRefNodes)
			Safe_Release(pPair.second);
		m_mapRefNodes.clear();

		for (auto& pMeshHierarchyNodes : m_vecHierarchyNodes)
			Safe_Release(pMeshHierarchyNodes);
		m_vecHierarchyNodes.clear();

		for (auto& pAnimation : m_vecAnimations)
			Safe_Release(pAnimation);
		m_vecAnimations.clear();

		for (auto& pMeshContainer : m_vecMeshContainers)
			Safe_Release(pMeshContainer);
		m_vecMeshContainers.clear();

		for (auto& pMeshMaterial : m_vecMeshMaterials)
		{
			for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
				Safe_Release(pMeshMaterial->pMaterialTextures[i]);

			if (!m_bCloned)
				Safe_Delete(pMeshMaterial);
		}
		m_vecMeshMaterials.clear();
	}
	else if (m_bForClient)
	{
		for (auto& pPair : m_mapRefNodes)
			Safe_Release(pPair.second);
		m_mapRefNodes.clear();

		for (auto& pMeshHierarchyNodes : m_vecHierarchyNodes)
			Safe_Release(pMeshHierarchyNodes);
		m_vecHierarchyNodes.clear();

		for (auto& pAnimation : m_vecAnimations)
			Safe_Release(pAnimation);
		m_vecAnimations.clear();

		for (auto& pMeshContainer : m_vecMeshContainers)
			Safe_Release(pMeshContainer);
		m_vecMeshContainers.clear();

		for (auto& pMeshMaterial : m_vecMeshMaterials)
		{
			for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
				Safe_Release(pMeshMaterial->pMaterialTextures[i]);
			Safe_Delete(pMeshMaterial);
		}
		m_vecMeshMaterials.clear();
		m_vecAnimationNames.clear();
	}
}
