#pragma once


#include "GameObject.h"


BEGIN(Engine)

/// <summary>
/// �׺���̼��� �ͷ��ο� �ͼӽ�Ű�� �ʰ� ������ �����ϱ� ���� ������ Ŭ����
/// �׺���̼��� ���� ��ġ�� �̿��ϴ� �׺���̼��� ������.
/// ��ü������ ���� ����� �׺���̼� ������Ʈ�� ���ϰ� ������.
/// �� Ŭ������ �׺���̼ǰ��� ������ ������ ����� ������.
/// </summary>
class ENGINE_DLL CNavigation : public CGameObject
{
	DERIVED_CLASS(CGameObject, CNavigation)

protected:
	explicit CNavigation();
	explicit CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Prototype(const _float3 vPos);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(const _float3 vPos);
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CNavigation* Create();
	static CNavigation* Create(const _float3 vPos);
	CGameObject*		Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;

public:
	// ����ȭ ������ �̾Ƴ� �� ����ϴ� �Լ�. �� ������Ʈ ���� �̾Ƴ��� �����Ͱ� �ٸ��ϴ�.
	virtual FSerialData SerializeData();

private:
	HRESULT	Initialize_Component();

private:
	class CNavigationComponent* m_pNaviComp = { nullptr };
};

END