#include "pch.h"
#include "NormalMonster.h"

#include "MonsterHPbar.h"
#include "MonsterHPbar_Frame.h"

void CNormalMonster::Initialize()
{
	CMonster::Initialize();
}

_int CNormalMonster::PreUpdate(const _float& _fTimeDelta)
{
	return CMonster::PreUpdate(_fTimeDelta);
}

_int CNormalMonster::Update(const _float& _fTimeDelta)
{
	return CMonster::Update(_fTimeDelta);
}

_int CNormalMonster::LateUpdate(const _float& _fTimeDelta)
{
	return CMonster::LateUpdate(_fTimeDelta);
}

void CNormalMonster::Render()
{
	CMonster::Render();
}

void CNormalMonster::EndPlay()
{
	CMonster::EndPlay();
}

void CNormalMonster::Release()
{
	CMonster::Release();
}

HRESULT CNormalMonster::ReadyHPbar(_float _fOffsetY)
{
	shared_ptr<CMonsterHPbar_Frame> spHPbar_Frame = CreateActor<CMonsterHPbar_Frame>(L"Hpbar_Frame" + to_wstring(g_iNumMonster++));
	if (nullptr == spHPbar_Frame)
		return E_FAIL;

	spHPbar_Frame->SetShaderFileName(L"Shader_UI_MonsterHPbar_Frame");
	spHPbar_Frame->SetTextureFileName(L"UI_HPBar_Two");
	spHPbar_Frame->SetSize({ 132.f, 11.f });
	spHPbar_Frame->SetOffsetY(_fOffsetY);
	m_vecHPbar.push_back(spHPbar_Frame);

	shared_ptr<CMonsterHPbar> spHPbar = CreateActor<CMonsterHPbar>(L"Hpbar" + to_wstring(g_iNumMonster));
	if (nullptr == spHPbar)
		return E_FAIL;

	spHPbar->SetShaderFileName(L"Shader_UI_MonsterHPbar");
	spHPbar->SetTextureFileName(L"UI_HPBar_Two");
	spHPbar->SetSize({ 128.f, 10.f });
	spHPbar->SetOffsetY(_fOffsetY);
	m_vecHPbar.push_back(spHPbar);

	return S_OK;
}

void CNormalMonster::SetHPBarEnable(_bool _bEnable)
{
	for (auto& iter : m_vecHPbar)
		iter->SetEnable(_bEnable);
}
