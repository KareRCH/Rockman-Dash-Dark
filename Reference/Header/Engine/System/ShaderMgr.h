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
	wstring					strFileName = L"";			// �ε��� ���̴� �ڵ�, ���� ��ũ��
	_bool					bLoaded = false;			// �ε��
	ComPtr<ID3DBlob>		pBlob = { nullptr };		// ���̴� ������
};


/// <summary>
/// ���̴� �ڵ带 �����ϴ� Ŭ����
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
	// �����̸��� ���� ���̴��� �ε�
	HRESULT	Load_Shader(const wstring& strFileName, const wstring& strKey);
	// �ε�� ���̴��� �ִٸ� �� ���� ��ȯ�Ѵ�.
	ID3DBlob* Get_Shader(const wstring& strName) const;

private:
	ID3DBlob* Read_ShaderBinary(const wstring& strFile);
	

private:
	wstring								m_strMainPath;
	_unmap<wstring, FShaderData*>		m_mapShaderData;
};

END