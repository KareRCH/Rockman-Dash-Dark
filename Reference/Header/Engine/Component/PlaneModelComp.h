#pragma once

#include "Component/ModelComponent.h"
#include "Utility/ClassID.h"

BEGIN(Engine)


class CRectBufferComp;
class CEffectComponent;
class CTextureComponent;


/// <summary>
/// 2D�� ��� ���� ��ϵǴ� ������Ʈ
/// [�⺻ ������Ʈ]
/// Ʈ������, ����������, ����̽�
/// </summary>
class ENGINE_DLL CPlaneModelComp final : public CModelComponent
{
	DERIVED_CLASS(CModelComponent, CPlaneModelComp)

public:
	static const _uint g_ClassID = ECast(EComponentID::PlaneModel);

public:
	enum MODE { ORTHO, PERSP, MODE_END };

public:
	struct TPlaneModelCloneDesc
	{
		MODE eMode;
	};

protected:
	explicit CPlaneModelComp();
	explicit CPlaneModelComp(const CPlaneModelComp& rhs);
	virtual ~CPlaneModelComp() = default;

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
	// ������Ÿ�� ���ۿ� �Լ�
	virtual FSerialData SerializeData_Prototype();
	// Ŭ�δ� ���� �Լ�
	virtual FSerialData SerializeData();

public:
	static	CPlaneModelComp* Create();
	static	CPlaneModelComp* Create(FSerialData& InputData);
	virtual CComponent* Clone(void* Arg = nullptr) override;
	virtual CComponent* Clone(FSerialData& InputData);

protected:
	virtual void			Free() override;


#pragma region ���ο� ������Ʈ
#pragma region ���� ������Ʈ
public:
	CRectBufferComp* const VIBufferComp() const { return m_pVIBufferComp; }

private:
	CRectBufferComp* m_pVIBufferComp = { nullptr };
#pragma endregion


#pragma region ����Ʈ ������Ʈ
public:
	CEffectComponent* const EffectComp() const { return m_pEffectComp; }

private:
	CEffectComponent* m_pEffectComp = { nullptr };
#pragma endregion


#pragma region �ؽ�ó ������Ʈ
public:
	CTextureComponent* const TextureComp() const { return m_pTextureComp; }

private:
	CTextureComponent* m_pTextureComp = { nullptr };
#pragma endregion
#pragma endregion


public:
	HRESULT Bind_ShaderResources();
	
public:
	void Set_Mode(MODE eMode) { m_eMode = eMode; }
	void Set_CurrentTextureIndex(_uint iIndex) { m_iCurrentTextureIndex = iIndex; }
	void Set_Alpha(_float fAlpha) { m_fAlpha = fAlpha; }

	void Set_MinUV(_float2 vMinUV) { m_vMinUV = vMinUV; }
	void Set_MaxUV(_float2 vMaxUV) { m_vMaxUV = vMaxUV; }

private:
	MODE	m_eMode = { MODE_END };
	_uint	m_iCurrentTextureIndex = { 0 };
	_float	m_fAlpha = 1.f;
	_float2 m_vMinUV = {};
	_float2 m_vMaxUV = { 1.f, 1.f };

};

template <>
struct TComponentTrait<CPlaneModelComp::g_ClassID>
{
	using Class = CPlaneModelComp;
};

END