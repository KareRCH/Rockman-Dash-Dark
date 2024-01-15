#include "Component/ModelComponent.h"

CModelComponent::CModelComponent()
{
	// 기본 0번 패스는 활성화 시켜 놓는다.
	Set_ActivePass(0);
}

CModelComponent::CModelComponent(const CModelComponent& rhs)
	: Base(rhs)
{
}

HRESULT CModelComponent::Initialize_Prototype(void* Arg)
{
	if (FAILED(__super::Initialize_Prototype(Arg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModelComponent::Initialize_Prototype(FSerialData& InputData)
{
	if (FAILED(__super::Initialize_Prototype(InputData)))
		return E_FAIL;

	Reset_ActivePass();

	_uint iNumActivePasses = InputData.Get_ArraySize("ActivePasses");
	for (_uint i = 0; i < iNumActivePasses; i++)
	{
		FSerialData PassData;
		InputData.Get_ObjectFromArray("ActivePasses", i, PassData);

		_uint iPassNum;
		if (FAILED(PassData.Get_Data("PassNum", iPassNum)))
			return E_FAIL;

		Set_ActivePass(iPassNum);
	}

	return S_OK;
}

HRESULT CModelComponent::Initialize(void* Arg)
{
	if (FAILED(__super::Initialize(Arg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModelComponent::Initialize(FSerialData& InputData)
{
	if (FAILED(__super::Initialize_Prototype(InputData)))
		return E_FAIL;

	Reset_ActivePass();

	_uint iNumActivePasses = InputData.Get_ArraySize("ActivePasses");
	for (_uint i = 0; i < iNumActivePasses; i++)
	{
		FSerialData PassData;
		InputData.Get_ObjectFromArray("ActivePasses", i, PassData);

		_uint iPassNum;
		if (FAILED(PassData.Get_Data("PassNum", iPassNum)))
			return E_FAIL;

		Set_ActivePass(iPassNum);
	}

	return S_OK;
}

void CModelComponent::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);
}

void CModelComponent::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);
}

void CModelComponent::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);
}

void CModelComponent::Free()
{
	SUPER::Free();


}

FSerialData CModelComponent::SerializeData_Prototype()
{
	FSerialData Data = SUPER::SerializeData_Prototype();

	for (_uint i = 0; i < m_vecActivePasses.size(); i++)
	{
		FSerialData ActivePassData;
		ActivePassData.Add_Member("PassNum", m_vecActivePasses[i]);

		Data.Pushback_Member("ActivePasses", ActivePassData);
	}

	return Data;
}

FSerialData CModelComponent::SerializeData()
{
	FSerialData Data = SUPER::SerializeData();

	for (_uint i = 0; i < m_vecActivePasses.size(); i++)
	{
		FSerialData ActivePassData;
		ActivePassData.Add_Member("PassNum", m_vecActivePasses[i]);

		Data.Pushback_Member("ActivePasses", ActivePassData);
	}

	return Data;
}

void CModelComponent::Set_ActivePass(_uint iPass)
{
	if (iPass < 0 || iPass >= UINT_MAX)
		return;

	// 동일한 패스는 못 그리게 막아 놓는다.
	auto iter = find_if(m_vecActivePasses.begin(), m_vecActivePasses.end(),
		[&iPass](const _uint& iMemPass) {
			return (iPass == iMemPass);
		});
	if (iter != m_vecActivePasses.end())
		return;

	m_vecActivePasses.reserve(++m_iNumActivePasses);
	m_vecActivePasses.push_back(iPass);
}

void CModelComponent::Unset_ActivePass(_uint iPass)
{
	if (iPass < 0 || iPass >= UINT_MAX)
		return;

	auto iter = find_if(m_vecActivePasses.begin(), m_vecActivePasses.end(),
		[&iPass](const _uint& iMemPass) {
			return (iPass == iMemPass);
		});
	if (iter == m_vecActivePasses.end())
		return;

	m_vecActivePasses.erase(iter);
	--m_iNumActivePasses;
}

void CModelComponent::Reset_ActivePass()
{
	m_iNumActivePasses = 0;
	m_vecActivePasses.clear();
}


