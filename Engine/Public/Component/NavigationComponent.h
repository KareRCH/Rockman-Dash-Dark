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

public:
	struct TCloneDesc
	{
		_uint iCurrentInex;
	};

protected:
	explicit CNavigationComponent();
	explicit CNavigationComponent(const CNavigationComponent& rhs);
	virtual ~CNavigationComponent() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT	Initialize_Prototype(const wstring& strNavigationFilePath);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CNavigationComponent*	Create();
	static CNavigationComponent*	Create(const wstring& strNavigationFilePath);
	virtual CComponent*				Clone(void* Arg = nullptr) override;

protected:
	virtual void					Free() override;


public:
	HRESULT Load_NavigationFromFile(const wstring& strNavigationFilePath);
	HRESULT Make_Neighbors();
	_bool	IsMove(_fvector vPosition);


private:
	_int						m_iCurrentNavi = { -1 };		// ���� �� �ִ� �׺���̼� or ��� �׺���̼�

	vector<class CNaviCell*>	m_vecCells;						// �׺���̼� ��
	_int						m_iCurrentCell = { -1 };		// ���� ��
	


#ifdef _DEBUG
private:
	class CEffectComponent* m_pEffectComp = { nullptr };
	class CPipelineComp* m_pPipelineComp = { nullptr };
#endif // _DEBUG

};

END