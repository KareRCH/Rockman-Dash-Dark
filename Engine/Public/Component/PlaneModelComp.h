#pragma once

#include "Component/SceneComponent.h"
#include "Component/TextureComponent.h"

BEGIN(Engine)

/// <summary>
/// 2D�� ��� ���� ��ϵǴ� ������Ʈ
/// [�⺻ ������Ʈ]
/// Ʈ������, ����������, ����̽�
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


#pragma region �ؽ�ó ������Ʈ
public:


private:
	CTextureComponent* m_pTextureComp = { nullptr };
#pragma endregion


};

END