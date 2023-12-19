#pragma once

#include "InternalComponent.h"

#include "Component/Interface/ISkeletalComponent.h"

BEGIN(Engine)

enum class EModelGroupIndex : _uint;
class FBoneGroup;

/// <summary>
/// ���̷����� ������ �� �ִ� ������Ʈ
/// �ϳ��� �ν��Ͻ�ȭ �� ���̷��� �׷��� ������.
/// �� ������Ʈ�� �޽� ������Ʈ�� ��ǰ���� ��޵ȴ�.
/// ���� �����Ͽ� ����Ѵ�.
/// </summary>
class ENGINE_DLL CSkeletalComponent final : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CSkeletalComponent)
	
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
	// ������ �� �����͸� �߰��Ѵ�.
	void		Set_ModelData(class FModelData* pModelData);
	// �Ƹ��߾ �ε��Ѵ�.
	HRESULT		Load_Skeletal(const wstring& strSkeletalKey);

	// ���� Ʈ�������� �ּҸ� ������ ���͸� ������, ���ۿ� ���� �� �� �ֵ��� ���ش�.
	FModelData* Get_ModelData();
	FBoneGroup* Get_BoneGroup();
	void		Invalidate_BoneTransforms();

private:	// �� ����
	class FModelData* m_pModelData = { nullptr };		// �� ������, �ش� ���� ���ε� ���ѳ��� ���� �س��´�.
	class FBoneGroup* m_pBoneDatas = { nullptr };		// �Ƹ��߾�, �� �༮�� ���� ���������� ����ȭ�� �迭�� ������ �ִ�.
	
};

END