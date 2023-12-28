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


private:    // 현재 상태
    FGauge      m_fHP;
    FGauge      m_fSubWeapon_Main;
    FGauge      m_fSubWeapon_Usage;

    _int            m_iSubWeapon_Equipped = { -1 };
    _int            m_iHeadPart_Equipped = { -1 };
    _int            m_iBodyPart_Equipped = { -1 };
    _int            m_iFootPart_Equipped = { -1 };
    vector<_uint>   m_vecBusterPart_Equipped;

private:    // 인벤토리
    vector<_uint>       m_vecUseItems;      // 사용 아이템
    vector<_uint>       m_vecSomeItems;     // 기타 아이템
    vector<_uint>       m_vecKeyItems;      // 키 아이템

    vector<_uint>       m_vecHeadParts;     // 헤드 파츠
    vector<_uint>       m_vecBodyParts;     // 바디 파츠
    vector<_uint>       m_vecFootParts;     // 풋 파츠
    
    vector<_uint>       m_vecBusterParts;   // 버스터 파츠

private:    // 돈
    _uint       m_iMoney = { 0 };

private:    // 위치 정보
    wstring       m_strWorld;
    wstring       m_strLocation;


};

END