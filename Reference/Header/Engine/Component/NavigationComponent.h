#pragma once


#include "Component/PrimitiveComponent.h"


BEGIN(Engine)


/// <summary>
/// �׺���̼ǿ� �����Ͽ� �̵��� �� �ֵ��� �ϴ� Ŭ����
/// �׺���̼� ���� �����ϴ� ���, Ž�� ����� �����Ѵ�.
/// ���ڴ� �׺���̼��� �����ϴ� ��ü. Navigation Ŭ�����̸�
/// ���ڴ� �׺���̼��� ����ϴ� ��ü. ���ӿ�����Ʈ�鿡 �ش��Ѵ�.
/// </summary>
class ENGINE_DLL CNavigationComponent final : public CPrimitiveComponent
{
	DERIVED_CLASS(CPrimitiveComponent, CNavigationComponent)
protected:
	explicit CNavigationComponent() = default;
	explicit CNavigationComponent(const CNavigationComponent& rhs);
	virtual ~CNavigationComponent() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg) override { return S_OK; }
	virtual HRESULT	Initialize_Prototype(const wstring& strNavigationFilePath);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CNavigationComponent*	Create(const wstring& strNavigationFilePath);
	virtual CComponent*				Clone(void* Arg = nullptr) override;

protected:
	virtual void					Free() override;


private:
	_int						m_iCurrentNavi = { -1 };		// ���� �� �ִ� �׺���̼� or ��� �׺���̼�

	vector<class CNaviCell*>	m_vecCells;						// �׺���̼� ��
	_int						m_iCurrentCell = { -1 };		// ���� ��
	


#ifdef _DEBUG
private:
	class CEffectComponent* m_pEffectComp = { nullptr };
#endif // _DEBUG

};

END