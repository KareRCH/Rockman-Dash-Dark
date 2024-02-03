#include "System/ObjectMgr.h"

#include "BaseClass/GameObject.h"
#include "System/GameInstance.h"
#include "System/RenderMgr.h"

_uint CObjectMgr::g_iCountClassID = 0;

HRESULT CObjectMgr::Initialize(_uint iNumLevels)
{
	m_vecGameObjects.reserve(100000);	// 기본 크기는 10만

	return S_OK;
}

void CObjectMgr::Priority_Tick(const _float& fTimeDelta)
{
	// BeginPlay 작동
	m_EventBeginPlay.Broadcast();
	m_EventBeginPlay.Clear();

	RegistToTick_GameObjects();

	// EndPlay로 인해 추가된 객체에 대한 BeginPlay 작동
	m_EventBeginPlay.Broadcast();
	m_EventBeginPlay.Clear();

	for (auto pObj : m_listTickObjects)
	{
		if (pObj->IsState(EGObjectState::Priority_Tick))
			pObj->Priority_Tick(fTimeDelta);
	}
}

_int CObjectMgr::Tick(const _float& fTimeDelta)
{
	for (auto pObj : m_listTickObjects)
	{
		if (pObj->IsState(EGObjectState::Tick))
			pObj->Tick(fTimeDelta);
	}

	return 0;
}

void CObjectMgr::Late_Tick(const _float& fTimeDelta)
{
	for (auto pObj : m_listTickObjects)
	{
		if (pObj->IsState(EGObjectState::Late_Tick))
			pObj->Late_Tick(fTimeDelta);
	}
}

CObjectMgr* CObjectMgr::Create(_uint iNumLevels)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("ObjectMgr Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObjectMgr::Free()
{
	// 인스턴스 해제
	for (auto pObj : m_vecGameObjects)
	{
		pObj->OnDeleted();
		Safe_Release(pObj);
	}
	m_vecGameObjects.clear();

	// 프로토타입 해제
	for (auto& Pair : m_mapPrototypes)
		Safe_Release(Pair.second);
	m_mapPrototypes.clear();
}

HRESULT CObjectMgr::Add_Prototype(const wstring& strTag, const wstring& strPrototypeKey, CGameObject* pPrototype)
{
	if (nullptr == pPrototype)
		return E_FAIL;

	if (nullptr != Find_Prototype(strPrototypeKey))
	{
		Safe_Release(pPrototype);
		return E_FAIL;
	}

	pPrototype->Add_Tag(EGObjTag::Level, strTag);
	m_mapPrototypes.emplace(strPrototypeKey, pPrototype);

	return S_OK;
}

HRESULT CObjectMgr::Add_CloneObject(const wstring& strPrototypeKey, void* pArg)
{
	/* 원형을 찾고. */
	CGameObject* pPrototype = Find_Prototype(strPrototypeKey);
	if (nullptr == pPrototype)
		return E_FAIL;

	/* 원형을 복제하여 실제 게임내에 사용할 사본 객체를 생성해낸다.  */
	CGameObject* pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	Add_GameObject(pGameObject);

	return S_OK;
}

HRESULT CObjectMgr::Add_CloneObject(FSerialData& InputData)
{
	string strPrototypeKey = "";
	if (FAILED(InputData.Get_Data("ProtoName", strPrototypeKey)))
		return E_FAIL;

	/* 원형을 찾고. */
	CGameObject* pPrototype = Find_Prototype(ConvertToWstring(strPrototypeKey));
	if (nullptr == pPrototype)
		return E_FAIL;

	/* 원형을 복제하여 실제 게임내에 사용할 사본 객체를 생성해낸다.  */
	CGameObject* pGameObject = pPrototype->Clone(InputData);
	if (nullptr == pGameObject)
		return E_FAIL;

	Add_GameObject(pGameObject);

	return S_OK;
}

CGameObject* CObjectMgr::Find_Prototype(const wstring& strPrototypeKey)
{
	auto iter = m_mapPrototypes.find(strPrototypeKey);
	if (iter == m_mapPrototypes.end())
		return nullptr;

	return (*iter).second;
}

void CObjectMgr::Clear_Prototypes(const wstring& strContainTag)
{
	// 분류별로 프로토타입을 메모리 해제하는 작업
	for (auto iter = m_mapPrototypes.begin(); iter != m_mapPrototypes.end();)
	{
		CGameObject** ppObj = &(*iter).second;
		if ((*ppObj)->Has_Tag(EGObjTag::Level, strContainTag))
		{
			// 태그 삭제후 다른 태그가 있으면 삭제하지 않는다. (다른 레벨에도 속해있음)
			(*ppObj)->Delete_Tag(EGObjTag::Level, strContainTag);
			if ((*ppObj)->Tag_Size(EGObjTag::Level) == 0U)
			{
				Safe_Release(*ppObj);
				iter = m_mapPrototypes.erase(iter);
			}
			else
				++iter;
		}
		else
			++iter;
	}
}

void CObjectMgr::Set_LevelTag(const wstring& strLevelTag)
{
	m_strLevelTag = strLevelTag;
}

HRESULT CObjectMgr::Add_GameObject(CGameObject* pObj)
{
	if (nullptr == pObj)
		return E_FAIL;

	m_vecGameObjects.push_back(pObj);
	pObj->m_iID = m_iGiveObjectID++;

	pObj->m_setTag[ECast(EGObjTag::Level)].emplace(m_strLevelTag);
	wstring& strName = pObj->m_strName;
	auto iter = m_setObjectNames.find(strName);
	// 대충 중복 이름 있으면 숫자 붙여서 넣는 방식
	if (iter != m_setObjectNames.end())
	{
		_uint i = 0U;
		while (true)
		{
			wstringstream ss;
			wstring strAdd = pObj->Get_Name();
			queue<_tchar> vecLastChar;
			_tchar lastChar;
			do
			{
				lastChar = strAdd.back();
				if (isdigit(lastChar))
				{
					vecLastChar.push(lastChar);
					strAdd.pop_back();
				}
			} while (isdigit(lastChar));

			_ulonglong lastInt = i, j = 1;
			while (vecLastChar.size())
			{
				_tchar c = vecLastChar.front();
				lastInt += (c - L'0') * j;
				vecLastChar.pop();
				j *= 10;
			}

			ss << lastInt;
			strAdd = strAdd + ss.str();

			auto iterObject = m_setObjectNames.find(strAdd);
			// 없으면 추가하기
			if (iterObject == m_setObjectNames.end())
			{
				m_setObjectNames.emplace(strAdd);
				pObj->Set_Name(strAdd);
				break;
			}
			++i;
		}
	}
	else
	{
		m_setObjectNames.emplace(strName);
	}
	pObj->OnCreated();
	m_EventBeginPlay.Add(MakeDelegate(pObj, &CGameObject::BeginPlay));

	return S_OK;
}

HRESULT CObjectMgr::Add_GameObject(const wstring& strLevelTag, CGameObject* pObj)
{
	if (nullptr == pObj)
		return E_FAIL;

	m_vecGameObjects.push_back(pObj);
	pObj->m_iID = m_iGiveObjectID++;

	// 세팅된 레벨 태그
	pObj->m_setTag[ECast(EGObjTag::Level)].emplace(m_strLevelTag);
	// 추가 레벨 태그
	pObj->m_setTag[ECast(EGObjTag::Level)].emplace(strLevelTag);

	wstring& strName = pObj->m_strName;
	auto iter = m_setObjectNames.find(strName);
	// 대충 중복 이름 있으면 숫자 붙여서 넣는 방식
	if (iter != m_setObjectNames.end())
	{
		_uint i = 0U;
		while (true)
		{
			wstringstream ss;
			wstring strAdd = pObj->Get_Name();
			queue<_tchar> vecLastChar;
			_tchar lastChar;
			do
			{
				lastChar = strAdd.back();
				if (isdigit(lastChar))
				{
					vecLastChar.push(lastChar);
					strAdd.pop_back();
				}
			} while (isdigit(lastChar));

			_ulonglong lastInt = i, j = 1;
			while (vecLastChar.size())
			{
				_tchar c = vecLastChar.front();
				lastInt += (c - L'0') * j;
				vecLastChar.pop();
				j *= 10;
			}

			ss << lastInt;
			strAdd = strAdd + ss.str();

			auto iterObject = m_setObjectNames.find(strAdd);
			// 없으면 추가하기
			if (iterObject == m_setObjectNames.end())
			{
				m_setObjectNames.emplace(strAdd);
				pObj->Set_Name(strAdd);
				break;
			}
			++i;
		}
	}
	else
	{
		m_setObjectNames.emplace(strName);
	}
	pObj->OnCreated();
	m_EventBeginPlay.Add(MakeDelegate(pObj, &CGameObject::BeginPlay));

	return S_OK;
}

CGameObject* CObjectMgr::Find_GameObjectByID(_uint iFindID)
{
	_uint iMaxIndex = Cast<_uint>(m_vecGameObjects.size() - 1);
	_uint iMinID = m_vecGameObjects.front()->Get_ID();
	_uint iMaxID = m_vecGameObjects.back()->Get_ID();

	if (iFindID > iMaxID)
		return nullptr;

	/*auto iter = find_if(m_vecGameObjects.begin(), m_vecGameObjects.end(),
		[&iFindID](const CGameObject* pObj) {
			if (pObj == nullptr)
				return false;
			return (pObj->Get_ID() == iFindID);
		});*/
	auto iter = lower_bound(m_vecGameObjects.begin(), m_vecGameObjects.end(), iFindID, 
		[](const CGameObject* pObj, _uint iID) {
			if (pObj == nullptr)
				return false;
			return (pObj->Get_ID() < iID);
		});
	if (iter == m_vecGameObjects.end())
		return nullptr;

	return (*iter);
}

CGameObject* CObjectMgr::Find_GameObjectByIndex(_uint iIndex)
{
	if (iIndex < 0 || iIndex >= m_vecGameObjects.size())
		return nullptr;

	return m_vecGameObjects[iIndex];
}

CGameObject* CObjectMgr::Find_GameObjectByName(const wstring& strName)
{
	auto iter = find_if(m_vecGameObjects.begin(), m_vecGameObjects.end(),
		[&strName](const CGameObject* pObj) {
			if (pObj == nullptr)
				return false;
			return (pObj->Get_Name() == strName);
		});
	if (iter == m_vecGameObjects.end())
		return nullptr;

	return *iter;
}

vector<CGameObject*> CObjectMgr::Get_AllGameObjectFromLevel(const wstring& strLevelTag)
{
	vector<CGameObject*> vecObjects;
	vecObjects.reserve(m_vecGameObjects.size());

	for (auto iter = m_vecGameObjects.begin(); iter != m_vecGameObjects.end(); ++iter)
	{
		if (nullptr == *iter)
			continue;

		if ((*iter)->Has_Tag(EGObjTag::Level, strLevelTag))
			vecObjects.push_back(*iter);
	}

	return vecObjects;
}

void CObjectMgr::Pause_ObjectsByLevelTag(const wstring& strLevelTag)
{
	for (auto iter = m_vecGameObjects.begin(); iter != m_vecGameObjects.end(); ++iter)
	{
		if (nullptr == *iter)
			continue;

		if ((*iter)->Has_Tag(EGObjTag::Level, strLevelTag))
			(*iter)->TurnOn_State(EGObjectState::Pause);
	}
}

void CObjectMgr::Resume_ObjectsByLevelTag(const wstring& strLevelTag)
{
	for (auto iter = m_vecGameObjects.begin(); iter != m_vecGameObjects.end(); ++iter)
	{
		if (nullptr == *iter)
			continue;

		if ((*iter)->Has_Tag(EGObjTag::Level, strLevelTag))
			(*iter)->TurnOff_State(EGObjectState::Pause);
	}
}

void CObjectMgr::Clear_GameObject(const wstring& strLevelTag)
{
	for (_uint i = 0; i < m_vecGameObjects.size(); i++)
	{
		CGameObject** ppObj = &m_vecGameObjects[i];
		if ((*ppObj)->Has_Tag(EGObjTag::Level, strLevelTag))
		{
			// 태그 삭제후 다른 태그가 있으면 삭제하지 않는다. (다른 레벨에도 속해있음)
			(*ppObj)->Delete_Tag(EGObjTag::Level, strLevelTag);
			if ((*ppObj)->Tag_Size(EGObjTag::Level) == 0U)
			{
				(*ppObj)->Set_Dead();
				auto iter = m_setObjectNames.find((*ppObj)->Get_Name());
				if (iter != m_setObjectNames.end())
					m_setObjectNames.erase(iter);
			}
		}
	}
}

void CObjectMgr::Straighten_GameObjects()
{
	// 새로운 공간을 만들고 스위칭한다.
	vector<CGameObject*> vecNewVector;
	vecNewVector.reserve(m_vecGameObjects.capacity());

	for (_uint i = 0; i < m_vecGameObjects.size(); ++i)
	{
		if (m_vecGameObjects[i] != nullptr)
		{
			vecNewVector.push_back(m_vecGameObjects[i]);
		}
	}

	m_vecGameObjects.swap(vecNewVector);
}

void CObjectMgr::RegistToTick_GameObjects()
{
	// 오브젝트 삭제, 순서 조절, 오브젝트 상태를 통한 렌더러에 추가하는 기능을 가진다.
	m_listTickObjects.clear();

	_bool bChanged = false;
	for (_uint i = 0; i < m_vecGameObjects.size(); ++i)
	{
		CGameObject* pObj = m_vecGameObjects[i];
		if (pObj->IsDead())
		{
			// 레퍼런스 카운터가 1이상이었을 때 추가적인 해제 작업이 필요하다.
			auto iter = m_setObjectNames.find(pObj->Get_Name());
			if (iter != m_setObjectNames.end())
				m_setObjectNames.erase(iter);

			pObj->EndPlay();
			pObj->OnDeleted();
			_uint iRefCount = Safe_Release(pObj);
			m_vecGameObjects[i] = nullptr;
			bChanged = true;
		}
		// 아닐시 렌더러에 추가하고 다음 요소로
		else
		{
			// 렌더 상태가 켜져있으면 조건에 따라 그룹에 추가한다.
			if (!pObj->IsState(EGObjectState::Pause) && pObj->IsState(EGObjectState::Render))
			{
				if (pObj->IsState(EGObjectState::Cull))
				{
					GI()->Transform_Frustum_ToLocalSpace(pObj->Transform().Get_TransformMatrix());
					if (GI()->IsIn_LocalPlanes(XMVector3TransformCoord(
						pObj->Transform().Get_PositionVector(),
						pObj->Transform().Get_TransformInverseMatrix()), pObj->Get_CullRadius()))
					{
						GI()->Add_RenderGroup(pObj->Get_RenderGroup(), pObj);
						if (pObj->IsState(EGObjectState::Shadow))
							GI()->Add_RenderGroup(ERenderGroup::Shadow, pObj);
						pObj->TurnOn_State(EGObjectState::Drawing);
					}
					else
						pObj->TurnOff_State(EGObjectState::Drawing);
				}
				else
				{
					GI()->Add_RenderGroup(pObj->Get_RenderGroup(), pObj);
					if (pObj->IsState(EGObjectState::Shadow))
						GI()->Add_RenderGroup(ERenderGroup::Shadow, pObj);
					pObj->TurnOn_State(EGObjectState::Drawing);
				}
			}
		}
	}

	// 삭제시 정렬
	if (bChanged)
		Straighten_GameObjects();

	// 리스트에 저장, Pause가 아닌 객체만 Tick을 작동시킴
	copy_if(m_vecGameObjects.begin(), m_vecGameObjects.end(), back_inserter(m_listTickObjects),
		[](const CGameObject* pObj) {
			return (!pObj->IsState(EGObjectState::Pause));
		});

	// Tick 우선도 기반 정렬
	_uint iIndex = ECast(EGObjTickPriority::Tick);
	m_listTickObjects.sort(
		[&iIndex](CGameObject* const pDst, CGameObject* const pSrc) {
			return (pDst->Get_Priority(iIndex) > pSrc->Get_Priority(iIndex));
		});
}
