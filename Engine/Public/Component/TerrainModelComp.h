#pragma once


#include "ModelComponent.h"

#include "Component/TerrainBufferComp.h"
#include "Component/EffectComponent.h"
#include "Component/TextureComponent.h"

#include "Utility/ClassID.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainModelComp : public CModelComponent
{
	DERIVED_CLASS(CModelComponent, CTerrainModelComp)

public:
	static const _uint g_ClassID = ECast(EComponentID::TerrainModel);

public:
	enum TEXTURE { TYPE_DIFFUSE, TYPE_MASK, TYPE_BRUSH, TYPE_END };

protected:
	explicit CTerrainModelComp();
	explicit CTerrainModelComp(const CTerrainModelComp& rhs);
	virtual ~CTerrainModelComp() = default;

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
	static	CTerrainModelComp* Create();
	static	CTerrainModelComp* Create(FSerialData& InputData);
	virtual CComponent* Clone(void* Arg = nullptr) override;
	virtual CComponent* Clone(FSerialData& InputData);

protected:
	virtual void	Free() override;


public:
	HRESULT	IsRender_Ready();
	HRESULT Bind_ShaderResources();



#pragma region 터레인 버퍼 컴포넌트
public:
	HRESULT Create_Buffer(const FTerrainBufInit_HeightMap tInit);
	HRESULT Create_Buffer(const FTerrainBufInit_NoHeightMap tInit);
	// 버퍼 복사
	HRESULT Copy_VBuffer(void* pArray, size_t iSize, size_t iSizePerIndex);
	// 버퍼 업데이트
	void	Update_VBuffer(void* pData, _uint iSize);
	HRESULT Copy_IBuffer(void* pArray, size_t iSize);
	const size_t Get_VertexCount() const;
	const size_t Get_IndexCount() const;

	CTerrainBufferComp* Get_TerrainBufferComp() { return m_pTerrainBufferComp; }

private:
	CTerrainBufferComp* m_pTerrainBufferComp = { nullptr };
#pragma endregion



#pragma region 이펙트 컴포넌트
public:
	// 이펙트를 바인드 한다.
	HRESULT Bind_Effect(const wstring& strEffectKey, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements, const D3D_SHADER_MACRO* pShaderMacro = nullptr);
	// 이펙트를 언바인드 한다. 안쓸듯
	HRESULT Unbind_Effect();

private:
	CEffectComponent* m_pEffectComp = { nullptr };
#pragma endregion
	

#pragma region 텍스처 컴포넌트
public:
	HRESULT Bind_Texture(TEXTURE eType, const wstring& strFileName, const _uint iNumTextures);
	HRESULT Unbind_Texture(TEXTURE eType);


private:
	CTextureComponent* m_pTextureComps[TYPE_END] = {nullptr};
	wstring m_strDiffuseTexture = TEXT("");
	wstring m_strMaskTexture = TEXT("");
	
#pragma endregion

};

template <>
struct TComponentTrait<CTerrainModelComp::g_ClassID>
{
	using Class = CTerrainModelComp;
};

END

