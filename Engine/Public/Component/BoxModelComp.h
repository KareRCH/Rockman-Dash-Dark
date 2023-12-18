#pragma once

#include "SceneComponent.h"





BEGIN(Engine)

class CBoxBufferComp;
class CEffectComponent;
class CTextureComponent;


/// <summary>
/// �ڽ��� �𵨿� ���� ������ ��� ������Ʈ.
/// ����, ����Ʈ, �ؽ�ó ������ ������ �ִ�.
/// </summary>
class ENGINE_DLL CBoxModelComp final : public CSceneComponent
{
	DERIVED_CLASS(CSceneComponent, CBoxModelComp)

protected:
	explicit CBoxModelComp();
	explicit CBoxModelComp(const CBoxModelComp& rhs);
	virtual ~CBoxModelComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CBoxModelComp*	Create();
	virtual CComponent*		Clone(void* Arg = nullptr) override;
protected:
	virtual void			Free() override;


#pragma region ���ο� ������Ʈ
#pragma region ���� ������Ʈ
public:
	CBoxBufferComp* const VIBufferComp() const { return m_pVIBufferComp; }

private:
	CBoxBufferComp* m_pVIBufferComp = { nullptr };
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

private:
	_uint		m_iPass = { 0 };		// ���̴� �н�


};

END