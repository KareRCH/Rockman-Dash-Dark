#pragma once

#include "BaseClass/Level.h"
#include "Utility/DelegateTemplate.h"

BEGIN(Engine)

typedef MulticastDelegate<FastDelegate0<void>> MultiBgeinDelegate;

/// <summary>
/// 생성한 게임오브젝트를 모아놓는 클래스
/// 원형객체를 복제하여 생성한 사본객체를 레이어로 구분하여 모아놓는다.
/// 보관하고 있는 사본객체들의 Tick관련 함수를 반복적으로 호출해준다.
/// 월드의 영역을 관리해주며, 이 관리되는 영역을 
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


public:
	static _uint g_iCountClassID;

#pragma region 프로토타입 관리
public:
	HRESULT				Add_Prototype(const wstring& strTag, const wstring& strPrototypeKey, class CGameObject* pPrototype);
	HRESULT				Add_CloneObject(const wstring& strPrototypeKey, void* pArg);
	HRESULT				Add_CloneObject(FSerialData& InputData);
	class CGameObject*	Find_Prototype(const wstring& strPrototypeKey);
	void				Clear_Prototypes(const wstring& strContainTag);

private:
	map<wstring, class CGameObject*>			m_mapPrototypes;			// 오브젝트의 프로토타입  
#pragma endregion



	
#pragma region 오브젝트 관리
public:
	// 오브젝트를 추가할 때 자동으로 추가될 레벨 태그를 설정한다.
	void				Set_LevelTag(const wstring& strLevelTag);
	// 게임 오브젝트를 추가한다. 
	HRESULT				Add_GameObject(class CGameObject* pObj);
	// 추가 레벨 태그와 함께 게임 오브젝트를 추가한다. 주로 영구적인 객체를 추가할 때 설정한다.
	HRESULT				Add_GameObject(const wstring& strLevelTag, class CGameObject* pObj);
	// 오브젝트의 ID 기반 검색
	class CGameObject*	Find_GameObjectByID(_uint iFindID);
	// 오브젝트의 단순 인덱스 기반 검색, 툴에서 쓰임
	class CGameObject*	Find_GameObjectByIndex(_uint iIndex);
	// 이름 검색
	class CGameObject*	Find_GameObjectByName(const wstring& strName);
	// 해당 레벨 태그가 달린 모든 게임 오브젝트를 반환하다. 툴에서 쓰인다.
	vector<class CGameObject*> Get_AllGameObjectFromLevel(const wstring& strLevelTag);
	// 해당 Common 태그가 달린 모든 게임 오브젝트를 정지시킨다.
	void Pause_ObjectsByCommonTag(const wstring& strCommonTag, _bool bJustTick);
	// 다시 재생
	void Resume_ObjectsByCommonTag(const wstring& strCommonTag, _bool bJustTick);
	// 해당 레벨 태그가 달린 모든 게임 오브젝트를 정지시킨다.
	void Pause_ObjectsByLevelTag(const wstring& strLevelTag);
	// 다시 재생
	void Resume_ObjectsByLevelTag(const wstring& strLevelTag);
	// 특정 레벨 태그를 가진 객체를 제거한다.
	void				Clear_GameObject(const wstring& strLevelTag);

private:
	// 벡터 정리 함수, 빈공간을 채워준다. 0(n)의 시간복잡도를 지닌다. 주로 업데이트가 끝날 때 사용된다.
	void				Straighten_GameObjects();

private:
	_uint							m_iGiveObjectID = 0;		// 부여되는 게임 오브젝트의 ID
	vector<class CGameObject*>		m_vecGameObjects;			// 게임 오브젝트 저장공간
	set<wstring>					m_setObjectNames;			// 게임 오브젝트 이름 중복 검사용
	wstring							m_strLevelTag;				// 생성할 오브젝트에 대한 부여 레벨 태그
	MultiBgeinDelegate				m_EventBeginPlay;			// 오브젝트 플레이 시작 이벤트
#pragma endregion




#pragma region 루프 오브젝트 관리
public:
	void				RegistToTick_GameObjects();

private:
	list<class CGameObject*>		m_listTickObjects;			// 루프용 오브젝트 리스트  
#pragma endregion



#pragma region 영구 저장 오브젝트
public:


private:

#pragma endregion

};

END