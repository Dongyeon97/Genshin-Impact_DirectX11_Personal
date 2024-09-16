#pragma once

/* CCollisionManager */
// 1. �浹ü ������Ʈ�� ����
// 2. ����� �浹ü ������Ʈ���� �浹 ����
// 3. ���콺 ��ŷ

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
	void BlockEachOther(shared_ptr<CActor> _spSrc, shared_ptr<CActor> _spDst, const _float& _fTimeDelta);	// �浹�� �� ��ü�� �о�� �Լ�

	_int IntersectPlayer_MonsterAttack();
	_int IntersectMonster_PlayerAttack();

#pragma region ���콺 ��ŷ
public:
	// [ ������Ʈ �߰� ] -> ��ŷ��
	void AddNonMeshComponent(std::weak_ptr<CNonMeshComponent> _wpPrimitiveComponent);
	void AddNonAnimModel(std::weak_ptr<CNonAnimModel> _wpNonAnimModel);

public:
	// [ ���콺��ŷ ]
	static _vector IntersectRayToNonMesh(_matrix _matCamWorld, POINT _tPoint, _uint _iWidth, _uint _iHeight);
	static _vector IntersectRayToModel(_fvector _vRayOrigin, POINT _tPoint, _uint _iWidth, _uint _iHeight);
#pragma endregion ���콺 ��ŷ
/********************
	Getter / Setter
********************/
public:

#pragma region ���콺 ��ŷ
public:
	vector<shared_ptr<CNonMeshComponent>>& GetVecNonMeshs() { return m_vecNonMeshComponents; }
	vector<shared_ptr<CNonAnimModel>>& GetVecNonAnimModels() { return m_vecNonAnimModels; }

#pragma endregion ���콺 ��ŷ
/********************
	Data Members
********************/
private:
	// [ �÷��̾� ��ü �ݶ��̴� ]
	vector<shared_ptr<CCollider>> m_vecPlayerCollider;
	// [ ���� ��ü �ݶ��̴� ]
	vector<shared_ptr<CCollider>> m_vecMonsterCollider;

	// [ �÷��̾� ���� �ݶ��̴� ]
	vector<shared_ptr<CAttackCollider>> m_vecPlayerAttackCollider;
	// [ ���� ���� �ݶ��̴� ]
	vector<shared_ptr<CAttackCollider>> m_vecMonsterAttackCollider;

#pragma region ���콺 ��ŷ
private:
	// [ ��ŷ�� ����� �Ǵ� ������Ʈ�� ]
	vector<shared_ptr<CNonMeshComponent>> m_vecNonMeshComponents;
	vector<shared_ptr<CNonAnimModel>> m_vecNonAnimModels;

#pragma endregion ���콺 ��ŷ
};

END