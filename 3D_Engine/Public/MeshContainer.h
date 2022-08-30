#pragma once
#ifndef __ENGINE_MESH_CONTAINER_H__
#define __ENGINE_MESH_CONTAINER_H__

#include "Base.h"
BEGIN(Engine)
class CMeshContainer final : public CBase
{
public:
	typedef vector<class CHierarchyNode*>		BONES;

private:
	CMeshContainer(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CMeshContainer(const CMeshContainer& rhs);
	virtual ~CMeshContainer() DEFAULT;

public:
	HRESULT				NativeConstruct(_uint iStartVertexIndex
		, _uint iNumVertices, _uint iStartFaceIndex, _uint iNumFaces, _uint iMaterialIndex);
	HRESULT				Render();

public:
	_uint				Get_MaterialIndex() const {
		return m_iMaterialIndex; }
	_uint				Get_StartVertexIndex() const {
		return m_iStartVertexIndex; }
	_uint				Get_BonesCount() const {
		return m_iNumBones;	}
	_uint				Get_BoneCount() {
		return (_uint)m_tBones.size();
	}

public:
	HRESULT				Set_CombinedMatrices(_matrix* pMatrices, _fmatrix matPivot);
	void				Set_Bones(_uint iNumBones) { m_iNumBones = iNumBones; }

public:
	void				Add_HierarchyNode(class CHierarchyNode* pNode) {
		m_tBones.emplace_back(pNode);	Safe_AddRef(pNode);
	}

private:
	_pGraphicDevice*		m_pGraphicDevice = nullptr;
	_pContextDevice*		m_pContextDevice = nullptr;

private:
	_uint					m_iStartVertexIndex = 0;
	_uint					m_iStartFaceIndex = 0;
	_uint					m_iNumVertices = 0;
	_uint					m_iNumFaces = 0;
	_uint					m_iMaterialIndex = 0;
	_uint					m_iNumBones = 0;
	BONES					m_tBones;
public:
	static	CMeshContainer*	Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice,
		_uint iStartVertexIndex, _uint iNumVertices, _uint iStartFaceIndex, _uint iNumFaces, _uint iMaterialIndex);
	virtual CMeshContainer* Clone();
	virtual	void			Free() override;

};
END
#endif // !__ENGINE_MESH_CONTAINER_H__
