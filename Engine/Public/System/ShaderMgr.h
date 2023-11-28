#pragma once

#include "Base.h"
#include "System/ShaderMgr_Enum.h"

BEGIN(Engine)

class FShaderData final
{
	THIS_CLASS(FShaderData)
private:
	FShaderData() = default;
public:
	~FShaderData()
	{
		int t = 0;
	}

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
	void Set_Shader(const wstring& _strFileName, ID3DBlob* _pShaderByte, ID3D11DeviceChild* _pShaderBuffer)
	{
		if (_pShaderByte == nullptr || _pShaderBuffer == nullptr)
			return;

		strFileName = _strFileName;
		bLoaded = true;
		pShaderByte = _pShaderByte;
		pShaderBuffer = _pShaderBuffer;
	}

public:
	const ComPtr<ID3DBlob>&				Get_ShaderByte() const { return pShaderByte; }
	const ComPtr<ID3D11DeviceChild>&	Get_ShaderBuffer() const { return pShaderBuffer; }
	const _bool							IsLoaded() const { return bLoaded; }

private:
	wstring							strFileName = L"";				// �ε��� ���̴� �ڵ�, ���� ��ũ��
	_bool							bLoaded = false;				// �ε��
	ComPtr<ID3DBlob>				pShaderByte = { nullptr };		// ���̴� ����Ʈ �ڵ�
	ComPtr<ID3D11DeviceChild>		pShaderBuffer = { nullptr };	// ���̴� ����
};

class FEffectData final : public CBase
{
	DERIVED_CLASS(CBase, FEffectData)

private:
	FEffectData() = default;
	~FEffectData() = default;

public:
	HRESULT Initialize() { return S_OK; }

public:
	static FEffectData* Create()
	{
		ThisClass* pInstance = new ThisClass();

		if (FAILED(pInstance->Initialize()))
		{
			MSG_BOX("RenderMgr Create Failed");
			Safe_Delete(pInstance);

			return nullptr;
		}

		return pInstance;
	}

public:
	virtual void Free() {}

public:
	D3DX11_TECHNIQUE_DESC				tTechniqueDesc;
	ComPtr<ID3DX11Effect>				pEffect;
	vector<ComPtr<ID3D11InputLayout>>	pInputLayouts;
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
	ComPtr<ID3D11Device>			m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>		m_pDeviceContext = { nullptr };


public:
	// �����̸��� ���� ���̴��� �ε�
	HRESULT	Load_Shader(const wstring& strFileName, const EShaderType eType, const wstring& strKey);
	// �ε�� ���̴��� �ִٸ� �� ���� ��ȯ�Ѵ�.
	const ComPtr<ID3DBlob> Get_ShaderByte(const EShaderType eType, const wstring& strName) const;
	template <EShaderType Type>
	ComPtr<ShaderType<Type>> Get_ShaderBuffer(const wstring& strName) const;

private:
	ID3DBlob* Read_ShaderBinary(const wstring& strFile);

private:
	wstring								m_strMainPath;
	_unmap<wstring, FShaderData*>		m_mapShaderData[ECast(EShaderType::Size)];



public:
	HRESULT Load_Effect(const wstring& strFileName, const wstring& strKey);
	ID3DX11Effect* Find_Effect(const wstring& strKey) const;

#pragma region Effect11 ����
private:
	_unmap<wstring, FEffectData*>	m_mapEffects;		// Effect11�� ������� ���̴� �����.
#pragma endregion

};


#pragma region Get Shader ���ø�

template <EShaderType Type>
ComPtr<ShaderType<Type>> CShaderMgr::Get_ShaderBuffer(const wstring& strName) const
{
	constexpr _uint iIndex = ECast(Type);

	auto iter = m_mapShaderData[iIndex].find(strName);
	if (iter == m_mapShaderData[iIndex].end() || !(*iter).second->IsLoaded())
		return nullptr;

	ID3D11DeviceChild* pShaderBuffer = (*iter).second->Get_ShaderBuffer().Get();

	return ComPtr<ShaderType<Type>>(Cast<ShaderType<Type>*>(pShaderBuffer));
}

#pragma endregion



END