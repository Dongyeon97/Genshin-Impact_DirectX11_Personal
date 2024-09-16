#pragma once

class CImguiManager
{
	DECLARE_SINGLETON(CImguiManager)

public:
	HRESULT Initialize();
	void UpdateBegin();
	void UpdateEnd();
	void Render();
	void Release();

private:
	ID3D11Device* m_pDevice = nullptr;

	ID3D11DeviceContext* m_pDeviceContext = nullptr;

private:
	_float* m_vBackGroundColor;
};
