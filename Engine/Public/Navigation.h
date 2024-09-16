#pragma once
#include "NonMeshComponent.h"

BEGIN(Engine)

class CCell;

/* 네비게이션 컴포넌트 추가할 때, "셰이더이름, 디스크립션" 세팅 해줘야 함. */
/* 네비게이션을 가지는 액터(지형이나 메시가 되겠지)는 매 프레임 */
/* 네비게이션의 "Update를 호출"해서 자신의 월드매트릭스로 네비게이션의 월드를 갱신해줘야 함 */
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
	virtual HRESULT InitializeBuffer() { return S_OK; } // 네비게이션은 셀들이 버퍼를 가지고 렌더를 할 것이기 때문에 버퍼가 필요없다.

public:
	// [ 나 여기로 움직여도 됩니까? ] return Yes or No, 매개변수는 움직여야 할 위치를 넣어준다.
	_bool IsMove(_fvector _vPosition, _int* _iCurNaviCellIndex, _float* _pPlaneY);

	// [ 새로운 Cell 추가 ]
	HRESULT AddCell(const _float3* _vPoints);

	// [ 마지막에 추가된 Cell 삭제 ]
	HRESULT RemoveLatestCell();

	// [ 초기에 액터의 위치가 어느 셀 인덱스에 위치하는지 세팅 ]
	_uint InitializeActorCellIndex(_vector _vActorPos);

	// [ Cell 벡터 참조 반환 ]
	vector<shared_ptr<CCell>>& GetVecCells() { return m_vecCells; }

private:
	// [ 셰이더에 던질 값 세팅 ]
	HRESULT BindShaderResources();

	// [ 삼각형의 이웃을 세팅 ]
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
	// [ 네비게이션을 가지는 객체의 월드매트릭스 ]
	static _float4x4 m_matTerrainWorld;

	// [ 데이터 파일 경로 ]
	wstring m_wstrDataFileName = L"";

private:
	// [ 네비게이션을 컴포넌트로 가지는 액터가 시작할 인덱스 위치 ]
	_int m_iStartCellIndex = - 1;

	// [ 현재 액터가 서있는 Cell 인덱스? ]
	_int m_iCurrentIndex = -1;
	
	// [ 네비게이션이 가지고있는 셀들 ]
	vector<shared_ptr<CCell>> m_vecCells;
};

END