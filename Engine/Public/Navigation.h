#pragma once
#include "NonMeshComponent.h"

BEGIN(Engine)

class CCell;

/* �׺���̼� ������Ʈ �߰��� ��, "���̴��̸�, ��ũ����" ���� ����� ��. */
/* �׺���̼��� ������ ����(�����̳� �޽ð� �ǰ���)�� �� ������ */
/* �׺���̼��� "Update�� ȣ��"�ؼ� �ڽ��� �����Ʈ������ �׺���̼��� ���带 ��������� �� */
class ENGINE_DLL CNavigation final : public CNonMeshComponent
{
public:
	explicit CNavigation() = default;
	virtual ~CNavigation() = default;

public:
	void Initialize() override;
	void Update(_fmatrix _matTerrainWorldMatrix);
	void Render() override;
	void Release() override;

private:
	virtual HRESULT InitializeBuffer() { return S_OK; } // �׺���̼��� ������ ���۸� ������ ������ �� ���̱� ������ ���۰� �ʿ����.

public:
	// [ �� ����� �������� �˴ϱ�? ] return Yes or No, �Ű������� �������� �� ��ġ�� �־��ش�.
	_bool IsMove(_fvector _vPosition, _int* _iCurNaviCellIndex, _float* _pPlaneY);

	// [ ���ο� Cell �߰� ]
	HRESULT AddCell(const _float3* _vPoints);

	// [ �������� �߰��� Cell ���� ]
	HRESULT RemoveLatestCell();

	// [ �ʱ⿡ ������ ��ġ�� ��� �� �ε����� ��ġ�ϴ��� ���� ]
	_uint InitializeActorCellIndex(_vector _vActorPos);

	// [ Cell ���� ���� ��ȯ ]
	vector<shared_ptr<CCell>>& GetVecCells() { return m_vecCells; }

private:
	// [ ���̴��� ���� �� ���� ]
	HRESULT BindShaderResources();

	// [ �ﰢ���� �̿��� ���� ]
	HRESULT MakeNeighbors();
	
public:
	void SetDataFileName(const wstring& _wstrDataFileName) { m_wstrDataFileName = _wstrDataFileName; }
	void SetStartCellIndex(_int _iStartCellIndex) 
	{ 
		m_iStartCellIndex = _iStartCellIndex; 
		m_iCurrentIndex = _iStartCellIndex;
	}

	_int GetCurCellIndex() { return m_iCurrentIndex; }

private:
	// [ �׺���̼��� ������ ��ü�� �����Ʈ���� ]
	static _float4x4 m_matTerrainWorld;

	// [ ������ ���� ��� ]
	wstring m_wstrDataFileName = L"";

private:
	// [ �׺���̼��� ������Ʈ�� ������ ���Ͱ� ������ �ε��� ��ġ ]
	_int m_iStartCellIndex = - 1;

	// [ ���� ���Ͱ� ���ִ� Cell �ε���? ]
	_int m_iCurrentIndex = -1;
	
	// [ �׺���̼��� �������ִ� ���� ]
	vector<shared_ptr<CCell>> m_vecCells;
};

END