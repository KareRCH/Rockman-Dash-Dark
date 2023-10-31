#include "GameObject/TestObject.h"

CTestObject::CTestObject(ID3D11Device* pGraphicDev)
    : Base(pGraphicDev)
{
}

CTestObject::CTestObject(const CGameObject& rhs)
    : Base(rhs)
{
}

HRESULT CTestObject::Initialize()
{
    return S_OK;
}

_int CTestObject::Tick(const _float& fTimeDelta)
{
    return 0;
}

void CTestObject::LateTick()
{
}

void CTestObject::Render(ID3D11DeviceContext* pDeviceContext)
{
}

void CTestObject::Free()
{
}
