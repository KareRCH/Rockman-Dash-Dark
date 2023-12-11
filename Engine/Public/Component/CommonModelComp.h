#pragma once

#include "ModelComponent.h"

BEGIN(Engine)


/// <summary>
/// �п� �����ӿ�ũ�� ���缭 ���۵� ���� �� ������Ʈ
/// �̸� ������� ���� �𵨷ε� �Ŵ��� ������ �� ����
/// 
/// �� �Ŵ����� ��ŵ� ������ ���·� ���۵ȴ�.
/// �� ������Ʈ���� �� �ε� ����� ����.
/// �̴� ������Ÿ�����ε� ���� ���� �� �� �ְ� �ϱ� ������.
/// </summary>
class ENGINE_DLL CCommonModelComp final : public CModelComponent
{
	DERIVED_CLASS(CModelComponent, CCommonModelComp)

public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

protected:
	explicit CCommonModelComp() = default;
	explicit CCommonModelComp(const CCommonModelComp& rhs);
	virtual ~CCommonModelComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual HRESULT	Render();

public:
	static CCommonModelComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;


private:
	// �����͵��� ��� ������Ʈ ���·� ������. ������ �ٸ� �κ�
	TYPE	m_eModelType = { TYPE_END };


private:	// �޽� ����
	_uint m_iNumMeshes = { 0 };
	vector<class CMeshComponent*>	m_Meshes;


public:		// ���͸��� ����
	_uint	m_iNumMaterials = { 0 };
	vector<class CMaterialComponent*>	m_pMaterials;


private:	// �� ����
	// �� �ϳ��� ������ ����� ������Ʈ ������, �𸮾� �� ���Դ�.
	_uint	m_iNumSkeletals = { 0 };
	vector<class CSkeletalComponent*>	m_Skeletals;


private:	// �ִϸ��̼� ����
	_uint	m_iNumAnimations = { 0 };
	_uint	m_iCurrentAnimIndex = { 0 };
	vector<class CAnimationComponent*>	m_pAnimations;

};

END