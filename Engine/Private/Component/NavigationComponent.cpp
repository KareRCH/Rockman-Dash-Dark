#include "Component/NavigationComponent.h"

#include "Component/Data/NaviCell.h"
#include "Component/EffectComponent.h"
#include "Component/PipelineComp.h"

CNavigationComponent::CNavigationComponent()
{
#ifdef _DEBUG
    NULL_CHECK(m_pEffectComp = CEffectComponent::Create());
    NULL_CHECK(m_pPipelineComp = Cast<CPipelineComp*>(m_pGI->Reference_PrototypeComp(L"CamViewComp")));
#endif

}

CNavigationComponent::CNavigationComponent(const CNavigationComponent& rhs)
    : Base(rhs)
    , m_vecCells(rhs.m_vecCells)
#ifdef _DEBUG
    , m_pEffectComp(rhs.m_pEffectComp)
    , m_pPipelineComp(rhs.m_pPipelineComp)
#endif
{
    for (auto& pCell : m_vecCells)
        Safe_AddRef(pCell);

#ifdef _DEBUG
    Safe_AddRef(m_pEffectComp);
    Safe_AddRef(m_pPipelineComp);
#endif

}

HRESULT CNavigationComponent::Initialize_Prototype(void* Arg)
{
#ifdef _DEBUG
    m_pEffectComp->Bind_Effect(TEXT("RunTime/FX_Navigation.hlsl"), SHADER_VTX_SINGLE::Elements, SHADER_VTX_SINGLE::iNumElements);
#endif
    return S_OK;
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

        CNaviCell* pCell = CNaviCell::Create(vPoints, Cast<_uint>(m_vecCells.size()));
        if (nullptr == pCell)
            return E_FAIL;

        m_vecCells.push_back(pCell);
    }

    CloseHandle(hFile);

    if (FAILED(Make_Neighbors()))
        return E_FAIL;

#ifdef _DEBUG
    m_pEffectComp->Bind_Effect(TEXT("RunTime/FX_Navigation.hlsl"), SHADER_VTX_SINGLE::Elements, SHADER_VTX_SINGLE::iNumElements);

#endif

    return S_OK;
}

HRESULT CNavigationComponent::Initialize_Prototype(FSerialData& InputData)
{
    if (FAILED(__super::Initialize_Prototype(InputData)))
        return E_FAIL;


#ifdef _DEBUG
    m_pEffectComp->Bind_Effect(TEXT("RunTime/FX_Navigation.hlsl"), SHADER_VTX_SINGLE::Elements, SHADER_VTX_SINGLE::iNumElements);
#endif

    return S_OK;
}

HRESULT CNavigationComponent::Initialize(void* Arg)
{
    if (Arg != nullptr)
        m_iCurrentCell = ReCast<TCloneDesc*>(Arg)->iCurrentInex;

    return S_OK;
}

HRESULT CNavigationComponent::Initialize(FSerialData& InputData)
{
    if (FAILED(__super::Initialize(InputData)))
        return E_FAIL;

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
            pCell->Render(m_pEffectComp, m_pPipelineComp);
    }
#endif // _DEBUG

    return S_OK;
}

FSerialData CNavigationComponent::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ComponentID", g_ClassID);

    return Data;
}

FSerialData CNavigationComponent::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    Data.Add_Member("ComponentID", g_ClassID);

    return Data;
}

CNavigationComponent* CNavigationComponent::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("NavigationComponent Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CNavigationComponent* CNavigationComponent::Create(const wstring& strNavigationFilePath)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(strNavigationFilePath)))
    {
        MSG_BOX("NavigationComponent Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CNavigationComponent* CNavigationComponent::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
    {
        MSG_BOX("NavigationComponent Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CNavigationComponent::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(Arg)))
    {
        MSG_BOX("NavigationComponent Copy Failed");
        Safe_Release(pInstance);
    }

    return Cast<CComponent*>(pInstance);
}

CComponent* CNavigationComponent::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
    {
        MSG_BOX("NavigationComponent Copy Failed");
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
    Safe_Release(m_pPipelineComp);
#endif
}

HRESULT CNavigationComponent::Load_NavigationFromFile(const wstring& strNavigationFilePath)
{
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

        CNaviCell* pCell = CNaviCell::Create(vPoints, Cast<_uint>(m_vecCells.size()));
        if (nullptr == pCell)
            return E_FAIL;

        m_vecCells.push_back(pCell);
    }

    CloseHandle(hFile);

    if (FAILED(Make_Neighbors()))
        return E_FAIL;

    return S_OK;
}

HRESULT CNavigationComponent::Make_Neighbors()
{
    for (auto& pSourCell : m_vecCells)
    {
        for (auto& pDestCell : m_vecCells)
        {
            if (pSourCell == pDestCell)
                continue;

            if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CNaviCell::POINT_A), pSourCell->Get_Point(CNaviCell::POINT_B)))
            {
                pSourCell->SetUp_Neighbor(CNaviCell::LINE_AB, pDestCell);
            }
            if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CNaviCell::POINT_B), pSourCell->Get_Point(CNaviCell::POINT_C)))
            {
                pSourCell->SetUp_Neighbor(CNaviCell::LINE_BC, pDestCell);
            }
            if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CNaviCell::POINT_C), pSourCell->Get_Point(CNaviCell::POINT_A)))
            {
                pSourCell->SetUp_Neighbor(CNaviCell::LINE_CA, pDestCell);
            }

        }
    }

    return S_OK;
}

_bool CNavigationComponent::IsMove(_fvector vPosition)
{
    _int		iNeighborIndex = { -1 };

    if (true == m_vecCells[m_iCurrentCell]->IsIn(vPosition, XMMatrixIdentity(), &iNeighborIndex))
        return true;

    else
    {
        if (-1 != iNeighborIndex)
        {
            while (true)
            {
                if (-1 == iNeighborIndex)
                    return false;
                if (true == m_vecCells[iNeighborIndex]->IsIn(vPosition, XMMatrixIdentity(), &iNeighborIndex))
                {
                    m_iCurrentCell = iNeighborIndex;
                    return true;
                }
            }
        }
        else
            return false;
    }
}
