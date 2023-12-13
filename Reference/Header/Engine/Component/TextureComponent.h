#pragma once

#include "Component/InternalComponent.h"


BEGIN(Engine)

/// <summary>
/// 텍스처를 저장하는 컴포넌트.
/// 텍스처를 로드할 때 텍스처 매니저가 있다면
/// 텍스처를 등록해준다.
/// 현재 텍스처를 불러오는 기능 자체는 없어 텍스처 매니저에 의존하지만 추후
/// 컴포넌트 내부에서 기능을 제작할 예정이고, 매니저에 전역적으로 등록 시키는 방식으로 갈 예정이다.
/// 
/// 내부에 등록해두고 사용되는 컴포넌트이다.
/// 외부에서 텍스처에 대한 정보를 얻고 싶다면 포함한 객체에서 함수를 정의하여 얻도록 제작한다.
/// </summary>
class ENGINE_DLL CTextureComponent : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CTextureComponent)

public:
	struct FInitTextureComp
	{
		wstring strFilePath;
	};

protected:
	explicit CTextureComponent() = default;
	explicit CTextureComponent(const CTextureComponent& rhs);
	virtual ~CTextureComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static CTextureComponent* Create();
	virtual CComponent* Clone(void* Arg = nullptr);

protected:
	virtual void	Free();


public:
	ID3D11ShaderResourceView* Get_ShaderResourseView() { return m_pSRV.Get(); }

public:
	// 텍스처를 바인드한다. 동시에 텍스처 매니저에 등록도 한다.
	HRESULT Bind_Texture(const wstring& strFilePath);
	// 이미 등록된 텍스처를 찾아 저장하는 함수, 앞으로 잘 안쓸듯? [Deprecated]
	HRESULT Bind_TextureFromManager(const wstring& strFilePath);
	// 텍스처 연결 끊기
	HRESULT Unbind_Texture();

public:
	HRESULT Bind_SRV(class CEffectComponent* pEffect, const _char* pTextureName);

private:
	// 텍스처 매니저에 연결하는 내부 함수
	HRESULT Link_TextureManager();


private:
	class CTextureMgr*					m_pTextureMgr = { nullptr };	// 전역 텍스처 리소스 관리에 쓰인다.
	ComPtr<ID3D11ShaderResourceView>	m_pSRV = { nullptr };			// 실제 렌더링 되는 텍스처의 주소이다.
};

END