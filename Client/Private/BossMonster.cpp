#include "pch.h"
#include "BossMonster.h"

#include "HPbar.h"
#include "HPbar_Frame.h"

void CBossMonster::Initialize()
{
	CMonster::Initialize();
}

_int CBossMonster::PreUpdate(const _float& _fTimeDelta)
{
	return CMonster::PreUpdate(_fTimeDelta);
}

_int CBossMonster::Update(const _float& _fTimeDelta)
{
	return CMonster::Update(_fTimeDelta);
}

_int CBossMonster::LateUpdate(const _float& _fTimeDelta)
{
	return CMonster::LateUpdate(_fTimeDelta);
}

void CBossMonster::Render()
{
	CMonster::Render();
}

void CBossMonster::EndPlay()
{
	CMonster::EndPlay();
}

void CBossMonster::Release()
{
	CMonster::Release();
}

HRESULT CBossMonster::ReadyHPbar()
{
	shared_ptr<CHPbar_Frame> spHPbar_Frame = CreateActor<CHPbar_Frame>(L"Andrius_Hpbar_Frame");
	if (nullptr == spHPbar_Frame)
		return E_FAIL;

	spHPbar_Frame->SetShaderFileName(L"Shader_UI_BossHPbar_Frame");
	spHPbar_Frame->SetTextureFileName(L"UI_SubBar_Outline");
	spHPbar_Frame->SetSize({ 457.5f, 13.5f });
	spHPbar_Frame->SetPosition({ Client::g_iWinSizeX * 0.5f, Client::g_iWinSizeY * 0.08f });
	spHPbar_Frame->SetEnable(true);
	m_vecHPbar.push_back(spHPbar_Frame);

	shared_ptr<CHPbar> spHPbar = CreateActor<CHPbar>(L"Andrius_Hpbar");
	if (nullptr == spHPbar)
		return E_FAIL;

	spHPbar->SetShaderFileName(L"Shader_UI_BossHPbar");
	spHPbar->SetTextureFileName(L"UI_SubBar_");
	spHPbar->SetSize({ 450.f, 9.f });
	spHPbar->SetPosition({ Client::g_iWinSizeX * 0.5f, Client::g_iWinSizeY * 0.08f });
	spHPbar->SetEnable(true);
	m_vecHPbar.push_back(spHPbar);

	// 양쪽 장식
	shared_ptr<CHPbar_Frame> spHPbar_L = CreateActor<CHPbar_Frame>(L"Andrius_Hpbar_L");
	if (nullptr == spHPbar_L)
		return E_FAIL;

	spHPbar_L->SetShaderFileName(L"Shader_UIBase");
	spHPbar_L->SetTextureFileName(L"UI_ImgBossBar_Bg_L");
	spHPbar_L->SetSize({ 30.f, 18.f });
	spHPbar_L->SetPosition({ 400.f, Client::g_iWinSizeY * 0.08f });
	spHPbar_L->SetEnable(true);
	m_vecHPbar.push_back(spHPbar_L);

	shared_ptr<CHPbar_Frame> spHPbar_R = CreateActor<CHPbar_Frame>(L"Andrius_Hpbar_R");
	if (nullptr == spHPbar_R)
		return E_FAIL;

	spHPbar_R->SetShaderFileName(L"Shader_UIBase");
	spHPbar_R->SetTextureFileName(L"UI_ImgBossBar_Bg_R");
	spHPbar_R->SetSize({ 30.f, 18.f });
	spHPbar_R->SetPosition({ 880.f, Client::g_iWinSizeY * 0.08f });
	spHPbar_R->SetEnable(true);
	m_vecHPbar.push_back(spHPbar_R);

	return S_OK;
}

void CBossMonster::SetHPBarEnable(_bool _bEnable)
{
	for (auto& iter : m_vecHPbar)
		iter->SetEnable(_bEnable);
}
