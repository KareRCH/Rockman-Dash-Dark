#pragma once

#include "Component/ModelComponent.h"
#include "Utility/ClassID.h"

BEGIN(Engine)

class CBoxBufferComp;
class CEffectComponent;
class CTextureComponent;

/// <summary>
/// 박스의 모델에 대한 정보가 담긴 컴포넌트.
/// 버퍼, 이펙트, 텍스처 정보를 가지고 있다.
/// </summary>
class ENGINE_DLL CBoxModelComp final : public CModelComponent
{
	DERIVED_CLASS(CModelComponent, CBoxModelComp)

public:
	static const _uint g_ClassID = ECast(EComponentID::BoxModel);

protected:
	explicit CBoxModelComp();
	explicit CBoxModelComp(const CBoxModelComp& rhs);
	virtual ~CBoxModelComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize_Prototype(FSerialData& InputData);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(FSerialData& InputData);
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	// 프로토타입 제작용 함수
	virtual FSerialData SerializeData_Prototype();
	// 클로닝 전용 함수
	virtual FSerialData SerializeData();

public:
	static	CBoxModelComp*	Create();
	static	CBoxModelComp*	Create(FSerialData& InputData);
	virtual CComponent*		Clone(void* Arg = nullptr) override;
	virtual CComponent*		Clone(FSerialData& InputData);

protected:
	virtual void			Free() override;

#pragma region 내부용 컴포넌트
#pragma region 버퍼 컴포넌트
public:
	CBoxBufferComp* const VIBufferComp() const { return m_pVIBufferComp; }

private:
	CBoxBufferComp* m_pVIBufferComp = { nullptr };
#pragma endregion


#pragma region 이펙트 컴포넌트
public:
	CEffectComponent* const EffectComp() const { return m_pEffectComp; }

private:
	CEffectComponent* m_pEffectComp = { nullptr };
#pragma endregion


#pragma region 텍스처 컴포넌트
public:
	CTextureComponent* const TextureComp() const { return m_pTextureComp; }

private:
	CTextureComponent* m_pTextureComp = { nullptr };
#pragma endregion
#pragma endregion

public:
	HRESULT Bind_ShaderResources();

};

template <>
struct TComponentTrait<CBoxModelComp::g_ClassID>
{
	using Class = CBoxModelComp;
};

END