#pragma once

#include "InternalComponent.h"

#include "Component/Interface/ISkeletalComponent.h"

BEGIN(Engine)

enum class EModelGroupIndex : _uint;

/// <summary>
/// ���̷����� ������ �� �ִ� ������Ʈ
/// �ϳ��� �ν��Ͻ�ȭ �� ���̷��� �׷��� ������.
/// �� ������Ʈ�� �޽� ������Ʈ�� ��ǰ���� ��޵ȴ�.
/// </summary>
class ENGINE_DLL CSkeletalComponent final : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CSkeletalComponent)
	struct FInitData
	{
		EModelGroupIndex eGroupIndex;
		wstring strGroupKey;
		wstring strArmatureKey;
	};
protected:
	explicit CSkeletalComponent() = default;
	explicit CSkeletalComponent(const CSkeletalComponent& rhs);
	virtual ~CSkeletalComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	
public:
	static CSkeletalComponent* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;

public:
	HRESULT Load_BoneRootNode(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey, const wstring& strArmatureKey);

private:	// �� ����
	class FArmatureData* m_pArmature = { nullptr };
	
	// �� ������ ���� ������ �����ؾ��Ѵ�.
	// �� ������ ���� ������ �ϱ� ���� ������ ���� �˻� ������ �����ؾ��Ѵ�.
	// ���� �� �׷��� �Ǵ� Armature�� �̸����� ã���� ����, ID�� ���� ã�� �� �־�� �Ѵ�.
	// ���� �ѹ� ������� ���� �߰��� �� ���ٴ� �����Ͽ� ������ ¥������.
	// Ȥ�ó� ���� ������ �ٸ� ������Ʈ�� �ٴ´ٴ� �����Ͽ� ��Ÿġ ��ɵ� �� �� �ְ� ����� ���´�.
	// �̶� ���� ��� �ð��� ������ �� �� �ֵ��� ID�� �����ϵ��� �����Ѵ�.
	// ���� �⺻������ ���������� ������ ������
	
};

END