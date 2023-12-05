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

#pragma region 터레인 버퍼 컴포넌트
	HRESULT Create_Buffer(const FTerrainBufInit tInit);

private:
	CTerrainBufferComp* m_pTerrainBufferComp = { nullptr };
#pragma endregion



#pragma region 이펙트 컴포넌트
public:
	// 이펙트를 바인드 한다.
	HRESULT Bind_Effect(const wstring& strEffectKey);
	// 이펙트를 언바인드 한다. 안쓸듯
	HRESULT Unbind_Effect();

private:
	CEffectComponent* m_pEffectComp = { nullptr };
#pragma endregion

};

END