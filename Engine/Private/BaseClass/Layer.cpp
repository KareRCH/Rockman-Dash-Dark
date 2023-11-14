#include "BaseClass/Layer.h"

#include "Component/PrimitiveComponent.h"
#include "System/GameInstance.h"
#include "System/RenderMgr.h"

CLayer::CLayer()
	: m_fPriority(0.f)
{

}

CLayer* CLayer::Create(_float fPriority)
{
	CLayer* pLayer = new CLayer;

	if (FAILED(pLayer->Initialize(fPriority)))
	{
		Safe_Release(pLayer);

		MSG_BOX("Layer Create Failed");
		return nullptr;
	}

	return pLayer;
}

void CLayer::Free()
{
	// 컨테이너 해제 작업
	for (auto iter = m_mapObject.begin(); iter != m_mapObject.end();)
	{
		Safe_Release((*iter).second);
		iter = m_mapObject.erase(iter);
	}
}

HRESULT CLayer::Initialize(_float fPriority)
{
	m_fPriority = fPriority;

	return S_OK;
}

void CLayer::Priority_Tick(const _float& fTimeDelta)
{
	// Set Dead로 설정된 객체를 삭제시킨다.
	for (auto iter = m_mapObject.begin(); iter != m_mapObject.end();)
	{
		if ((*iter).second->IsDead())
		{
			// 레퍼런스 카운터가 1이상이었을 때 추가적인 해제 작업이 필요하다.
			_uint iRefCount = Safe_Release((*iter).second);
			iter = m_mapObject.erase(iter);
		}
		// 아닐시 렌더러에 추가하고 다음 iter로
		else
		{
			// 렌더 상태가 켜져있으면 조건에 따라 그룹에 추가한다.
			CGameObject* pObj = (*iter).second;
			if (pObj->IsState(EGObjectState::Render))
			{
				if (pObj->IsState(EGObjectState::RenderZBuffer))
				{
					if (pObj->IsState(EGObjectState::RenderDeferred))
					{
						GameInstance()->Add_RenderGroup(ERenderGroup::NonAlpha, pObj);
					}
					else
					{
						GameInstance()->Add_RenderGroup(ERenderGroup::Alpha, pObj);
					}
				}
				// ZBuffer가 필요없는 그룹
				else
				{
					if (pObj->IsState(EGObjectState::RenderPriority))
					{
						GameInstance()->Add_RenderGroup(ERenderGroup::Priority, pObj);
					}
					else if (pObj->IsState(EGObjectState::RenderPostProcess))
					{
						GameInstance()->Add_RenderGroup(ERenderGroup::PostProcess, pObj);
					}
					else
					{
						GameInstance()->Add_RenderGroup(ERenderGroup::UI, pObj);
					}
				}
			}

			++iter;
		}
	}

	// 우선도 설정을 해준다.
	for (auto& item : m_mapObject)
		m_listPriorityObject.push_back(item.second);

	// Tick 우선도 기반 정렬
	_uint iIndex = Cast_EnumDef(EGObjTickPriority::Tick);

	m_listPriorityObject.sort(
		[&iIndex](CGameObject* const pDst, CGameObject* const pSrc) {
			return (pDst->Get_Priority(iIndex) > pSrc->Get_Priority(iIndex));
		});

	// 선행 Tick 실행
	for (auto& gameObj : m_listPriorityObject)
	{
		gameObj->Priority_Tick(fTimeDelta);
	}
}

_int CLayer::Tick(const _float& fTimeDelta)
{
	// 이 레이어에 속한 게임 오브젝트를 업데이트 한다.
	_int iResult = 0;
	for (auto& gameObj : m_listPriorityObject)
	{
		iResult = gameObj->Tick(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CLayer::Late_Tick(const _float& fTimeDelta)
{
	// Late Tick 실행
	for (auto& gameObj : m_listPriorityObject)
	{
		gameObj->Late_Tick(fTimeDelta);
	}

	// 쓰고 나면 클리어
	m_listPriorityObject.clear();
}

CPrimitiveComponent* CLayer::Get_Component(COMPONENTID eID, const wstring& pObjTag, const wstring& pComponentTag)
{
	// 레이어 -> 오브젝트 -> 컴포넌트
	//auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));
	auto iter = m_mapObject.find(pObjTag);

	if (iter == m_mapObject.end())
		return nullptr;

	return nullptr;// (*iter).second->Get_Component(eID, pComponentTag);
}

// 인게임에서 추가할 때 (Update 타이밍때)
HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	// 이 레이어에 게임 오브젝트를 적재한다.
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	wstring strObjectTag = pGameObject->Get_Name();
	auto iter = m_mapObject.find(strObjectTag.c_str());
	// 대충 중복 이름 있으면 숫자 붙여서 넣는 방식
	if (iter != m_mapObject.end())
	{
		// 마지막 숫자 떼어내기
		_uint i = 0U;
		_tchar cLast = strObjectTag.back();
		_uint iMul = 1U;

		// 숫자면 1로 시작
		if (iswdigit(cLast))
			i = 1U;

		while (iswdigit(cLast))
		{
			i += _wtoi(&cLast) * iMul;
			iMul *= 10U;
			strObjectTag.pop_back();
			cLast = strObjectTag.back();
		}

		// 정해진 숫자로 추가한다.
		wstringstream ss;
		ss << i;
		wstring strNew = strObjectTag + ss.str();
		auto iterObject = m_mapObject.find(strNew);
		// 없으면 추가하기
		if (iterObject == m_mapObject.end())
		{
			m_mapObject.emplace(strNew, pGameObject);
			pGameObject->Set_Name(strNew);
		}
	}
	else
		m_mapObject.emplace(strObjectTag.c_str(), pGameObject);

	return S_OK;
}

// 씬 레디 전용
HRESULT CLayer::Add_GameObject(const wstring& pObjTag, CGameObject* pGameObject)
{
	// 이 레이어에 게임 오브젝트를 적재한다.
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	auto iter = m_mapObject.find(pObjTag);
	// 대충 중복 이름 있으면 숫자 붙여서 넣는 방식
	if (iter != m_mapObject.end())
	{
		_uint i = 0U;
		while (true)
		{
			wstringstream ss;
			ss << i;
			wstring strNew = pObjTag + ss.str();
			auto iterObject = m_mapObject.find(strNew);
			// 없으면 추가하기
			if (iterObject == m_mapObject.end())
			{
				m_mapObject.emplace(strNew, pGameObject);
				pGameObject->Set_Name(strNew);
				break;
			}

			++i;
		}
		
	}
	else
	{
		m_mapObject.emplace(pObjTag, pGameObject);
		pGameObject->Set_Name(pObjTag);
	}
		

	return S_OK;
}

CGameObject* CLayer::Get_GameObject(const wstring& pObjTag)
{
	auto iter = m_mapObject.find(pObjTag);
	if (iter != m_mapObject.end())
		return (*iter).second;

	return nullptr;
}
