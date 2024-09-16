#include "Engine_pch.h"
#include "FontManager.h"

#include "DeviceManager.h"

#include "CustomFont.h"

IMPLEMENT_SINGLETON(CFontManager)

HRESULT CFontManager::Initialize()
{
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	return S_OK;
}

HRESULT CFontManager::Render(const wstring& wstrFontTag, const wstring& wstrText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin, _float fScale)
{
	shared_ptr<CCustomFont> spFont = FindFont(wstrFontTag);
	if (nullptr == spFont)
		return E_FAIL;

	return spFont->Render(wstrText, vPosition, vColor, fRotation, vOrigin, fScale);
}

HRESULT CFontManager::AddFont(const wstring& wstrFontTag, const wstring& wstrFontFilePath)
{
	if (nullptr != FindFont(wstrFontTag))
		return E_FAIL;

	shared_ptr<CCustomFont> spFont = CCustomFont::Create(wstrFontFilePath);
	if (nullptr == spFont)
		return E_FAIL;

	m_mapFonts.emplace(wstrFontTag, spFont);

	return S_OK;
}

shared_ptr<CCustomFont> CFontManager::FindFont(const wstring& wstrFontTag)
{
	auto iter = m_mapFonts.find(wstrFontTag);

	if (iter == m_mapFonts.end())
		return nullptr;

	return iter->second;
}

void CFontManager::Release()
{
	for (auto& pair : m_mapFonts)
	{
		pair.second->Release();
		pair.second.reset();
	}
	m_mapFonts.clear();

	m_upInstance.reset();
}