#pragma once

BEGIN(Engine)

class ENGINE_DLL CInputManager final
{
	DECLARE_SINGLETON(CInputManager)

/********************
	Framework
********************/
public:
	HRESULT Initialize(HINSTANCE _hInst, HWND _hWnd);
	void Update();
	void Release();

/********************
	Methods
********************/
public:
	bool IsKeyDown(_int _iKey) { return GetKeyState(_iKey) == EKeyState::DOWN; }
	bool IsKeyPressed(_int _iKey) { return GetKeyState(_iKey) == EKeyState::PRESSED; }
	bool IsKeyUp(_int _iKey) { return GetKeyState(_iKey) == EKeyState::UP; }
	bool IsKeyReleased(_int _iKey) { return GetKeyState(_iKey) == EKeyState::RELEASED; }

	bool IsMouseDown(EMouseButtonType _eMouseButtonType) { return GetMouseState(_eMouseButtonType) == EMouseState::DOWN; }
	bool IsMousePressed(EMouseButtonType _eMouseButtonType) { return GetMouseState(_eMouseButtonType) == EMouseState::PRESSED; }
	bool IsMouseUp(EMouseButtonType _eMouseButtonType) { return GetMouseState(_eMouseButtonType) == EMouseState::UP; }
	bool IsMouseReleased(EMouseButtonType _eMouseButtonType) { return GetMouseState(_eMouseButtonType) == EMouseState::RELEASED; }

	_int GetMouseDeltaX() { return m_arrMouseDelta[(_int)EAxisType::X_AXIS]; }
	_int GetMouseDeltaY() { return m_arrMouseDelta[(_int)EAxisType::Y_AXIS]; }
	_int GetMouseDeltaZ() { return m_arrMouseDelta[(_int)EAxisType::Z_AXIS]; }
	tuple<_int, _int, _int> GetMouseDeltaAll(EAxisType _eAxisType) { return { m_arrMouseDelta[(_int)EAxisType::X_AXIS], m_arrMouseDelta[(_int)EAxisType::Y_AXIS],m_arrMouseDelta[(_int)EAxisType::Z_AXIS] }; }

	void SetCursorInCenter(bool _bCursorInCenter) { m_bMouseHold = _bCursorInCenter; }
	bool GetCursorInCenter() { return m_bMouseHold; }

private:
	EKeyState GetKeyState(_ubyte _iKey) { return m_arrKeyStates[_iKey]; }
	EMouseState GetMouseState(EMouseButtonType _eMouseButtonType) { return m_arrMouseStates[(_int)_eMouseButtonType]; }

/********************
	Data Members
\********************/
public:
	void SetMouseHold(bool _bMouseHold) { m_bMouseHold = _bMouseHold; }
	bool GetMouseHold() { return m_bMouseHold; }

/********************
	Data Members
********************/
private:
	HINSTANCE m_hInst;
	HWND m_hWnd;

private:
	LPDIRECTINPUT8 m_pDeviceInput = nullptr;
	LPDIRECTINPUTDEVICE8 m_pDIKeyDevice = nullptr;
	LPDIRECTINPUTDEVICE8 m_pDIMouseDevice = nullptr;


private:
	array<EKeyState, 256> m_arrKeyStates{ EKeyState::RELEASED };
	array<EMouseState, 3> m_arrMouseStates{ EMouseState::RELEASED };
	array<_int, 3> m_arrMouseDelta{ 0 };

private:
	bool m_bMouseHold = false;
};

END