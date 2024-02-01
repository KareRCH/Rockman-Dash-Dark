#include "CloudStation/CloudStation_Boss.h"

CCloudStation_Boss::CCloudStation_Boss()
{
}

CCloudStation_Boss::CCloudStation_Boss(const CCloudStation_Boss& rhs)
{
}

HRESULT CCloudStation_Boss::Initialize()
{
	return S_OK;
}

CCloudStation_Boss* CCloudStation_Boss::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CCloudStation_Player Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCloudStation_Boss::Free()
{
}

void CCloudStation_Boss::Access_HP(EMode eMode, FGauge& fHP)
{
	switch (eMode)
	{
	case EMode::Upload:
		m_fHP = fHP;
		break;
	case EMode::Download:
		fHP = m_fHP;
		break;
	}
}
