#pragma once

#include "ModelComponent.h"

#include "Component/TerrainBufferComp.h"
#include "Component/EffectComponent.h"
#include "Component/TextureComponent.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainModelComp : public CModelComponent
{
	DERIVED_CLASS(CModelComponent, CTerrainModelComp)

public:
	enum TEXTURE { TYPE_DIFFUSE, TYPE_MASK, TYPE_BRUSH, TYPE_END };

protected:
	explicit CTerrainModelComp() = default;
	explicit CTerrainModelComp(const CTerrainModelComp& rhs);
	virtual ~CTerrainModelComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CTerrainModelComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;

public:
	HRESULT	IsRender_Ready();
	HRESULT Bind_ShaderResources();



#pragma region 터레인 버퍼 컴포넌트
	HRESULT Create_Buffer(const FTerrainBufInit_HeightMap tInit);
	HRESULT Create_Buffer(const FTerrainBufInit_NoHeightMap tInit);
	// 버퍼 복사
	HRESULT Copy_VBuffer(void* pArray, size_t iSize, size_t iSizePerIndex);
	// 버퍼 업데이트
	void	Update_VBuffer(void* pData, _uint iSize);
	const size_t Get_VertexCount() const;

private:
	CTerrainBufferComp* m_pTerrainBufferComp = { nullptr };
#pragma endregion



#pragma region 이펙트 컴포넌트
public:
	// 이펙트를 바인드 한다.
	HRESULT Bind_Effect(const wstring& strEffectKey, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	// 이펙트를 언바인드 한다. 안쓸듯
	HRESULT Unbind_Effect();

private:
	CEffectComponent* m_pEffectComp = { nullptr };
#pragma endregion


#pragma region 텍스처 컴포넌트
public:
	HRESULT Bind_Texture(TEXTURE eType, const wstring& strFileName);
	HRESULT Unbind_Texture(TEXTURE eType);


private:
	CTextureComponent* m_pTextureComps[TYPE_END] = {nullptr};
#pragma endregion


};

END