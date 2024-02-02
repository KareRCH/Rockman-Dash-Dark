#pragma once

#include "Client_Define.h"
#include "BaseClass/CloudStation.h"
#include "Utility/LogicDeviceBasic.h"

BEGIN(Client)

class CCloudStation_Boss : public CCloudStation
{
    DERIVED_CLASS(CCloudStation, CCloudStation_Boss)

protected:
    explicit CCloudStation_Boss();
    explicit CCloudStation_Boss(const CCloudStation_Boss& rhs);
    virtual ~CCloudStation_Boss() = default;

protected:
    virtual HRESULT             Initialize();

public:
    static CCloudStation_Boss*  Create();
private:
    virtual void		        Free();

public:
    void Access_HP(EMode eMode, FGauge& fHP);

private:    // 현재 상태
    FGauge          m_fHP;

};

END