#pragma once

/* CCollisionManager */
// 1. 충돌체 컴포넌트를 보관
// 2. 저장된 충돌체 컴포넌트간의 충돌 연산
// 3. 마우스 피킹

BEGIN(Engine)

class CActor;

class CNonMeshComponent;
class CNonAnimModel;
class CCollider;
class CAttackCollider;

class ENGINE_DLL CCollisionManager final
{
	DECLARE_SINGLETON(CCollisionManager)
/********************
	Framework
********************/
public:
	HRESULT Initialize();
	_int Update(const _float& _fTimeDelta);
	void Release();

/********************
	Methods
********************/
public:
	void ClearCurrentScene();
	void ClearCurrentScene(_uint _iLevelIndex);

public:
	void AddPlayerBodyCollider(weak_ptr<CCollider> _wpCollider);
	void RemovePlayerBodyCollider(weak_ptr<CCollider> _wpCollider);

	void AddMonsterBodyCollider(weak_ptr<CCollider> _wpCollider);
	void RemoveMonsterBodyCollider(weak_ptr<CCollider> _wpCollider);

public:
	void AddPlayerAttackCollider(weak_ptr<CAttackCollider> _wpAtkCollider);
	void RemovePlayerAttackCollider(weak_ptr<CAttackCollider> _wpAtkCollider);

	void AddMonsterAttackCollider(weak_ptr<CAttackCollider> _wpAtkCollider);
	void RemoveMonsterAttackCollider(weak_ptr<CAttackCollider> _wpAtkCollider);

private:
	_int IntersectPlayer_Monster(const _float& _fTimeDelta);
	_int IntersectMonster_Monster(const _float& _fTimeDelta);
	void BlockEachOther(shared_ptr<CActor> _spSrc, shared_ptr<CActor> _spDst, const _float& _fTimeDelta);	// 충돌한 두 객체를 밀어내는 함수

	_int IntersectPlayer_MonsterAttack();
	_int IntersectMonster_PlayerAttack();

#pragma region 마우스 피킹
public:
	// [ 컴포넌트 추가 ] -> 피킹용
	void AddNonMeshComponent(std::weak_ptr<CNonMeshComponent> _wpPrimitiveComponent);
	void AddNonAnimModel(std::weak_ptr<CNonAnimModel> _wpNonAnimModel);

public:
	// [ 마우스피킹 ]
	static _vector IntersectRayToNonMesh(_matrix _matCamWorld, POINT _tPoint, _uint _iWidth, _uint _iHeight);
	static _vector IntersectRayToModel(_fvector _vRayOrigin, POINT _tPoint, _uint _iWidth, _uint _iHeight);
#pragma endregion 마우스 피킹
/********************
	Getter / Setter
********************/
public:

#pragma region 마우스 피킹
public:
	vector<shared_ptr<CNonMeshComponent>>& GetVecNonMeshs() { return m_vecNonMeshComponents; }
	vector<shared_ptr<CNonAnimModel>>& GetVecNonAnimModels() { return m_vecNonAnimModels; }

#pragma endregion 마우스 피킹
/********************
	Data Members
********************/
private:
	// [ 플레이어 본체 콜라이더 ]
	vector<shared_ptr<CCollider>> m_vecPlayerCollider;
	// [ 몬스터 본체 콜라이더 ]
	vector<shared_ptr<CCollider>> m_vecMonsterCollider;

	// [ 플레이어 공격 콜라이더 ]
	vector<shared_ptr<CAttackCollider>> m_vecPlayerAttackCollider;
	// [ 몬스터 공격 콜라이더 ]
	vector<shared_ptr<CAttackCollider>> m_vecMonsterAttackCollider;

#pragma region 마우스 피킹
private:
	// [ 피킹이 대상이 되는 컴포넌트들 ]
	vector<shared_ptr<CNonMeshComponent>> m_vecNonMeshComponents;
	vector<shared_ptr<CNonAnimModel>> m_vecNonAnimModels;

#pragma endregion 마우스 피킹
};

END