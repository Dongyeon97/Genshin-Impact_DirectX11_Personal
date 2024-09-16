#pragma once

BEGIN(Engine)

class ENGINE_DLL CCustomFont final
{
public:
	explicit CCustomFont() = default;
	virtual ~CCustomFont() = default;

/********************
	Framework
********************/
public:
	HRESULT Initialize(const wstring& _wstrFontFilePath);
	HRESULT Render(const wstring& wstrText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin, _float fScale);
	void Release();

	static shared_ptr<CCustomFont> Create(const wstring& strFontFilePath);
/********************
	Methods
********************/
public:


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
	SpriteBatch* m_pBatch = { nullptr };
	SpriteFont* m_pFont = { nullptr };
};

END