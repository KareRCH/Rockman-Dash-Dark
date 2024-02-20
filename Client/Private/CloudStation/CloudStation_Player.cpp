#include "CloudStation/CloudStation_Player.h"

CCloudStation_Player::CCloudStation_Player()
{
}

CCloudStation_Player::CCloudStation_Player(const CCloudStation_Player& rhs)
{
}

HRESULT CCloudStation_Player::Initialize()
{
	return S_OK;
}

CCloudStation_Player* CCloudStation_Player::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("CCloudStation_Player Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCloudStation_Player::Free()
{
}

void CCloudStation_Player::Access_HP(EMode eMode, FGauge& fHP)
{
    switch (eMode)
    {
    case EMode::Download:
        fHP = m_fHP;
        break;
    case EMode::Upload:
        m_fHP = fHP;
        break;
    }
}

void CCloudStation_Player::Access_MainWeapon(EMode eMode, EMainWeapon& Value)
{
    switch (eMode)
    {
    case EMode::Download:
        Value = m_eMainWeapon;
        break;
    case EMode::Upload:
        m_eMainWeapon = Value;
        break;
    }
}

void CCloudStation_Player::Access_SubWeapon(EMode eMode, ESubWeapon& Value)
{
    switch (eMode)
    {
    case EMode::Download:
        Value = m_eSubWeapon;
        break;
    case EMode::Upload:
        m_eSubWeapon = Value;
        break;
    }
}

void CCloudStation_Player::Access_IsCanUseSubWeapons(EMode eMode, _bool& Value)
{
    switch (eMode)
    {
    case EMode::Download:
        Value = m_bIsCanUseSubWeapons;
        break;
    case EMode::Upload:
        m_bIsCanUseSubWeapons = Value;
        break;
    }
}

void CCloudStation_Player::Access_Money(EMode eMode, _uint& iMoney)
{
    switch (eMode)
    {
    case EMode::Download:
        iMoney = m_iMoney;
        break;
    case EMode::Upload:
        m_iMoney = iMoney;
        break;
    }
}

void CCloudStation_Player::Access_StartPos(EMode eMode, _float3& vStartPos)
{
    switch (eMode)
    {
    case EMode::Download:
        vStartPos = m_vStartPos;
        break;
    case EMode::Upload:
        m_vStartPos = vStartPos;
        break;
    }
}

void CCloudStation_Player::Access_StartLook(EMode eMode, _float3& vStartLook)
{
    switch (eMode)
    {
    case EMode::Download:
        vStartLook = m_vStartLook;
        break;
    case EMode::Upload:
        m_vStartLook = vStartLook;
        break;
    }
}
