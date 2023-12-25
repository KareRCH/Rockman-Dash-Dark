#include "Component/D3D11DeviceComp.h"

#include "System/GameInstance.h"

CD3D11DeviceComp::CD3D11DeviceComp(const CD3D11DeviceComp& rhs)
	: Base(rhs)
	, m_pDevice(rhs.m_pDevice), m_pContext(rhs.m_pContext)
{
}

HRESULT CD3D11DeviceComp::Initialize_Prototype(void* Arg)
{
	m_pDevice = GI()->Get_GraphicDev();
	m_pContext = GI()->Get_GraphicContext();

	return S_OK;
}

HRESULT CD3D11DeviceComp::Initialize(void* Arg)
{
	return S_OK;
}

CD3D11DeviceComp* CD3D11DeviceComp::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("D3D11DeviceComp Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CD3D11DeviceComp::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(Arg)))
	{
		MSG_BOX("D3D11DeviceComp Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CComponent*>(pInstance);
}

void CD3D11DeviceComp::Free()
{
	SUPER::Free();
}