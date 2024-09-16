#pragma once

BEGIN(Engine)

class CCustomFont;

class ENGINE_DLL CFontManager final
{
	DECLARE_SINGLETON(CFontManager)

/********************
	Framework
********************/
public:
	HRESULT Initialize();
	HRESULT Render(const wstring& wstrFontTag, const wstring& wstrText, const _float2& vPosition, _fvector vColor = Colors::White, _float fRotation = 0.f, const _float2& vOrigin = _float2(0.f, 0.f), _float fScale = 1.f);
	void Release();


/********************
	Methods
********************/
public:
	HRESULT AddFont(const wstring& wstrFontTag, const wstring& wstrFontFilePath);
	shared_ptr<CCustomFont> FindFont(const wstring& wstrFontTag);

/********************
	Getter / Setter
********************/
public:


/********************
	Data Members
********************/
private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	map<wstring, shared_ptr<CCustomFont>> m_mapFonts;
};

END