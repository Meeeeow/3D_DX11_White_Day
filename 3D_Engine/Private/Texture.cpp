#include "..\Public\Texture.h"

CTexture::CTexture(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CComponent(pGraphicDevice, pContextDevice)
{
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_vecTextures(rhs.m_vecTextures)
{
	for (auto& pTexture : m_vecTextures)
		Safe_AddRef(pTexture);
}

HRESULT CTexture::NativeConstruct(const _tchar * pTextureFilePath, _uint iNumTextures)
{
	_tchar					szExt[MAX_PATH] = TEXT("");
	_tchar					szFileName[MAX_PATH] = TEXT("");
	_wsplitpath_s(pTextureFilePath, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

	DirectX::ScratchImage	CImage;

	HRESULT					hr;
	_tchar					szFilePath[MAX_PATH] = TEXT("");

	for (_uint i = 0; i < iNumTextures; ++i)
	{
		int iResult = wsprintf(szFilePath, pTextureFilePath, i);

		if (lstrcmp(szExt, TEXT(".dds")) == 0)
			hr = DirectX::LoadFromDDSFile(szFilePath, DDS_FLAGS_NONE, nullptr, CImage);
		else if (lstrcmp(szExt, TEXT(".tga")) == 0)
			hr = DirectX::LoadFromTGAFile(szFilePath, nullptr, CImage);
		else
			hr = DirectX::LoadFromWICFile(szFilePath, WIC_FLAGS_NONE, nullptr, CImage);

		if (FAILED(hr))
		{
			_tchar			szFailMSG[MAX_PATH] = TEXT("");
			wsprintf(szFailMSG, L"%s File Create Failed", szFileName);
			MessageBox(NULL, szFailMSG, L"Texture Create Failed", MB_OK);
			return E_FAIL;
		}

		RESOURCE*		pResource = nullptr;
		if (FAILED(DirectX::CreateTexture(m_pGraphicDevice, CImage.GetImages(), CImage.GetImageCount(), CImage.GetMetadata(), &pResource)))
			FAILMSG("Fail CreateTexture");

		SRV*			pSRV = nullptr;
		if (FAILED(m_pGraphicDevice->CreateShaderResourceView(pResource, nullptr, &pSRV)))
			FAILMSG("Fail CreateShaderResourceView");

		m_vecTextures.emplace_back(pSRV);

		Safe_Release(pResource);
		CImage.Release();
	}

	return S_OK;
}

HRESULT CTexture::Clone_Construct(void * pArg)
{
	return S_OK;
}

CTexture * CTexture::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const _tchar * pTextureFilePath, _uint iNumTexture)
{
	CTexture* pInstance = new CTexture(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(pTextureFilePath, iNumTexture)))
	{
		MSGBOX("CTexture NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTexture::Clone(void * pArg)
{
	CTexture* pInstance = new CTexture(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
	{
		MSGBOX("CTexture Clone_Construct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pTexture : m_vecTextures)
		Safe_Release(pTexture);

	m_vecTextures.clear();
}
