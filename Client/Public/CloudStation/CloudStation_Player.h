#pragma once

#include "Client_Define.h"
#include "BaseClass/CloudStation.h"

#include "Utility/LogicDeviceBasic.h"

BEGIN(Client)

/// <summary>
/// 플레이어 전용 클라우드 스테이션
/// 레벨을 오가도 이를 통해 영구적인 데이터 통신이 가능합니다.
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


private:    // 범용
    FGauge      m_fHP;
    FGauge      m_fSubWeapon_Main;
    FGauge      m_fSubWeapon_Usage;

private:    // 인벤토리
    vector<_uint>       m_vecKeyItems;

    vector<_uint>       m_vecHeadParts;
    vector<_uint>       m_vecBodyParts;
    vector<_uint>       m_vecFootParts;
    
    vector<_uint>       m_vecBusterParts;
};

END