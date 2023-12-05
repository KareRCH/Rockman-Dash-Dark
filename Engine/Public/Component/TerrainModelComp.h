#pragma once

#include "ModelComponent.h"

#include "Component/TerrainBufferComp.h"
#include "Component/EffectComponent.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainModelComp : public CModelComponent
{
	DERIVED_CLASS(CModelComponent, CTerrainModelComp)

protected:
	explicit CTerrainModelComp() = default;
	explicit CTerrainModelComp(const CTerrainModelComp& rhs);
	virtual ~CTerrainModelComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual _int	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual void	Render() override;

public:
	static CTerrainModelComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;

public:
	HRESULT Bind_ShaderResources();

#pragma region �ͷ��� ���� ������Ʈ
	HRESULT Create_Buffer(const FTerrainBufInit tInit);

private:
	CTerrainBufferComp* m_pTerrainBufferComp = { nullptr };
#pragma endregion



#pragma region ����Ʈ ������Ʈ
public:
	// ����Ʈ�� ���ε� �Ѵ�.
	HRESULT Bind_Effect(const wstring& strEffectKey);
	// ����Ʈ�� ����ε� �Ѵ�. �Ⱦ���
	HRESULT Unbind_Effect();

private:
	CEffectComponent* m_pEffectComp = { nullptr };
#pragma endregion

};

END