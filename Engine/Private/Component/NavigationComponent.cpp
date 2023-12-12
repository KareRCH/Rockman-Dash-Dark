#include "Component/NavigationComponent.h"

#include "Component/Data/NaviCell.h"
#include "Component/EffectComponent.h"

CNavigationComponent::CNavigationComponent(const CNavigationComponent& rhs)
    : Base(rhs)
    , m_vecCells(rhs.m_vecCells)
#ifdef _DEBUG
    , m_pEffectComp(rhs.m_pEffectComp)
#endif
{
    for (auto& pCell : m_vecCells)
        Safe_AddRef(pCell);

#ifdef _DEBUG
    Safe_AddRef(m_pEffectComp);
#endif

}

HRESULT CNavigationComponent::Initialize_Prototype(const wstring& strNavigationFilePath)
{
    // 디바이스 컴포넌트 추가
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    HANDLE		hFile = CreateFile(strNavigationFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (0 == hFile)
        return E_FAIL;

    _ulong		dwByte = { 0 };

    while (true)
    {
        _float3		vPoints[3];

        if (!ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr))
            return E_FAIL;

        if (0 == dwByte)
            break;

        CNaviCell* pCell = CNaviCell::Create(vPoints);
        if (nullptr == pCell)
            return E_FAIL;

        m_vecCells.push_back(pCell);
    }

    CloseHandle(hFile);

#ifdef _DEBUG
    //m_pEffectComp = CEffectComponent::Create(TEXT("../Bin/ShaderFiles/Shader_Navigation.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
    if (nullptr == m_pEffectComp)
        return E_FAIL;
#endif

    return S_OK;
}

HRESULT CNavigationComponent::Initialize(void* Arg)
{
    return S_OK;
}

void CNavigationComponent::Priority_Tick(const _float& fTimeDelta)
{
}

void CNavigationComponent::Tick(const _float& fTimeDelta)
{
}

void CNavigationComponent::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CNavigationComponent::Render()
{
#ifdef _DEBUG
    for (auto& pCell : m_vecCells)
    {
        if (nullptr != pCell)
            pCell->Render(m_pEffectComp);
    }
#endif // _DEBUG

    return S_OK;
}

CNavigationComponent* CNavigationComponent::Create(const wstring& strNavigationFilePath)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(strNavigationFilePath)))
    {
        MSG_BOX("BoxBufferComp Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CNavigationComponent::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("BoxBufferComp Copy Failed");
        Safe_Release(pInstance);
    }

    return Cast<CComponent*>(pInstance);
}

void CNavigationComponent::Free()
{
    SUPER::Free();

    for (auto& pCell : m_vecCells)
        Safe_Release(pCell);
    m_vecCells.clear();

#ifdef _DEBUG
    Safe_Release(m_pEffectComp);
#endif
}
