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
