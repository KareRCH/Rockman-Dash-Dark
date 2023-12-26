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
    DERIVED_CLASS(CBase, CCloudStation)

protected:
    explicit CCloudStation();
    explicit CCloudStation(const CCloudStation& rhs);
    virtual ~CCloudStation() = default;

protected:
    virtual HRESULT     Initialize() PURE;

private:
    virtual void		Free() PURE;

public:
    void    Set_Expired() { m_bExpired = true; }
    _bool   IsExpired() const { return m_bExpired; }

private:
    _bool   m_bExpired = false;             // Ŭ���� �����̼� ����� ��� ���������͵��� ������ ���� �� �ְ� ���ش�.
    
};

END