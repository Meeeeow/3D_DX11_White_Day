#pragma once
#ifndef __ENGINE_VI_BUFFER_H__
#define __ENGINE_VI_BUFFER_H__

#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer abstract : public CComponent
{
public:
	typedef struct tagVIBufferPassDesc {
		ID3DX11EffectPass*		pEffectPass = nullptr;
		ID3D11InputLayout*		pInputLayOut = nullptr;
	}PASSDESC;
	
public:
	typedef struct tagVIBufferDesc {
		_uint					iNumVerticesX = 0;
		_uint					iNumVerticesY = 0;
		_uint					iNumVerticesZ = 0;
		
		_float					fIntervalX = 0;
		_float					fIntervalY = 0;
		_float					fIntervalZ = 0;

		_float					fTexU = 0;
		_float					fTexV = 0;
		_float					fTexW = 0;

		_float3					vStartPos = _float3(0.f, 0.f, 0.f);

		wchar_t					pShaderFilePath[MAX_PATH] = L"";
		char					pTechniqueName[MAX_PATH] = "";
	}DESC;

protected:
	CVIBuffer(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() DEFAULT;

public:
	virtual	HRESULT				NativeConstruct() override;
	virtual HRESULT				Clone_Construct(void* pArg) override;

public:
	HRESULT						Set_RawValue(const char* pConstantName, void* pData, _uint iByteLength);
	HRESULT						Set_Texture(const char* pConstantName, ID3D11ShaderResourceView* pShaderResourceView);
public:
	HRESULT						Render(_uint iIndex = 0);

protected:
	HRESULT						Create_VertexBuffer();
	HRESULT						Create_IndexBuffer();
	HRESULT						Compile_Shader(D3D11_INPUT_ELEMENT_DESC* pElement,_uint iNumElement ,const _tchar* pShaderFilePath, const char* pTechniqueName);

public:
	virtual _bool				Picking(_float3* pOut, _matrix matWorld);

protected:
	_uint						m_iStride = 0;			//	정점의 사이즈
	_uint						m_iNumVertices = 0;		//	정점의 개수
	_uint						m_iNumPrimitive = 0;	//	그려야 하는 도형의 개수
	_uint						m_iNumIndicesPerPT = 0;	//	도형을 하나 그릴때 필요한 인덱스의 개수
	_uint						m_iFaceIndexSize = 0;	//	인덱스의 사이즈
	DXGI_FORMAT					m_eIndexFormat;			//	인덱스의 DXGI_FORMAT
	D3D11_PRIMITIVE_TOPOLOGY	m_eRenderType;			//	렌더 시 어떤 도형을 그릴건지

	ID3D11Buffer*				m_pVB = nullptr;
	D3D11_BUFFER_DESC			m_tVB_Desc;
	D3D11_SUBRESOURCE_DATA		m_tVB_SubResourceData;

	ID3D11Buffer*				m_pIB = nullptr;
	D3D11_BUFFER_DESC			m_tIB_Desc;
	D3D11_SUBRESOURCE_DATA		m_tIB_SubResourceData;

	void*						m_pVertices = nullptr;
	void*						m_pIndices = nullptr;

	ID3DX11Effect*				m_pEffect = nullptr;
	vector<PASSDESC*>			m_vecPassDesc;

public:
	virtual	CComponent*			Clone(void* pArg) PURE;
	virtual void				Free() override;

};
END
#endif // !__ENGINE_VI_BUFFER_H__
