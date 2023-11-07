#include "System/ObjectMgr.h"

CObjectMgr::CObjectMgr()
{
}

HRESULT CObjectMgr::Initialize()
{
    return S_OK;
}

void CObjectMgr::PriorityTick()
{
}

_int CObjectMgr::Tick(const _float& fTimeDelta)
{
    return 0;
}

void CObjectMgr::LateTick()
{
}

CObjectMgr* CObjectMgr::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("PhysicsMgr Create Failed");
		Engine::Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CObjectMgr::Free()
{
}
