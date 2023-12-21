#include "ModelLoad.h"

CModelLoad::CModelLoad()
{
}

HRESULT CModelLoad::Initialize()
{
    return S_OK;
}

CModelLoad* CModelLoad::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("TestObject Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CModelLoad::Free()
{
}
