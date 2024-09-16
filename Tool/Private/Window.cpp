#include "pch.h"
#include "Window.h"
#include "Level.h"

HRESULT CWindow::Initialize(std::weak_ptr<CLevel> _wpLevel)
{
	return S_OK;
}

_int CWindow::PreUpdate(const _float& _fTimeDelta)
{
	return S_OK;
}

_int CWindow::Update(const _float& _fTimeDelta)
{
	return S_OK;
}

_int CWindow::LateUpdate(const _float& _fTimeDelta)
{
	return S_OK;
}

void CWindow::Render()
{
}

void CWindow::EndPlay()
{
}

void CWindow::Release()
{
}
