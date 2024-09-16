#include "Engine_pch.h"
#include "CollisionManager.h"

#include "../../Client/Default/PlayerStateDefine.h"
#include "../../Client/Default/MonsterStateDefine.h"

#include "DeviceManager.h"
#include "CameraManager.h"

#include "Charactor.h"

#include "NonMeshComponent.h"
#include "NonAnimModel.h"
#include "Mesh.h"

#include "ColliderComponent.h"
#include "AttackCollider.h"
#include "StateMachine.h"

IMPLEMENT_SINGLETON(CCollisionManager)

HRESULT CCollisionManager::Initialize()
{
	return S_OK;
}

_int CCollisionManager::Update(const _float& _fTimeDelta)
{
	IntersectPlayer_Monster(_fTimeDelta);
	IntersectMonster_Monster(_fTimeDelta);

	IntersectPlayer_MonsterAttack();
	IntersectMonster_PlayerAttack();

	return S_OK;
}

void CCollisionManager::ClearCurrentScene()
{
	// [ �÷��̾� ��ü �ݶ��̴� ]
	/*for (auto& Collider : m_vecPlayerCollider)
	{
		if(nullptr != Collider)
		{
			Collider->Release();
			Collider.reset();
		}
	}
	m_vecPlayerCollider.clear();*/

	// [ ���� ��ü �ݶ��̴� ]
	for (auto& Collider : m_vecMonsterCollider)
	{
		if (nullptr != Collider)
		{
			Collider->Release();
			Collider.reset();
		}
	}
	m_vecMonsterCollider.clear();

	/*for (auto& Collider : m_vecPlayerAttackCollider)
	{
		if (nullptr != Collider)
		{
			Collider->Release();
			Collider.reset();
		}
	}
	m_vecPlayerAttackCollider.clear();*/

	for (auto& Collider : m_vecMonsterAttackCollider)
	{
		if (nullptr != Collider)
		{
			Collider->Release();
			Collider.reset();
		}
	}
	m_vecMonsterAttackCollider.clear();
}

void CCollisionManager::AddPlayerBodyCollider(weak_ptr<CCollider> _wpCollider)
{
	if (!_wpCollider.expired())
		m_vecPlayerCollider.push_back(_wpCollider.lock());
}

void CCollisionManager::RemovePlayerBodyCollider(weak_ptr<CCollider> _wpCollider)
{
	if (!_wpCollider.expired())
	{
		for (auto iter = m_vecPlayerCollider.begin(); iter != m_vecPlayerCollider.end(); ++iter)
		{
			if (*iter == _wpCollider.lock())
			{
				m_vecPlayerCollider.erase(iter);
				break;
			}
		}
	}
}

void CCollisionManager::AddMonsterBodyCollider(weak_ptr<CCollider> _wpCollider)
{
	if (!_wpCollider.expired())
		m_vecMonsterCollider.push_back(_wpCollider.lock());
}

void CCollisionManager::RemoveMonsterBodyCollider(weak_ptr<CCollider> _wpCollider)
{
	if (!_wpCollider.expired())
	{
		for (auto iter = m_vecMonsterCollider.begin(); iter != m_vecMonsterCollider.end(); ++iter)
		{
			if (*iter == _wpCollider.lock())
			{
				m_vecMonsterCollider.erase(iter);
				break;
			}
		}
	}
}

void CCollisionManager::AddPlayerAttackCollider(weak_ptr<CAttackCollider> _wpAtkCollider)
{
	// �ߺ� �˻�
	for (auto& iter : m_vecPlayerAttackCollider)
	{
		if (_wpAtkCollider.lock() == iter)
			return;
	}

	if (!_wpAtkCollider.expired())
		m_vecPlayerAttackCollider.push_back(_wpAtkCollider.lock());
}

void CCollisionManager::RemovePlayerAttackCollider(weak_ptr<CAttackCollider> _wpAtkCollider)
{
	if (!_wpAtkCollider.expired())
	{
		for (auto iter = m_vecPlayerAttackCollider.begin(); iter != m_vecPlayerAttackCollider.end(); ++iter)
		{
			if (*iter == _wpAtkCollider.lock())
			{
				m_vecPlayerAttackCollider.erase(iter);
				break;
			}
		}
	}
}

void CCollisionManager::AddMonsterAttackCollider(weak_ptr<CAttackCollider> _wpAtkCollider)
{
	// �ߺ� �˻�
	for (auto& iter : m_vecMonsterAttackCollider)
	{
		if (_wpAtkCollider.lock() == iter)
			return;
	}

	if (!_wpAtkCollider.expired())
		m_vecMonsterAttackCollider.push_back(_wpAtkCollider.lock());
}

void CCollisionManager::RemoveMonsterAttackCollider(weak_ptr<CAttackCollider> _wpAtkCollider)
{
	if (!_wpAtkCollider.expired())
	{
		for (auto iter = m_vecMonsterAttackCollider.begin(); iter != m_vecMonsterAttackCollider.end(); ++iter)
		{
			if (*iter == _wpAtkCollider.lock())
			{
				m_vecMonsterAttackCollider.erase(iter);
				break;
			}
		}
	}
}

void CCollisionManager::ClearCurrentScene(_uint _iLevelIndex)
{
	m_vecPlayerCollider.clear();

	m_vecMonsterCollider.clear();


	m_vecPlayerAttackCollider.clear();

	m_vecMonsterAttackCollider.clear();


	m_vecNonMeshComponents.clear();

	m_vecNonAnimModels.clear();
}

_int CCollisionManager::IntersectPlayer_Monster(const _float& _fTimeDelta)
{
	for (auto& pPlayerCol : m_vecPlayerCollider)
	{
		if (!pPlayerCol->GetOwnerActor().expired())
		{
			// ���Ͱ� Ȱ��ȭ ���°� �ƴ϶�� ���
			if (!pPlayerCol->GetOwnerActor().lock()->GetActorUse())
				continue;
		}

		for (auto& pMonsterCol : m_vecMonsterCollider)
		{
			if (!pMonsterCol->GetOwnerActor().expired())
			{
				// ���Ͱ� Ȱ��ȭ ���°� �ƴ϶�� ���
				if (!pMonsterCol->GetOwnerActor().lock()->GetActorUse())
					continue;
			}

			// ���� �÷��̾� �ݶ��̴��� ���� �ݶ��̴��� �浹�ߴٸ�, ���θ� �о��.
			if (pPlayerCol->Intersect(pMonsterCol.get()))
			{
				shared_ptr<CActor> spPlayer = pPlayerCol->GetOwnerActor().lock();
				shared_ptr<CActor> spMonster = pMonsterCol->GetOwnerActor().lock();

				BlockEachOther(spPlayer, spMonster, _fTimeDelta);
			}
		}
	}

	return S_OK;
}

_int CCollisionManager::IntersectMonster_Monster(const _float& _fTimeDelta)
{
	for (auto& pMonsterSrc : m_vecMonsterCollider)
	{
		if (!pMonsterSrc->GetOwnerActor().expired())
		{
			// ���Ͱ� Ȱ��ȭ ���°� �ƴ϶�� ���
			if (!pMonsterSrc->GetOwnerActor().lock()->GetActorUse())
				continue;
		}

		for (auto& pMonsterDst : m_vecMonsterCollider)
		{
			if (pMonsterSrc == pMonsterDst)
				continue;

			if (!pMonsterDst->GetOwnerActor().expired())
			{
				// ���Ͱ� Ȱ��ȭ ���°� �ƴ϶�� ���
				if (!pMonsterDst->GetOwnerActor().lock()->GetActorUse())
					continue;
			}

			// ���� ���� �ݶ��̴��� ���� �ݶ��̴��� �浹�ߴٸ�, ���θ� �о��.
			if (pMonsterSrc->Intersect(pMonsterDst.get()))
			{
				shared_ptr<CActor> spMonSrc = pMonsterSrc->GetOwnerActor().lock();
				shared_ptr<CActor> spMonDst = pMonsterDst->GetOwnerActor().lock();

				BlockEachOther(spMonSrc, spMonDst, _fTimeDelta);
			}
		}
	}

	return S_OK;
}

void CCollisionManager::BlockEachOther(shared_ptr<CActor> _spSrc, shared_ptr<CActor> _spDst, const _float& _fTimeDelta)
{
	// Src���� Dst�� ���ϴ� ���⺤�͸� ���Ѵ�. Src�� �ϴ� �÷��̾��� �����غ���
	_vector vSrcToDst = _spDst->GetActorTranslation() - _spSrc->GetActorTranslation();
	vSrcToDst = XMVectorSetY(vSrcToDst, 0.0f);
	vSrcToDst = XMVector3Normalize(vSrcToDst);

	// Dst�� vSrcToDst �������� �δ�.
	_spDst->SetActorTranslation(_spDst->GetActorTranslation() + vSrcToDst * 250.f * _fTimeDelta, &_spDst->GetCurNaviCellIndex());

	// Src�� vSrcToDst�� �ݴ�������� �δ�.
	_spSrc->SetActorTranslation(_spSrc->GetActorTranslation() - vSrcToDst * 250.f * _fTimeDelta, &_spSrc->GetCurNaviCellIndex());
}

_int CCollisionManager::IntersectPlayer_MonsterAttack()
{
	for (auto& spMonsterAtkCol : m_vecMonsterAttackCollider) // Dst
	{
		// ���Ͱ� Ȱ��ȭ �Ǿ����� ������ �Ѿ��.
		if (!spMonsterAtkCol->GetOwnerActor().expired())
		{
			if (!spMonsterAtkCol->GetOwnerActor().lock()->GetActorUse())
				continue;
		}

		// ������ ���� �ݶ��̴��� Ȱ��ȭ �Ǿ����� ������ �Ѿ��.
		if (!spMonsterAtkCol->GetISCollideOn())
			continue;

		for (auto& spPlayerCol : m_vecPlayerCollider) // Src
		{
			// ���Ͱ� Ȱ��ȭ �Ǿ����� ������ �Ѿ��.
			if (!spPlayerCol->GetOwnerActor().expired())
			{
				if (!spPlayerCol->GetOwnerActor().lock()->GetActorUse())
					continue;
			}

			// �÷��̾��� �ݶ��̴��� Ȱ��ȭ �Ǿ����� ������ �Ѿ��.
			if (!spPlayerCol->GetISCollideOn())
				continue;

			// ���� ����
			ATTACK_DESC AttackDesc = spMonsterAtkCol->GetAttackDesc();

			// �÷��̾� ������
			shared_ptr<CCharactor> spPlayer = dynamic_pointer_cast<CCharactor>(spPlayerCol->GetOwnerActor().lock());

			// Ȱ��ȭ�� ������ ���� �ݶ��̴��� �÷��̾ �浹�ߴٸ�,
			if (spPlayerCol->Intersect(spMonsterAtkCol.get()))
			{
				if (!spMonsterAtkCol->GetISCollideOn())
					continue;

				unordered_set<shared_ptr<CCollider>>& usetColliders = spPlayerCol->GetColliderComponents();

				// ������ OnCollisionEnterȣ�� -> StateMachine���� RequestChangeState ȣ��
				// 1. ������ �浹���� �ʰ� ��� �浹�� ���
				if (usetColliders.find(spMonsterAtkCol) == usetColliders.end())
				{
					spPlayer->OnCollisionEnter(spPlayerCol, spMonsterAtkCol, AttackDesc);

					usetColliders.insert(spMonsterAtkCol);
					usetColliders.insert(spPlayerCol);
				}
				// 2. ������ �̹� �浹�� ���
				else
				{
					spPlayer->OnCollisionStay(spPlayerCol, spMonsterAtkCol, AttackDesc);
				}
			}
			// �÷��̾��� ������ ���Ϳ� �浹���� ���� ���
			else
			{
				unordered_set<shared_ptr<CCollider>>& usetColliders = spPlayerCol->GetColliderComponents();

				// 3. ������ �浹 ���̿�����, ��� �浹���� ��� ���
				if (usetColliders.find(spMonsterAtkCol) != usetColliders.end())
				{
					spPlayer->OnCollisionExit(spPlayerCol, spMonsterAtkCol, AttackDesc);

					usetColliders.erase(spPlayerCol);
					usetColliders.erase(spMonsterAtkCol);
				}
			}
		}
	}

	return S_OK;
}

_int CCollisionManager::IntersectMonster_PlayerAttack()
{
	for (auto& spPlayerAtkCol : m_vecPlayerAttackCollider) // Dst
	{
		// ���Ͱ� Ȱ��ȭ �Ǿ����� ������ �Ѿ��.
		if (!spPlayerAtkCol->GetOwnerActor().expired())
		{
			if (!spPlayerAtkCol->GetOwnerActor().lock()->GetActorUse())
				continue;
		}

		// �÷��̾��� ���� �ݶ��̴��� Ȱ��ȭ �Ǿ����� ������ �Ѿ��.
		if (!spPlayerAtkCol->GetISCollideOn())
			continue;

		for (auto& spMonsterCol : m_vecMonsterCollider) // Src
		{
			// ���Ͱ� Ȱ��ȭ �Ǿ����� ������ �Ѿ��.
			if (!spMonsterCol->GetOwnerActor().expired())
			{
				if (!spMonsterCol->GetOwnerActor().lock()->GetActorUse())
					continue;
			}

			// ������ �ݶ��̴��� Ȱ��ȭ �Ǿ����� ������ �Ѿ��.
			if (!spMonsterCol->GetISCollideOn())
				continue;

			// ���� ����
			ATTACK_DESC AttackDesc = spPlayerAtkCol->GetAttackDesc();

			// ���� ������
			shared_ptr<CCharactor> spMonster = dynamic_pointer_cast<CCharactor>(spMonsterCol->GetOwnerActor().lock());

			// Ȱ��ȭ�� �÷��̾��� ���� �ݶ��̴��� ���Ͱ� �浹�ߴٸ�,
			if (spMonsterCol->Intersect(spPlayerAtkCol.get()))
			{
				if (!spPlayerAtkCol->GetISCollideOn())
					continue;

				unordered_set<shared_ptr<CCollider>>& usetColliders = spMonsterCol->GetColliderComponents();

				// ������ OnCollisionEnterȣ�� -> StateMachine���� RequestChangeState ȣ��
				// 1. ������ �浹���� �ʰ� ��� �浹�� ���
				if (usetColliders.find(spPlayerAtkCol) == usetColliders.end())
				{
					spMonster->OnCollisionEnter(spMonsterCol, spPlayerAtkCol, AttackDesc);

					if (!spPlayerAtkCol->GetOwnerActor().expired())
						spPlayerAtkCol->GetOwnerActor().lock()->OnCollisionEnter(spMonsterCol, spPlayerAtkCol, AttackDesc);


					usetColliders.insert(spPlayerAtkCol);
					usetColliders.insert(spMonsterCol);
				}
				// 2. ������ �̹� �浹�� ���
				else
				{
					spMonster->OnCollisionStay(spMonsterCol, spPlayerAtkCol, AttackDesc);

					if (!spPlayerAtkCol->GetOwnerActor().expired())
						spPlayerAtkCol->GetOwnerActor().lock()->OnCollisionStay(spMonsterCol, spPlayerAtkCol, AttackDesc);
				}
			}
			// �÷��̾��� ������ ���Ϳ� �浹���� ���� ���
			else
			{
				unordered_set<shared_ptr<CCollider>>& usetColliders = spMonsterCol->GetColliderComponents();

				// 3. ������ �浹 ���̿�����, ��� �浹���� ��� ���
				if (usetColliders.find(spPlayerAtkCol) != usetColliders.end())
				{
					spMonster->OnCollisionExit(spMonsterCol, spPlayerAtkCol, AttackDesc);

					if (!spPlayerAtkCol->GetOwnerActor().expired())
						spPlayerAtkCol->GetOwnerActor().lock()->OnCollisionExit(spMonsterCol, spPlayerAtkCol, AttackDesc);

					usetColliders.erase(spMonsterCol);
					usetColliders.erase(spPlayerAtkCol);
				}
			}
		}
	}

	return S_OK;
}

void CCollisionManager::AddNonMeshComponent(std::weak_ptr<CNonMeshComponent> _wpPrimitiveComponent)
{
	if(!_wpPrimitiveComponent.expired())
		m_vecNonMeshComponents.push_back(_wpPrimitiveComponent.lock());
}

void CCollisionManager::AddNonAnimModel(std::weak_ptr<CNonAnimModel> _wpNonAnimModel)
{
	if (!_wpNonAnimModel.expired())
		m_vecNonAnimModels.push_back(_wpNonAnimModel.lock());
}

// �Ű�����(ī�޶� ��ġ, ���콺 ������ GetCursorPos�Ѱ�)
_vector CCollisionManager::IntersectRayToNonMesh(_matrix _matCamWorld, POINT _tPoint, _uint _iWidth, _uint _iHeight)
{
	// _vRayOrigin : ������ǥ���� ī�޶� ��ġ
	// _tPoint : ����Ʈ���� ���콺������ ��ġ
	// _iWidth : ������â�� Width
	// _iHeight : ������â�� Height

	_float3 vMouseProj = {};

	// 1. ����Ʈ -> ����
	vMouseProj.x = ((2.0f * (_float)_tPoint.x) / (_float)_iWidth) - 1.0f;
	vMouseProj.y = (((2.0f * (_float)_tPoint.y) / (_float)_iHeight) - 1.0f) * -1.0f;
	vMouseProj.z = 0.0f;

	// 2. ���� -> ��
	_vector vMouseView = {};

	_matrix matProjInverse = CCameraManager::GetInstance()->GetProjMatrix();
	matProjInverse = XMMatrixInverse(0, matProjInverse);
	vMouseView = XMVector3Transform(XMLoadFloat3(&vMouseProj), matProjInverse);

	// 3. �� -> ����
	_vector vMouseWorld = {};

	_matrix matViewInverse = CCameraManager::GetInstance()->GetViewMatrix();
	matViewInverse = XMMatrixInverse(0, matViewInverse);
	vMouseWorld = XMVector3Transform(vMouseView, matViewInverse);

	// 4. ���� -> ����
	_vector vMouseLocal = {};

	_matrix matWorldInverse = XMMatrixInverse(0, _matCamWorld);
	vMouseLocal = XMVector3Transform(vMouseWorld, matWorldInverse);

	_vector vRayDir = vMouseLocal - _matCamWorld.r[3];

	vRayDir = XMVector3Normalize(vRayDir);

	// 5. Ray�浹 üũ
	_float fShortestDist = 1000.0f;

	vector<shared_ptr<CNonMeshComponent>>& vecNonMesh = CCollisionManager::GetInstance()->GetVecNonMeshs();

	for (_uint i = 0; i < vecNonMesh.size(); ++i)
	{
		_uint temp = vecNonMesh[i]->GetNumTriangles();

		for (_uint j = 0; j < vecNonMesh[i]->GetNumTriangles(); ++j)
		{
			_int iIndex_0 = (_int)(vecNonMesh[i]->GetVertexIndices(j)).x;
			_int iIndex_1 = (_int)(vecNonMesh[i]->GetVertexIndices(j)).y;
			_int iIndex_2 = (_int)(vecNonMesh[i]->GetVertexIndices(j)).z;

			_float fDist = 0.0f;

			if (DirectX::TriangleTests::Intersects(_matCamWorld.r[3], vRayDir,
				vecNonMesh[i]->GetVertexPos(iIndex_0),
				vecNonMesh[i]->GetVertexPos(iIndex_1),
				vecNonMesh[i]->GetVertexPos(iIndex_2), fDist))
			{
				if (fDist < fShortestDist)
				{
					fShortestDist = fDist;
					return _matCamWorld.r[3] + (vRayDir * fShortestDist);
				}
			}
		}
	}

	// ������� ������ �������͸� ��ȯ
	if (fShortestDist == 1000.0f/* || spShortestActor == nullptr*/)
		return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	else
	{
		// ������ ���� ��ġ���� ��ȯ
		return _matCamWorld.r[3] + (vRayDir * fShortestDist);
	}
}

_vector CCollisionManager::IntersectRayToModel(_fvector _vRayOrigin, POINT _tPoint, _uint _iWidth, _uint _iHeight)
{
	// _vRayOrigin : ������ǥ���� ī�޶� ��ġ
	// _tPoint : ����Ʈ���� ���콺������ ��ġ
	// _iWidth : ������â�� Width
	// _iHeight : ������â�� Height

	_float3 vMouseProj = {};

	// 1. ����Ʈ -> ����
	vMouseProj.x = ((2.0f * (_float)_tPoint.x) / (_float)_iWidth) - 1.0f;
	vMouseProj.y = (((2.0f * (_float)_tPoint.y) / (_float)_iHeight) - 1.0f) * -1.0f;
	vMouseProj.z = 0.0f;

	// 2. ���� -> ��
	_vector vMouseView = {};

	_matrix matProjInverse = CCameraManager::GetInstance()->GetProjMatrix();
	matProjInverse = XMMatrixInverse(0, matProjInverse);
	vMouseView = XMVector3Transform(XMLoadFloat3(&vMouseProj), matProjInverse);

	// 3. �� -> ����
	_vector vMouseWorld = {};

	_matrix matViewInverse = CCameraManager::GetInstance()->GetViewMatrix();
	matViewInverse = XMMatrixInverse(0, matViewInverse);
	vMouseWorld = XMVector3Transform(vMouseView, matViewInverse);

	// 4. ���� -> ����
	/*_vector vMouseLocal = {};

	_matrix matWorldInverse = XMMatrixInverse(0, _matCamWorld);
	vMouseLocal = XMVector3Transform(vMouseWorld, matWorldInverse);*/

	_vector vRayDir = vMouseWorld - _vRayOrigin;

	vRayDir = XMVector3Normalize(vRayDir);

	// 5. Ray�浹 üũ
	_float fShortestDist = 20000.0f;

	vector<shared_ptr<CNonAnimModel>>& vecModels = CCollisionManager::GetInstance()->GetVecNonAnimModels();

	for (_uint i = 0; i < vecModels.size(); ++i)
	{
		_uint iNumMeshs = vecModels[i]->GetNumMeshes();
		vector<shared_ptr<CMesh>>& vecMeshs = vecModels[i]->GetVecMeshs();

		for (_uint j = 0; j < iNumMeshs; ++j)
		{
			// �޽��� �ﰢ�� �� ��ŭ ��ȸ�� ���� ã�´�.
			_uint iNumTriangles = vecMeshs[j]->GetNumTriangles();
			for (_uint k = 0; k < iNumTriangles; ++k)
			{
				_int iIndex_0 = (_int)(vecMeshs[j]->GetVertexIndices(k)).x;
				_int iIndex_1 = (_int)(vecMeshs[j]->GetVertexIndices(k)).y;
				_int iIndex_2 = (_int)(vecMeshs[j]->GetVertexIndices(k)).z;

				_float fDist = 0.0f;

				if (DirectX::TriangleTests::Intersects(_vRayOrigin, vRayDir,
					vecMeshs[j]->GetVertexPos(iIndex_0),
					vecMeshs[j]->GetVertexPos(iIndex_1),
					vecMeshs[j]->GetVertexPos(iIndex_2), fDist))
				{
					if (fDist < fShortestDist)
						fShortestDist = fDist;
				}
			}
		}
	}

	// ������� ������ �������͸� ��ȯ
	if (fShortestDist == 20000.0f/* || spShortestActor == nullptr*/)
		return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	else
	{
		// ������ ���� ��ġ���� ��ȯ
		return _vRayOrigin + (vRayDir * fShortestDist);
	}
}

void CCollisionManager::Release()
{
	/* NonMesh �����̳� ���� */
	for (auto& spPrimitiveComponents : m_vecNonMeshComponents)
	{
		if (nullptr != spPrimitiveComponents)
		{
			spPrimitiveComponents->Release();
			spPrimitiveComponents.reset();
		}
	}
	m_vecNonMeshComponents.clear();

	/* Player �����̳� ���� */
	for (auto& spCollider : m_vecPlayerCollider)
	{
		if (nullptr != spCollider)
		{
			spCollider->Release();
			spCollider.reset();
		}
	}
	m_vecPlayerCollider.clear();

	/* PlayerAttack �����̳� ���� */
	for (auto& spCollider : m_vecPlayerAttackCollider)
	{
		if (nullptr != spCollider)
		{
			spCollider->Release();
			spCollider.reset();
		}
	}
	m_vecPlayerAttackCollider.clear();

	/* Monster �����̳� ���� */
	for (auto& spCollider : m_vecMonsterCollider)
	{
		if (nullptr != spCollider)
		{
			spCollider->Release();
			spCollider.reset();
		}
	}
	m_vecMonsterCollider.clear();

	/* MonsterAttack �����̳� ���� */
	for (auto& spCollider : m_vecMonsterAttackCollider)
	{
		if (nullptr != spCollider)
		{
			spCollider->Release();
			spCollider.reset();
		}
	}
	m_vecMonsterAttackCollider.clear();

	m_upInstance.reset();
}