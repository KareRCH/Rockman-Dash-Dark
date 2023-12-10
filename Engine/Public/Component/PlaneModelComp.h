#pragma once

#include "Component/SceneComponent.h"
#include "Component/TextureComponent.h"

BEGIN(Engine)

/// <summary>
/// 2D용 평면 모델이 기록되는 컴포넌트
/// [기본 컴포넌트]
/// 트랜스폼, 파이프라인, 디바이스
/// </summary>
class CPlaneModelComp final : public CSceneComponent
{
	DERIVED_CLASS(CSceneComponent, CPlaneModelComp)

protected:
	explicit CPlaneModelComp() = default;
	explicit CPlaneModelComp(const CPlaneModelComp& rhs);
	virtual ~CPlaneModelComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual HRESULT	Render();

public:
	static CPlaneModelComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;


#pragma region 텍스처 컴포넌트
public:


private:
	CTextureComponent* m_pTextureComp = { nullptr };
#pragma endregion


};

END