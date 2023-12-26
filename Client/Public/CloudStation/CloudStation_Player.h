#pragma once

#include "Client_Define.h"
#include "BaseClass/CloudStation.h"

#include "Utility/LogicDeviceBasic.h"

BEGIN(Client)

/// <summary>
/// �÷��̾� ���� Ŭ���� �����̼�
/// ������ ������ �̸� ���� �������� ������ ����� �����մϴ�.
/// </summary>
class CCloudStation_Player : public CCloudStation
{
    DERIVED_CLASS(CCloudStation, CCloudStation_Player)

protected:
    explicit CCloudStation_Player();
    explicit CCloudStation_Player(const CCloudStation_Player& rhs);
    virtual ~CCloudStation_Player() = default;

protected:
    virtual HRESULT     Initialize();

private:
    static CCloudStation_Player*    Create();
    virtual void		            Free();


private:    // ����
    FGauge      m_fHP;
    FGauge      m_fSubWeapon_Main;
    FGauge      m_fSubWeapon_Usage;

private:    // �κ��丮
    vector<_uint>       m_vecKeyItems;

    vector<_uint>       m_vecHeadParts;
    vector<_uint>       m_vecBodyParts;
    vector<_uint>       m_vecFootParts;
    
    vector<_uint>       m_vecBusterParts;
};

END