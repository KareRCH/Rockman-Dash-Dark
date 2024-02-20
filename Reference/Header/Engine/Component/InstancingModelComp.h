#pragma once

#include "Component/ModelComponent.h"
#include "Component/MaterialComponent.h"

#include "Component/InstancingBufferComp.h"

BEGIN(Engine)
class CParticlePointBufferComp;
class CEffectComponent;
class CTextureComponent;


class ENGINE_DLL CInstancingModelComp : public CModelComponent
{
	DERIVED_CLASS(CModelComponent, CInstancingModelComp)

protected:
	explicit CInstancingModelComp();
	explicit CInstancingModelComp(const CInstancingModelComp& rhs);
	virtual ~CInstancingModelComp() = default;

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
	static CInstancingModelComp* Create();
	static CInstancingModelComp* Create(FSerialData& InputData);
	virtual CComponent* Clone(void* Arg = nullptr) override;
	virtual CComponent* Clone(FSerialData& InputData) override;

protected:
	virtual void	Free() override;

public:
	// ������Ÿ�� ���ۿ� �Լ�
	virtual FSerialData SerializeData_Prototype();
	// Ŭ�δ� ���� �Լ�
	virtual FSerialData SerializeData();

#pragma region ���ο� ������Ʈ
#pragma region ���� ������Ʈ
public:
	CParticlePointBufferComp* const VIBufferComp() const { return m_pVIBufferComp; }

private:
	CParticlePointBufferComp* m_pVIBufferComp = { nullptr };
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
	HRESULT Create_InstanceVIBuffers(_uint iNumInstance, CInstancingBufferComp::INSTANCING_DESC Desc);
	HRESULT Load_Texture(const wstring& strPath, _uint iNumTextures = 1);

	HRESULT Bind_TextureToEffect(const _char* pConstantName, _uint iIndex);
	HRESULT BindAndRender_Buffer(_uint iPass);

};

END