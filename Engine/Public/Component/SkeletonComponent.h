#pragma once

#include "InternalComponent.h"

BEGIN(Engine)

enum class EModelGroupIndex : _uint;

/// <summary>
/// ���̷����� ������ �� �ִ� ������Ʈ
/// �ϳ��� �ν��Ͻ�ȭ �� ���̷��� �׷��� ������.
/// �� ������Ʈ�� �޽� ������Ʈ�� ��ǰ���� ��޵ȴ�.
/// </summary>
class ENGINE_DLL CSkeletonComponent final : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CSkeletonComponent)
	struct FInitialData
	{
		wstring strArmatureKey;
	};
protected:
	explicit CSkeletonComponent();
	explicit CSkeletonComponent(const CSkeletonComponent& rhs);
	virtual ~CSkeletonComponent() = default;

public:
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	
public:
	static CSkeletonComponent* Create();
	virtual CComponent* Clone(void* Arg = nullptr);

protected:
	virtual void	Free();

public:
	HRESULT Load_BoneRootNode(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const wstring& strArmatureKey);

private:	// �� ����
	class FArmatureData* m_pBoneRootNode = { nullptr };	// ���̽��� �Ǵ� ��Ʈ �뵵�� ��
};

END