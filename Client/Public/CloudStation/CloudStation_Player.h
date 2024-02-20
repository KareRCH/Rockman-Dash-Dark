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

public:
    static CCloudStation_Player*    Create();
private:
    virtual void		            Free();

public:
    GETSET_2(FGauge, m_fHP, HP, GET_C_REF, SET_C_REF)
    void Access_HP(EMode eMode, FGauge& fHP);
    void Access_MainWeapon(EMode eMode, EMainWeapon& Value);
    void Access_SubWeapon(EMode eMode, ESubWeapon& Value);
    void Access_IsCanUseSubWeapons(EMode eMode, _bool& Value);

private:    // 현재 상태
    FGauge          m_fHP = FGauge(100.f, true);
    FGauge          m_fSubWeapon_Main;
    FGauge          m_fSubWeapon_Usage;

    _bool			m_bIsCanUseSubWeapons = { false };
    EMainWeapon     m_eMainWeapon = { EMainWeapon::None };
    ESubWeapon      m_eSubWeapon = { ESubWeapon::ThrowArm };

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

public:
    void Access_Money(EMode eMode, _uint& iMoney);

private:    // 돈
    _uint       m_iMoney = { 0 };

private:    // 위치 정보
    wstring       m_strWorld;
    wstring       m_strLocation;

public:
    void Access_StartPos(EMode eMode, _float3& vStartPos);
    void Access_StartLook(EMode eMode, _float3& vStartLook);

private:
    _float3 m_vStartPos = { 55.f, 0.f, 20.f };      // 맵 옮겨갈 때 위치
    _float3 m_vStartLook = {};                      // 맵 옮겨갈 때 바라보는 방향

};

END