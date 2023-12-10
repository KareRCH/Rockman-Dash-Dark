#pragma once

#include "Component/ModelComponent.h"

#include "Component/MultiMeshBufComp.h"
#include "Component/SkinnedModelShaderComp.h"
#include "Component/SkeletalComponent.h"
#include "Component/AnimationComponent.h"
#include "Component/EffectComponent.h"


BEGIN(Engine)

class FModelData;

/// <summary>
/// �޽��� ���� �ѷ��δ� ������ ���� ����ϴ� ������Ʈ
/// </summary>
class ENGINE_DLL CSkinnedModelComp final : public CModelComponent, public ISkeletalComponent, public IAnimationComponent
{
	DERIVED_CLASS(CModelComponent, CSkinnedModelComp)

protected:
	explicit CSkinnedModelComp() = default;
	explicit CSkinnedModelComp(const CSkinnedModelComp& rhs);
	virtual ~CSkinnedModelComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CSkinnedModelComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;




#pragma region �� ������ ���ε�
public:	// �ܺ� ������
	// ���� ���ε��Ѵ�. �޽�, ���̷�Ż, �ִϸ��̼� ��ο��� ����ȴ�.
	HRESULT Bind_Model(EModelGroupIndex eGroup, const wstring& strModelKey);

private:
	FModelData* m_pModelData = { nullptr };
#pragma endregion






#pragma region ��Ƽ �޽� ����
	// �ܺ� ������
public:
	// �ε�� �𵨿� ���� ����� �޽ø� ���ε� �Ѵ�.
	HRESULT Bind_Mesh(const wstring& strMeshKey);
	// ���ε� �� �޽��� �ʿ���� �޽ø� ����.
	HRESULT Unbind_Mesh(const wstring& strMeshKey);
	// ���ε� �� �޽õ��� ��� ����ε� �Ѵ�.
	void	Unbind_AllMeshes();

private:
	CMultiMeshBufComp* m_pMultiMeshBufComp = { nullptr };
#pragma endregion





#pragma region ���̷�Ż ������Ʈ
public:		// �ܺ� ������
	// ���� ����Ѵ�. ���̷�Ż�� �ִϸ��̼ǿ� ���δ�.
	HRESULT Bind_Skeletal(const wstring& strSkeletalKey);

public:
	virtual CSkeletalComponent* SkeletalComp() override { return m_pSkeletalComp; }

private:
	CSkeletalComponent* m_pSkeletalComp = { nullptr };
#pragma endregion





#pragma region �ִϸ��̼� ������Ʈ
	// �ܺ� ������
public:
	// ����ũ ������ ���ε� �Ѵ�.
	HRESULT Create_Mask(const wstring& strMaskName, const wstring& strSkeletalName, _bool bInitBoneActive);
	// ����ũ ���� Ȱ��ȭ �Ѵ�.
	void Deactive_BoneMask(_uint iIndex, const wstring& strBoneName);
	// ����ũ ���� Ȱ��ȭ �Ѵ�.
	void Active_BoneMask(_uint iIndex, const wstring& strBoneName);
	// ����ũ�� �ִϸ��̼��� �����Ѵ�.
	void Set_MaskAnimation(_uint iIndex, const wstring& strAnimName);
	// ����ũ�� �ִϸ��̼� �ð��� �����Ѵ�.
	void Set_MaskTime(_uint iIndex, _float fTime);
	// �ִϸ��̼��� ������ �Ǵ� �ð���ȭ���� �����Ѵ�. Tick���� 
	void Set_TickDeltaTime(_float fDeltaTime);
	// �ִϸ��̼��� ���� ������ ����� ���� �����Ѵ�. 
	HRESULT Apply_Pose();

public:
	virtual CAnimationComponent* AnimComp() override { return m_pAnimationComp; }

private:
	CAnimationComponent* m_pAnimationComp = { nullptr };
#pragma endregion






#pragma region ���� �Բ� ���̵� �Ǵ� ���̴�
	// �ܺ� ������
public:
	// ����Ʈ�� ���ε� �Ѵ�.
	HRESULT Bind_Effect(const wstring& strEffectKey, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	// ����Ʈ�� ����ε� �Ѵ�. �Ⱦ���
	HRESULT Unbind_Effect();


private:
	CEffectComponent* m_pEffectComp = { nullptr };
#pragma endregion


};

END