#pragma once

BEGIN(Engine)
class CLevel;
END

class CLevel_Editor;

class CWindow
{
public:
	explicit CWindow() = default;
	virtual ~CWindow() = default;

public:
	HRESULT Initialize(std::weak_ptr<CLevel> _wpLevel);
	_int PreUpdate(const _float& _fTimeDelta);
	_int Update(const _float& _fTimeDelta);
	_int LateUpdate(const _float& _fTimeDelta);
	void EndPlay();
	void Render();
	void Release();

protected:
	// 레벨 에디터
	std::weak_ptr<CLevel_Editor> m_wpEditorLevel;
};

