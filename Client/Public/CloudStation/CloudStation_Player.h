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

public:
    static CCloudStation_Player*    Create();
private:
    virtual void		            Free();

public:
    GETSET_2(FGauge, m_fHP, HP, GET_C_REF, SET_C_REF)

private:    // ���� ����
    FGauge          m_fHP;
    FGauge          m_fSubWeapon_Main;
    FGauge          m_fSubWeapon_Usage;

    _int            m_iSubWeapon_Equipped = { -1 };
    _int            m_iHeadPart_Equipped = { -1 };
    _int            m_iBodyPart_Equipped = { -1 };
    _int            m_iFootPart_Equipped = { -1 };
    vector<_uint>   m_vecBusterPart_Equipped;

private:    // �κ��丮
    vector<_uint>       m_vecUseItems;      // ��� ������
    vector<_uint>       m_vecSomeItems;     // ��Ÿ ������
    vector<_uint>       m_vecKeyItems;      // Ű ������

    vector<_uint>       m_vecHeadParts;     // ��� ����
    vector<_uint>       m_vecBodyParts;     // �ٵ� ����
    vector<_uint>       m_vecFootParts;     // ǲ ����
    
    vector<_uint>       m_vecBusterParts;   // ������ ����

private:    // ��
    _uint       m_iMoney = { 0 };

private:    // ��ġ ����
    wstring       m_strWorld;
    wstring       m_strLocation;


};

END