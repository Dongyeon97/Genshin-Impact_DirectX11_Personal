#include "Engine_pch.h"
#include "PrefabManager.h"

IMPLEMENT_SINGLETON(CPrefabManager)

HRESULT CPrefabManager::Initialize()
{
	return S_OK;
}

void CPrefabManager::Release()
{
	m_vecPrefabData.clear();

	// �Ŵ��� �ı�
	m_upInstance.reset();
}

MODEL_DATA CPrefabManager::FindModelData(wstring _wstrModelName)
{
	for (const auto& pair : m_vecPrefabData)
	{
		if (pair.first == _wstrModelName)
			return pair.second;
	}
	return MODEL_DATA();
}

void CPrefabManager::AddModelData(wstring _wstrModelName, MODEL_DATA _tModelData)
{
	auto iter = find_if(m_vecPrefabData.begin(), m_vecPrefabData.end(), [&](auto pair) {
		if (pair.first == _wstrModelName)
			return true;
		return false; });

	if (iter != m_vecPrefabData.end())
	{
		// MSG_BOX("�ߺ��Ǵ� �� �̸��� �����մϴ�.");
		return;
	}

	m_vecPrefabData.push_back(make_pair(_wstrModelName, _tModelData));
}

void CPrefabManager::RemoveModelData(wstring _wstrModelName)
{
	for (auto iter = m_vecPrefabData.begin(); iter != m_vecPrefabData.end(); ++iter)
	{
		if ((*iter).first == _wstrModelName)
		{
			// (*iter).second.reset();
			m_vecPrefabData.erase(iter);
			break;
		}
	}
}

MODEL_DATA* CPrefabManager::GetModelData(wstring _wstrModelName)
{
	for (auto iter = m_vecPrefabData.begin(); iter != m_vecPrefabData.end(); ++iter)
	{
		if ((*iter).first == _wstrModelName)
			return &(*iter).second;
	}

	return nullptr;
}

MODEL_DATA* CPrefabManager::GetModelData_Static(wstring _wstrModelName)
{
	for (auto iter = m_vecPrefabData_Static.begin(); iter != m_vecPrefabData_Static.end(); ++iter)
	{
		if ((*iter).first == _wstrModelName)
			return &(*iter).second;
	}

	return nullptr;
}

void CPrefabManager::ClearCurrentScene(_uint _iLevelIndex)
{
	// ����ƽ���� �ʱ�ȭ���� �ʽ��ϴ�.
	m_vecPrefabData.clear();
}

HRESULT CPrefabManager::WriteBones(MY_AINODE& _MyaiNode, ofstream& _Ostr)
{
	// ��ִ� ���� ��� Bone�����Ͱ� �������� �ʴ´�. -> �� �� ������ ä���ش�.
	//if (0 >= _MyaiNode.iNumChildren)
	//{
	//	char			szBoneName[MAX_PATH] = "";
	//	XMFLOAT4X4		matTransformation;
	//	XMStoreFloat4x4(&matTransformation, XMMatrixIdentity());
	//	unsigned int iNumChildren = 0;

	//	_Ostr.write(szBoneName, MAX_PATH);
	//	_Ostr.write((char*)(&matTransformation), sizeof(XMFLOAT4X4));
	//	_Ostr.write((char*)(&iNumChildren), sizeof(unsigned int));
	//	//for (_uint i = 0; i < _MyaiNode.iNumChildren; ++i)
	//	//{
	//	//	WriteBones(_MyaiNode.vecChildren[i], _Ostr);
	//	//}
	//	return S_OK;
	//}

	// MY_AINODE -> szBoneName ����
	_Ostr.write((char*)(&_MyaiNode.szBoneName), MAX_PATH);

	// MY_AINODE -> matTranslation ����
	_Ostr.write((char*)(&_MyaiNode.matTransformation), sizeof(XMFLOAT4X4));

	// MY_AINODE -> iNumChildren ����
	_Ostr.write((char*)(&_MyaiNode.iNumChildren), sizeof(unsigned int));

	// MY_AINODE -> vecChildren ����
	for (_uint i = 0; i < _MyaiNode.iNumChildren; ++i)
	{
		WriteBones(_MyaiNode.vecChildren[i], _Ostr);
	}

	return S_OK;
}

HRESULT CPrefabManager::ReadBones(MY_AINODE& _MyaiNode, ifstream& _Istr)
{
	// MY_AINODE -> szBoneName �б�
	_Istr.read((char*)(&_MyaiNode.szBoneName), MAX_PATH);

	// MY_AINODE -> matTranslation �б�
	_Istr.read((char*)(&_MyaiNode.matTransformation), sizeof(XMFLOAT4X4));

	// MY_AINODE -> iNumChildren �б�
	_Istr.read((char*)(&_MyaiNode.iNumChildren), sizeof(unsigned int));

	// MY_AINODE -> Children���Ϳ� �����͸� ä��� ���� resize
	_MyaiNode.vecChildren.resize(_MyaiNode.iNumChildren);

	// MY_AINODE -> vecChildren �б�
	for (_uint i = 0; i < _MyaiNode.iNumChildren; ++i)
	{
		ReadBones(_MyaiNode.vecChildren[i], _Istr);
	}

	return S_OK;
}
