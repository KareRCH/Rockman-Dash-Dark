#pragma once

#include "BaseClass/Level.h"

BEGIN(Engine)


enum class EGameObject : _uint
{

};

/// <summary>
/// ������ ���ӿ�����Ʈ�� ��Ƴ��� Ŭ����
/// ������ü�� �����Ͽ� ������ �纻��ü�� ���̾�� �����Ͽ� ��Ƴ��´�.
/// �����ϰ� �ִ� �纻��ü���� Tick���� �Լ��� �ݺ������� ȣ�����ش�.
/// ������ ������ �������ָ�, �� �����Ǵ� ������ 
/// </summary>
class CObjectMgr final : public CBase
{
	DERIVED_CLASS(CBase, CObjectMgr)

private:
	explicit CObjectMgr() = default;
	virtual ~CObjectMgr() = default;

public:
	HRESULT				Initialize(_uint iNumLevels);
	void				Priority_Tick(const _float& fTimeDelta);
	_int				Tick(const _float& fTimeDelta);
	void				Late_Tick(const _float& fTimeDelta);

public:
	static CObjectMgr*	Create(_uint iNumLevels);
private:
	virtual void		Free();

#pragma region ������Ÿ�� ����
public:
	HRESULT				Add_Prototype(const wstring& strTag, const wstring& strPrototypeKey, class CGameObject* pPrototype);
	HRESULT				Add_CloneObject(const wstring& strPrototypeKey, void* pArg);
	class CGameObject*	Find_Prototype(const wstring& strPrototypeKey);
	void				Clear_Prototypes(const wstring& strContainTag);

private:
	_unmap<wstring, class CGameObject*>			m_mapPrototypes;			// ������Ʈ�� ������Ÿ��  
#pragma endregion



	
#pragma region ������Ʈ ����
public:
	// ������Ʈ�� �߰��� �� �ڵ����� �߰��� ���� �±׸� �����Ѵ�.
	void				Set_LevelTag(const wstring& strLevelTag);
	// ���� ������Ʈ�� �߰��Ѵ�. 
	HRESULT				Add_GameObject(class CGameObject* pObj);
	// �߰� ���� �±׿� �Բ� ���� ������Ʈ�� �߰��Ѵ�. �ַ� �������� ��ü�� �߰��� �� �����Ѵ�.
	HRESULT				Add_GameObject(const wstring& strLevelTag, class CGameObject* pObj);
	// ������Ʈ�� ID ��� �˻�
	class CGameObject*	Find_GameObjectByID(_uint iFindID);
	// ������Ʈ�� �ܼ� �ε��� ��� �˻�, ������ ����
	class CGameObject*	Find_GameObjectByIndex(_uint iIndex);
	// �̸� �˻�
	class CGameObject* Find_GameObjectByName(const wstring& strName);
	// �ش� ���� �±װ� �޸� ��� ���� ������Ʈ�� ��ȯ�ϴ�. ������ ���δ�.
	vector<class CGameObject*> Get_AllGameObjectFromLevel(const wstring& strLevelTag);
	// Ư�� ���� �±׸� ���� ��ü�� �����Ѵ�.
	void				Clear_GameObject(const wstring& strLevelTag);

private:
	// ���� ���� �Լ�, ������� ä���ش�. 0(n)�� �ð����⵵�� ���Ѵ�. �ַ� ������Ʈ�� ���� �� ���ȴ�.
	void				Straighten_GameObjects();

private:
	_uint							m_iGiveObjectID = 0;		// �ο��Ǵ� ���� ������Ʈ�� ID
	vector<class CGameObject*>		m_vecGameObjects;			// ���� ������Ʈ �������
	wstring							m_strLevelTag;				// ������ ������Ʈ�� ���� �ο� ���� �±�
#pragma endregion




#pragma region ���� ������Ʈ ����
public:
	void				RegistToTick_GameObjects();

private:
	list<class CGameObject*>		m_listTickObjects;			// ������ ������Ʈ ����Ʈ  
#pragma endregion



#pragma region ���� ���� ������Ʈ
public:


private:

#pragma endregion

};

END