#pragma once

#include "Component/ModelComponent.h"

BEGIN(Engine)


class CRectBufferComp;
class CEffectComponent;
class CTextureComponent;


/// <summary>
/// 2D용 평면 모델이 기록되는 컴포넌트
/// [기본 컴포넌트]
/// 트랜스폼, 파이프라인, 디바이스
/// </summary>
class ENGINE_DLL CPlaneModelComp final : public CModelComponent
{
	DERIVED_CLASS(CModelComponent, CPlaneModelComp)

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
	virtual HRESULT	Initialize_Prototype(void* pArg = nullptr) override;
	virtual HRESULT Initialize(void* pArg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual HRESULT	Render();

public:
	static CPlaneModelComp* Create();
	virtual CComponent*		Clone(void* pArg = nullptr);

protected:
	virtual void			Free() override;


#pragma region 내부용 컴포넌트
#pragma region 버퍼 컴포넌트
public:
	CRectBufferComp* const VIBufferComp() const { return m_pVIBufferComp; }

private:
	CRectBufferComp* m_pVIBufferComp = { nullptr };
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
	
public:
	void Set_Mode(MODE eMode) { m_eMode = eMode; }
	void Set_CurrentTextureIndex(_uint iIndex) { m_iCurrentTextureIndex = iIndex; }
	void Set_Alpha(_float fAlpha) { m_fAlpha = fAlpha; }

private:
	MODE	m_eMode = { MODE_END };
	_uint	m_iCurrentTextureIndex = { 0 };
	_float	m_fAlpha = 1.f;

};

END