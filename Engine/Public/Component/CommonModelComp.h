#pragma once

#include "ModelComponent.h"
#include "Component/AnimationComponent.h"
#include "Utility/ClassID.h"

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
	static const _uint g_ClassID = ECast(EComponentID::CommonModel);

public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

protected:
	explicit CCommonModelComp();
	explicit CCommonModelComp(const CCommonModelComp& rhs);
	virtual ~CCommonModelComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize_Prototype(FSerialData& InputData);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(FSerialData& InputData);
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual HRESULT	Render();

public:
	static CCommonModelComp* Create();
	static CCommonModelComp* Create(FSerialData& InputData);
	virtual CComponent* Clone(void* Arg = nullptr);
	virtual CComponent* Clone(FSerialData& InputData);

protected:
	virtual void	Free() override;

public:
	// ������Ÿ�� ���ۿ� �Լ�
	virtual FSerialData SerializeData_Prototype();
	// Ŭ�δ� ���� �Լ�
	virtual FSerialData SerializeData();

public:
	HRESULT Render_AnimModel();
	HRESULT Render_NoAnimModel();

public:
	HRESULT Link_ToModelMgr();
	HRESULT Bind_Model(TYPE eType, EModelGroupIndex eGroupIndex, const wstring& strModelFilePath);
	HRESULT Bind_Model(const wstring& strModelFilePath) { return Bind_Model(m_eModelType, EModelGroupIndex::Permanent, strModelFilePath); }
	void Add_MaskTime(_uint iIndex, _float fAddTrackPos);
	void Apply_Pos();
	void Set_Animation(_uint iAnimIndex, _float fSpeedMultiply, _bool bIsLoop, _bool bReverse = false, _float fTransitionSpeed = 0.1f);
	void Set_AnimationMaintain(_uint iAnimIndex, _float fSpeedMultiply, _bool bIsLoop, _bool bReverse = false, _float fTransitionSpeed = 0.1f);
	void Add_AnimTime(const _float& fTimeDelta);
	void Invalidate_Animation();
	void Invalidate_BoneTransforms();


private:
	// �����͵��� ��� ������Ʈ ���·� ������. ������ �ٸ� �κ�
	TYPE	m_eModelType = { TYPE_END };
	wstring m_strModelPath = TEXT("");		// ������Ÿ�Կ� ����
	wstring m_strEffectPath = TEXT("");		// ������Ÿ�Կ� ����


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

template <>
struct TComponentTrait<CCommonModelComp::g_ClassID>
{
	using Class = CCommonModelComp;
};

END