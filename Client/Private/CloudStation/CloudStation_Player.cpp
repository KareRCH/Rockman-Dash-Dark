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
