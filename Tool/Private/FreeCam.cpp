#include "pch.h"
#include "FreeCam.h"

#include "InputManager.h"
#include "CameraManager.h"
#include "CollisionManager.h"

#include "Level.h"
#include "Level_Editor.h"
#include "Inspector.h"

#include "CameraComponent.h"

void FreeCam::Initialize()
{
	m_eRenderGroup = RENDER_NONBLEND;

	m_wpCameraComponent = CreateActorComponent<CCameraComponent>(L"CameraComponent");
	m_wpCameraComponent.lock()->SetFar(1000000.0f);
	CCameraManager::GetInstance()->AddCamera(L"Player_Camera", m_wpCameraComponent.lock());
	CCameraManager::GetInstance()->SetCurCamera(L"Player_Camera");

	CActor::Initialize();
}

_int FreeCam::PreUpdate(const _float& _fTimeDelta)
{
	return CActor::PreUpdate(_fTimeDelta);
}

_int FreeCam::Update(const _float& _fTimeDelta)
{
	// FocusOnViewport();
	KeyInput(_fTimeDelta);

	return CActor::Update(_fTimeDelta);
}

_int FreeCam::LateUpdate(const _float& _fTimeDelta)
{
	return CActor::LateUpdate(_fTimeDelta);
}

void FreeCam::Render()
{
	CActor::Render();
}

void FreeCam::EndPlay()
{
	CActor::EndPlay();
}

void FreeCam::Release()
{
	CActor::Release();
}

void FreeCam::KeyInput(const _float& _fTimeDelta)
{
	// if (!CInputManager::GetInstance()->GetMouseHold()) { return; }

	if (CInputManager::GetInstance()->IsKeyPressed(DIK_W))
	{
		_vector vPosition = GetPositionVector();
		_vector vLook = GetLookVector();

		vPosition += XMVector3Normalize(vLook) * 3000.0f * _fTimeDelta;

		SetActorTranslation_NoNavigation(vPosition);
	}

	if (CInputManager::GetInstance()->IsKeyPressed(DIK_S))
	{
		_vector vPosition = GetPositionVector();
		_vector vLook = GetLookVector();

		vPosition -= XMVector3Normalize(vLook) * 3000.0f * _fTimeDelta;

		SetActorTranslation_NoNavigation(vPosition);
	}

	if (CInputManager::GetInstance()->IsKeyPressed(DIK_A))
	{
		_vector vPosition = GetPositionVector();
		_vector vRight = GetRightVector();

		vPosition -= XMVector3Normalize(vRight) * 3000.0f * _fTimeDelta;

		SetActorTranslation_NoNavigation(vPosition);
	}

	if (CInputManager::GetInstance()->IsKeyPressed(DIK_D))
	{
		_vector vPosition = GetPositionVector();
		_vector vRight = GetRightVector();

		vPosition += XMVector3Normalize(vRight) * 3000.0f * _fTimeDelta;

		SetActorTranslation_NoNavigation(vPosition);
	}

	if (CInputManager::GetInstance()->IsKeyPressed(DIK_SPACE))
	{
		_vector vPosition = GetPositionVector();
		_vector vUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		vPosition += XMVector3Normalize(vUp) * 3000.0f * _fTimeDelta;

		SetActorTranslation_NoNavigation(vPosition);
	}

	if (CInputManager::GetInstance()->IsKeyPressed(DIK_LCONTROL))
	{
		_vector vPosition = GetPositionVector();
		_vector vUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		vPosition -= XMVector3Normalize(vUp) * 3000.0f * _fTimeDelta;

		SetActorTranslation_NoNavigation(vPosition);
	}

	if (CInputManager::GetInstance()->IsMousePressed(EMouseButtonType::LEFT_BUTTON))
	{
		_int iDeltaY = 0;
		if (iDeltaY = CInputManager::GetInstance()->GetMouseDeltaY())
		{
			_vector vRightVector = GetRightVector();
			_matrix matMouseRotateMatrix = GetMouseRotateMatrix();
			_matrix matRotateMatrix;

			matRotateMatrix = XMMatrixRotationAxis(vRightVector, XMConvertToRadians((_float)iDeltaY / 10.0f));
			matMouseRotateMatrix = XMMatrixMultiply(matMouseRotateMatrix, matRotateMatrix);
			SetMouseRotateMatrix(matMouseRotateMatrix);
		}

		_int iDeltaX = 0;
		if (iDeltaX = CInputManager::GetInstance()->GetMouseDeltaX())
		{
			_vector vUpVector = { 0.0f, 1.0f, 0.0f };
			_matrix matMouseRotateMatrix = GetMouseRotateMatrix();
			_matrix matRotateMatrix;

			matRotateMatrix = XMMatrixRotationAxis(vUpVector, XMConvertToRadians((_float)iDeltaX / 10.0f));
			matMouseRotateMatrix = XMMatrixMultiply(matMouseRotateMatrix, matRotateMatrix);
			SetMouseRotateMatrix(matMouseRotateMatrix);
		}
	}

	//if (CInputManager::GetInstance()->IsKeyDown(DIK_F2))
	//{
	//	m_bMouseHold = !m_bMouseHold;

	//	CInputManager::GetInstance()->SetMouseHold(m_bMouseHold);
	//}
}

void FreeCam::FocusOnViewport()
{
	bool bMouseInCenter = CInputManager::GetInstance()->GetMouseHold();

	// 커서를 가운데로 잠그거나 해제합니다.
	if (CInputManager::GetInstance()->IsKeyDown(DIK_F2))
	{
		bool bMouseInCenter = CInputManager::GetInstance()->GetMouseHold();
		CInputManager::GetInstance()->SetMouseHold(!bMouseInCenter);
	}
}
