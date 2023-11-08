#pragma once

#include "Base.h"

BEGIN(Engine)

class FShaderData final
{
	THIS_CLASS(FShaderData)
private:
	FShaderData() = default;
public:
	~FShaderData() = default;

public:
	HRESULT Initialize()
	{
		return S_OK;
	}

public:
	static FShaderData* Create()
	{
		ThisClass* pInstance = new ThisClass();

		if (FAILED(pInstance->Initialize()))
		{
			Engine::Safe_Delete(pInstance);

			MSG_BOX("RenderMgr Create Failed");

			return nullptr;
		}

		return pInstance;
	}

	void Free() 
	{
		delete this;
	}
	void Set_Shader(const wstring& _strFileName, ComPtr<ID3DBlob> shader)
	{
		if (shader == nullptr)
			return;

		strFileName = _strFileName;
		bLoaded = true;
		pBlob = shader;
	}

public:
	ID3DBlob*			Get_Shader() const { return pBlob.Get(); }
	const _bool			IsLoaded() const { return bLoaded; }

private:
	wstring					strFileName = L"";			// 로드할 셰이더 코드, 파일 링크용
	_bool					bLoaded = false;			// 로드됨
	ComPtr<ID3DBlob>		pBlob = { nullptr };		// 셰이더 데이터
};


/// <summary>
/// 셰이더 코드를 저장하는 클래스
/// </summary>
class CShaderMgr final : public CBase
{
	DERIVED_CLASS(CBase, CShaderMgr)
private:
	explicit CShaderMgr(const DX11DEVICE_T tDevice);
	explicit CShaderMgr(const CShaderMgr& rhs) = delete;
	virtual ~CShaderMgr() = default;

public:
	HRESULT				Initialize(const wstring& strMainPath);

public:
	static CShaderMgr*	Create(const DX11DEVICE_T tDevice, const wstring& strMainPath);

private:
	virtual void		Free() override;

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pDeviceContext = { nullptr };


public:
	// 파일이름을 통해 셰이더를 로드
	HRESULT	Load_Shader(const wstring& strFileName, const wstring& strKey);
	// 로드된 셰이더가 있다면 그 값을 반환한다.
	ID3DBlob* Get_Shader(const wstring& strName) const;

private:
	ID3DBlob* Read_ShaderBinary(const wstring& strFile);
	

private:
	wstring								m_strMainPath;
	_unmap<wstring, FShaderData*>		m_mapShaderData;
};

END