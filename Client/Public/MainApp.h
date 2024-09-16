#pragma once

BEGIN(Engine)

class CLevel;

END

class CMainApp final
{
public:
	explicit CMainApp() = default;
	~CMainApp() = default;

public:
	HRESULT PreInitialize(HINSTANCE _hInst, HWND _hWnd);
	HRESULT Initialize();
	_int PreUpdate(const _float& _fTimeDelta);
	_int Update(const _float& _fTimeDelta);
	_int LateUpdate(const _float& _fTimeDelta);
	void EndPlay();
	void Render();
	void Release();

private:
	HRESULT OpenLevel(LEVEL _eStartLevel);
};
