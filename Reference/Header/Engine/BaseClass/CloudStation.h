#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// ������ ���� �߻�Ŭ�����Դϴ�.
/// �������� �����ϴ� Ŭ�����̸�, ������Ʈ�� �ܺο��� �Ͼ��.
/// ������ ��ü�� �������� ������ ������ ������ ���� ������
/// ������ �����͸� ��û���� ���� ����� �ǽ��ϵ��� �Ѵ�.
/// BlackBoard�� �׻� FBlackBoardPtr�� ���ؼ� ������ ���� ����Ѵ�.
/// �� �ܿ� �� �����带 ��ӹ޾� ����� �������� ������ ��� �ǵ� ��� ����.
/// STL�� ���� ��ܳ־ �׷��� �����ϵ��� ���� ����
/// ���� ����� �����־ ����.
/// Ȱ���� ����ڿ��� �ñ��.
/// </summary>
class ENGINE_DLL CCloudStation abstract : public CBase
{
protected:
    explicit CCloudStation();
    explicit CCloudStation(const CCloudStation& rhs);
    virtual ~CCloudStation() = default;

private:
    virtual void		Free() PURE;

protected:
    virtual HRESULT     Ready_BlackBoard() PURE;

public:
    void    Set_Expired() { m_bExpired = true; }
    _bool   IsExpired() { return m_bExpired; }

private:
    _bool   m_bExpired;             // ������ ����� ��� ���������͵��� ������ ���� �� �ְ� ���ش�.
    
};

END