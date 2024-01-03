#pragma once

#include "Component/ModelComponent.h"

BEGIN(Engine)

class CCylinderBufferComp;
class CEffectComponent;
class CTextureComponent;

class ENGINE_DLL CCylinderModelComp : public CModelComponent
{
	DERIVED_CLASS(CModelComponent, CCylinderModelComp)

protected:
	explicit CCylinderModelComp();
	explicit CCylinderModelComp(const CCylinderModelComp& rhs);
	virtual ~CCylinderModelComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CCylinderModelComp*	Create();
	virtual CComponent*			Clone(void* Arg = nullptr) override;
protected:
	virtual void				Free() override;


#pragma region ���ο� ������Ʈ
#pragma region ���� ������Ʈ
public:
	CCylinderBufferComp* const VIBufferComp() const { return m_pVIBufferComp; }

private:
	CCylinderBufferComp* m_pVIBufferComp = { nullptr };
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

};

END