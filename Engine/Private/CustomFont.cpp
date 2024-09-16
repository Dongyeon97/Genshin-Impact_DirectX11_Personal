#include "Engine_pch.h"
#include "CustomFont.h"

#include "DeviceManager.h"

HRESULT CCustomFont::Initialize(const wstring& _wstrFontFilePath)
{
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	m_pBatch = new SpriteBatch(m_pContext);
	m_pFont = new SpriteFont(m_pDevice, _wstrFontFilePath.c_str());

	return S_OK;
}

HRESULT CCustomFont::Render(const wstring& wstrText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin, _float fScale)
{
	m_pContext->GSSetShader(nullptr, nullptr, 0);

	m_pBatch->Begin();

	m_pFont->DrawString(m_pBatch, wstrText.c_str(), vPosition, vColor, fRotation, vOrigin, fScale);

	m_pBatch->End();

	return S_OK;
}

void CCustomFont::Release()
{
	Safe_Delete(m_pBatch);
	Safe_Delete(m_pFont);
}

shared_ptr<CCustomFont> CCustomFont::Create(const wstring& strFontFilePath)
{
	shared_ptr<CCustomFont> spInstance = make_shared<CCustomFont>();

	if (FAILED(spInstance->Initialize(strFontFilePath)))
	{
		MSG_BOX("Failed to Created : CCustomFont");
		spInstance->Release();
		spInstance.reset();
	}

	return spInstance;
}
