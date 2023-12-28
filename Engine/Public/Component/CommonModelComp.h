#pragma once

#include "ModelComponent.h"
#include "Component/AnimationComponent.h"

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
	explicit CCommonModelComp();
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

public:
	HRESULT Render_AnimModel();
	HRESULT Render_NoAnimModel();

public:
	HRESULT Link_ToModelMgr();
	HRESULT Bind_Model(TYPE eType, EModelGroupIndex eGroupIndex, const wstring& strModelFilePath);
	void Add_MaskTime(_uint iIndex, _float fAddTrackPos);
	void Apply_Pos();
	void Set_Animation(_uint iAnimIndex, _float fSpeedMultiply, _bool bIsLoop, _bool bReverse = false);
	void Add_AnimTime(const _float& fTimeDelta);
	void Invalidate_Animation();
	void Invalidate_BoneTransforms();


private:
	// �����͵��� ��� ������Ʈ ���·� ������. ������ �ٸ� �κ�
	TYPE	m_eModelType = { TYPE_END };


private:		// �޽� ����
	_uint								m_iNumMeshes = { 0 };
	vector<class CMeshComp*>			m_pMeshComps;



private:		// ���͸��� ����
	_uint								m_iNumMaterials = { 0 };
	vector<class CMaterialComponent*>	m_pMaterialComps;



private:		// �� ����
	// �� �ϳ��� ������ ����� ������Ʈ ������, �𸮾� �� ���Դ�.
	class CSkeletalComponent*			m_pSkeletalComp = { nullptr };


public:
	CAnimationComponent* AnimationComp() { return m_pAnimationComp; }

private:		// �ִϸ��̼� ����
	CAnimationComponent*			m_pAnimationComp = { nullptr };


#pragma region ���� �Բ� ���̵� �Ǵ� ���̴�
	// �ܺ� ������
public:
	// ����Ʈ�� ���ε� �Ѵ�.
	HRESULT Bind_Effect(const wstring& strEffectKey, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	// ����Ʈ�� ����ε� �Ѵ�. �Ⱦ���
	HRESULT Unbind_Effect();


private:
	class CEffectComponent* m_pEffectComp = { nullptr };
#pragma endregion

};

END