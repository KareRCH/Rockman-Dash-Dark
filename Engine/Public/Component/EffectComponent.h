#pragma once

#include "InternalComponent.h"

#include "Component/D3D11DeviceComp.h"

BEGIN(Engine)

/// <summary>
/// 간편한 셰이더를 사용하기 위해 따로 만들어낸 FX11을 사용하는 셰이더 컴포넌트.
/// 성능용은 ShaderComponent이며.
/// 이 EffectComponent 계열은 고수준 셰이딩을 자주 사용하고 간편하게 사용해야 할 때 쓰인다.
/// 간편함을 위해 만들어 져서 이 컴포넌트 하나로 해결본다.
/// 
/// [바인딩 된 컴포넌트]
/// - 디바이스
/// [참조하는 관리자]
/// - 셰이더 매니저
/// </summary>
class ENGINE_DLL CEffectComponent final : public CInternalComponent, public ID3D11DeviceComp
{
	DERIVED_CLASS(CInternalComponent, CEffectComponent)

protected:
	explicit CEffectComponent();
	explicit CEffectComponent(const CEffectComponent& rhs);
	virtual ~CEffectComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	// 로드시 이미 만들어진 셰이더를 활용한다.
	// 셰이더 코드는 이미 컴파일 되어 만들어져 있다는 가정하에 로드한다.
	static CEffectComponent* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free();

#pragma region 디바이스 컴포넌트
protected:
	virtual ID3D11Device* const D3D11Device() const override { return m_pDeviceComp->Get_Device(); }
	virtual ID3D11DeviceContext* const D3D11Context() const override { return m_pDeviceComp->Get_Context(); }

private:
	CD3D11DeviceComp* m_pDeviceComp = { nullptr };
#pragma endregion

	
public:	// 외부용 함수
	// 이펙트 불러오기
	HRESULT Bind_Effect(const wstring& strEffectFileName, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements, const D3D_SHADER_MACRO* pShaderMacro = nullptr);
	// 이펙트 풀기
	HRESULT Unbind_Effect();
	// 외부에서 
	HRESULT	IsRender_Ready();

public:
	// 패스 선택
	HRESULT Begin(_uint iPassIndex);
	// 행렬 바인딩
	HRESULT Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix);
	HRESULT Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrix, _uint iNumMatrices);
	// 텍스처 바인딩
	HRESULT Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV);
	HRESULT Bind_SRVs(const _char* pConstantName, ID3D11ShaderResourceView** ppSRV, _uint iNumTextures);
	// 일반 변수 바인딩
	HRESULT Bind_RawValue(const _char* pConstantName, const void* pData, _uint iSize);

private:
	// 내부적으로 쓰이는 셰이더 매니저 링크 함수. 이를 통해 전역적으로 셰이더를 등록하고 해제하는 행위를 한다.
	HRESULT Link_ShaderMgr();

private:
	class CShaderMgr* m_pShaderMgr = { nullptr };	// 셰이더 매니저 연결
	class FEffectData* m_pEffectData = { nullptr };	// 이펙트 데이터
};

END