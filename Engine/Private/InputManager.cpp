#include "Engine_pch.h"
#include "InputManager.h"

IMPLEMENT_SINGLETON(CInputManager)

HRESULT CInputManager::Initialize(HINSTANCE _hInst, HWND _hWnd)
{
	m_hInst = _hInst;
	m_hWnd = _hWnd;

	// DInput 컴객체를 생성하는 함수
	if (FAILED(DirectInput8Create(_hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pDeviceInput,
		nullptr)))
		return E_FAIL;

	// 키보드 객체 생성
	if (FAILED(m_pDeviceInput->CreateDevice(GUID_SysKeyboard, &m_pDIKeyDevice, nullptr)))
		return E_FAIL;

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pDIKeyDevice->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pDIKeyDevice->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pDIKeyDevice->Acquire();


	// 마우스 객체 생성
	if (FAILED(m_pDeviceInput->CreateDevice(GUID_SysMouse, &m_pDIMouseDevice, nullptr)))
		return E_FAIL;

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pDIMouseDevice->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pDIMouseDevice->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pDIMouseDevice->Acquire();


	return S_OK;
}

void CInputManager::Update()
{
	// 키보드 입력을 업데이트 합니다.
	char asciiKeys[256];
	assert(SUCCEEDED(m_pDIKeyDevice->GetDeviceState(256, asciiKeys)));

	for (_uint iKey = 0; iKey < 256; iKey++)
	{
		// 키를 누른 경우
		if (asciiKeys[iKey] & 0x80)
		{
			switch (m_arrKeyStates[iKey])
			{
				// 이전에 눌렀거나 누르는 중인 경우
			case EKeyState::DOWN:
			case EKeyState::PRESSED:
				m_arrKeyStates[iKey] = EKeyState::PRESSED;
				break;

				// 이전에 뗐거나 떼는 중인 경우
			case EKeyState::UP:
			case EKeyState::RELEASED:
				m_arrKeyStates[iKey] = EKeyState::DOWN;
				break;
			}
		}

		// 키를 누르지 않은 경우
		else
		{
			switch (m_arrKeyStates[iKey])
			{
				// 이전에 눌렀거나 누르는 중인 경우
			case EKeyState::DOWN:
			case EKeyState::PRESSED:
				m_arrKeyStates[iKey] = EKeyState::UP;
				break;

				// 이전에 뗐거나 떼는 중인 경우
			case EKeyState::UP:
			case EKeyState::RELEASED:
				m_arrKeyStates[iKey] = EKeyState::RELEASED;
				break;
			}
		}
	}

	// 마우스 입력을 업데이트 합니다.
	DIMOUSESTATE m_tMouseState;
	assert(SUCCEEDED(m_pDIMouseDevice->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState)));

	for (_uint iMouse = 0; iMouse < 3; iMouse++)
	{
		// 키를 누른 경우
		if (m_tMouseState.rgbButtons[iMouse])
		{
			switch (m_arrMouseStates[iMouse])
			{
				// 이전에 눌렀거나 누르는 중인 경우
			case EMouseState::DOWN:
			case EMouseState::PRESSED:
				m_arrMouseStates[iMouse] = EMouseState::PRESSED;
				break;

				// 이전에 뗐거나 떼는 중인 경우
			case EMouseState::UP:
			case EMouseState::RELEASED:
				m_arrMouseStates[iMouse] = EMouseState::DOWN;
				break;
			}
		}

		// 키를 누르지 않은 경우
		else
		{
			switch (m_arrMouseStates[iMouse])
			{
				// 이전에 눌렀거나 누르는 중인 경우
			case EMouseState::DOWN:
			case EMouseState::PRESSED:
				m_arrMouseStates[iMouse] = EMouseState::UP;
				break;

				// 이전에 뗐거나 떼는 중인 경우
			case EMouseState::UP:
			case EMouseState::RELEASED:
				m_arrMouseStates[iMouse] = EMouseState::RELEASED;
				break;
			}
		}
	}

	// 마우스의 움직임을 업데이트합니다.
	m_arrMouseDelta[0] = m_tMouseState.lX;
	m_arrMouseDelta[1] = m_tMouseState.lY;
	m_arrMouseDelta[2] = m_tMouseState.lZ;

	// 마우스를 스크린 가운데로 이동합니다.
	if (m_bMouseHold)
	{
		POINT ptCursor = { g_iWinSizeX >> 1, g_iWinSizeY >> 1 };
		ClientToScreen(m_hWnd, &ptCursor);
		SetCursorPos(ptCursor.x, ptCursor.y);
	}
}

void CInputManager::Release()
{
	Safe_Release(m_pDIKeyDevice);
	Safe_Release(m_pDIMouseDevice);
	Safe_Release(m_pDeviceInput);

	m_upInstance.reset();
}
