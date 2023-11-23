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
	HRESULT Add_Prototype(const wstring& strPrototypeKey, class CGameObject* pPrototype);
	HRESULT Add_CloneObject(const wstring& strPrototypeKey, void* pArg);
	class CGameObject* Find_Prototype(const wstring& strPrototypeKey);
	void	Clear_Prototypes(const wstring& strContainTag);

private:
	_unmap<wstring, class CGameObject*>			m_mapPrototypes;			// ������Ʈ�� ������Ÿ��  
#pragma endregion



	
#pragma region ������Ʈ ����
public:
	HRESULT				Add_GameObject(class CGameObject* pObj);
	class CGameObject*	Find_GameObject(_uint iFindID);
	void				Clear_GameObject(const wstring& strLayerTag);

	// ���� ���� �Լ�, ������� ä���ش�. 0(n)�� �ð����⵵�� ���Ѵ�. �ַ� ������Ʈ�� ���� �� ���ȴ�.
private:
	void				Straighten_GameObjects();

private:
	_uint							m_iGiveObjectID = 0;		// �ο��Ǵ� ���� ������Ʈ�� ID
	vector<class CGameObject*>		m_vecGameObjects;			// ���� ������Ʈ �������  
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