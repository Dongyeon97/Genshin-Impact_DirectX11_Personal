#include "Engine_pch.h"
#include "InputManager.h"

IMPLEMENT_SINGLETON(CInputManager)

HRESULT CInputManager::Initialize(HINSTANCE _hInst, HWND _hWnd)
{
	m_hInst = _hInst;
	m_hWnd = _hWnd;

	// DInput �İ�ü�� �����ϴ� �Լ�
	if (FAILED(DirectInput8Create(_hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pDeviceInput,
		nullptr)))
		return E_FAIL;

	// Ű���� ��ü ����
	if (FAILED(m_pDeviceInput->CreateDevice(GUID_SysKeyboard, &m_pDIKeyDevice, nullptr)))
		return E_FAIL;

	// ������ Ű���� ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pDIKeyDevice->SetDataFormat(&c_dfDIKeyboard);

	// ��ġ�� ���� �������� �������ִ� �Լ�, (Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�)
	m_pDIKeyDevice->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pDIKeyDevice->Acquire();


	// ���콺 ��ü ����
	if (FAILED(m_pDeviceInput->CreateDevice(GUID_SysMouse, &m_pDIMouseDevice, nullptr)))
		return E_FAIL;

	// ������ ���콺 ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pDIMouseDevice->SetDataFormat(&c_dfDIMouse);

	// ��ġ�� ���� �������� �������ִ� �Լ�, Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�
	m_pDIMouseDevice->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pDIMouseDevice->Acquire();


	return S_OK;
}

void CInputManager::Update()
{
	// Ű���� �Է��� ������Ʈ �մϴ�.
	char asciiKeys[256];
	assert(SUCCEEDED(m_pDIKeyDevice->GetDeviceState(256, asciiKeys)));

	for (_uint iKey = 0; iKey < 256; iKey++)
	{
		// Ű�� ���� ���
		if (asciiKeys[iKey] & 0x80)
		{
			switch (m_arrKeyStates[iKey])
			{
				// ������ �����ų� ������ ���� ���
			case EKeyState::DOWN:
			case EKeyState::PRESSED:
				m_arrKeyStates[iKey] = EKeyState::PRESSED;
				break;

				// ������ �ðų� ���� ���� ���
			case EKeyState::UP:
			case EKeyState::RELEASED:
				m_arrKeyStates[iKey] = EKeyState::DOWN;
				break;
			}
		}

		// Ű�� ������ ���� ���
		else
		{
			switch (m_arrKeyStates[iKey])
			{
				// ������ �����ų� ������ ���� ���
			case EKeyState::DOWN:
			case EKeyState::PRESSED:
				m_arrKeyStates[iKey] = EKeyState::UP;
				break;

				// ������ �ðų� ���� ���� ���
			case EKeyState::UP:
			case EKeyState::RELEASED:
				m_arrKeyStates[iKey] = EKeyState::RELEASED;
				break;
			}
		}
	}

	// ���콺 �Է��� ������Ʈ �մϴ�.
	DIMOUSESTATE m_tMouseState;
	assert(SUCCEEDED(m_pDIMouseDevice->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState)));

	for (_uint iMouse = 0; iMouse < 3; iMouse++)
	{
		// Ű�� ���� ���
		if (m_tMouseState.rgbButtons[iMouse])
		{
			switch (m_arrMouseStates[iMouse])
			{
				// ������ �����ų� ������ ���� ���
			case EMouseState::DOWN:
			case EMouseState::PRESSED:
				m_arrMouseStates[iMouse] = EMouseState::PRESSED;
				break;

				// ������ �ðų� ���� ���� ���
			case EMouseState::UP:
			case EMouseState::RELEASED:
				m_arrMouseStates[iMouse] = EMouseState::DOWN;
				break;
			}
		}

		// Ű�� ������ ���� ���
		else
		{
			switch (m_arrMouseStates[iMouse])
			{
				// ������ �����ų� ������ ���� ���
			case EMouseState::DOWN:
			case EMouseState::PRESSED:
				m_arrMouseStates[iMouse] = EMouseState::UP;
				break;

				// ������ �ðų� ���� ���� ���
			case EMouseState::UP:
			case EMouseState::RELEASED:
				m_arrMouseStates[iMouse] = EMouseState::RELEASED;
				break;
			}
		}
	}

	// ���콺�� �������� ������Ʈ�մϴ�.
	m_arrMouseDelta[0] = m_tMouseState.lX;
	m_arrMouseDelta[1] = m_tMouseState.lY;
	m_arrMouseDelta[2] = m_tMouseState.lZ;

	// ���콺�� ��ũ�� ����� �̵��մϴ�.
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
