#include "System/CustomFont.h"

CCustomFont::CCustomFont(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pContext(tDevice.pDeviceContext)
{
}

HRESULT CCustomFont::Initialize(const wstring& strFontFilePath)
{
	m_pBatch = new SpriteBatch(m_pContext.Get());
	m_pFont = new SpriteFont(m_pDevice.Get(), strFontFilePath.c_str());

	return S_OK;
}

HRESULT CCustomFont::Render(const wstring& strText, const _float2& vPosition, _fvector vColor, _float fScale, _float2 vOrigin, _float fRotation)
{
	m_pBatch->Begin();

	m_pFont->DrawString(m_pBatch, strText.c_str(), vPosition, vColor, fRotation, vOrigin, fScale);

	m_pBatch->End();

	return S_OK;
}

CCustomFont* CCustomFont::Create(const DX11DEVICE_T tDevice, const wstring& strFontFilePath)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize(strFontFilePath)))
	{
		Safe_Release(pInstance);
		MSG_BOX("폰트 문제야");
	}

	return pInstance;
}

void CCustomFont::Free()
{
	Safe_Delete(m_pBatch);
	Safe_Delete(m_pFont);
}
