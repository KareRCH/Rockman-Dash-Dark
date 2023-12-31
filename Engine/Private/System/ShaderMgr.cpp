#include "System/ShaderMgr.h"

CShaderMgr::CShaderMgr(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext)
{
}

HRESULT CShaderMgr::Initialize(const wstring& strMainPath)
{
	m_strMainPath = strMainPath;

	return S_OK;
}

CShaderMgr* CShaderMgr::Create(const DX11DEVICE_T tDevice, const wstring& strMainPath)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize(strMainPath)))
	{
		MSG_BOX("RenderMgr Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CShaderMgr::Free()
{
	// 셰이더 코드 해제
	for (size_t i = 0; i < ECast(EShaderType::Size); i++)
	{
		for (auto& Pair : m_mapShaderData[i])
			Pair.second->Free();
		m_mapShaderData[i].clear();
	}
	
	for (auto& Pair : m_mapEffects)
		Safe_Release(Pair.second);
	m_mapEffects.clear();
}

HRESULT CShaderMgr::Load_Shader(const wstring& strFileName, const EShaderType eType, const wstring& strKey)
{
	_uint iIndex = ECast(eType);
	if (eType == EShaderType::Size)
		return E_FAIL;

	FShaderData* pData = nullptr;
	auto iter = m_mapShaderData[iIndex].find(strKey);
	if (iter != m_mapShaderData[iIndex].end())
	{
		if ((*iter).second->IsLoaded())
			return E_FAIL;

		// 코드를 로드하지 않고 키에 들어간 셰이더가 있다면 지금 로드하도록 한다.
		pData = (*iter).second;
		if (nullptr == pData)
			return E_FAIL;
	}
	else
	{
		pData = FShaderData::Create();
		if (nullptr == pData)
			return E_FAIL;

		m_mapShaderData[iIndex].emplace(strKey, pData);
	}

	// 여기서 셰이더 컴파일 혹은 컴파일된 셰이더 코드를 가지고 Blob에 저장한다.
	ComPtr<ID3DBlob> pBlob = Read_ShaderBinary(m_strMainPath + strFileName);
	if (nullptr == pBlob)
		return E_FAIL;


	ComPtr<ID3D11DeviceChild> pShaderBuffer = { nullptr };
	switch (eType)
	{
	case Engine::EShaderType::Vertex:
	{
		ID3D11VertexShader* pShader = Cast<ID3D11VertexShader*>(pShaderBuffer.Get());
		ComPtr<ID3D11VertexShader> pComShader = pShader;
		if (FAILED(m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pComShader)))
			return E_FAIL;
		pShaderBuffer = Cast<ID3D11DeviceChild*>(pComShader.Get());
		break;
	}
	case Engine::EShaderType::Pixel:
	{
		ID3D11PixelShader* pShader = Cast<ID3D11PixelShader*>(pShaderBuffer.Get());
		ComPtr<ID3D11PixelShader> pComShader = pShader;
		if (FAILED(m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pComShader)))
			return E_FAIL;
		pShaderBuffer = Cast<ID3D11DeviceChild*>(pShader);
		break;
	}
	case Engine::EShaderType::Geometry:
	{
		ID3D11GeometryShader* pShader = Cast<ID3D11GeometryShader*>(pShaderBuffer.Get());
		if (FAILED(m_pDevice->CreateGeometryShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pShader)))
			return E_FAIL;
		pShaderBuffer = Cast<ID3D11DeviceChild*>(pShader);
		break;
	}
	case Engine::EShaderType::Hull:
	{
		ID3D11HullShader* pShader = Cast<ID3D11HullShader*>(pShaderBuffer.Get());
		if (FAILED(m_pDevice->CreateHullShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pShader)))
			return E_FAIL;
		pShaderBuffer = Cast<ID3D11DeviceChild*>(pShader);
		break;
	}
	case Engine::EShaderType::Domain:
	{
		ID3D11DomainShader* pShader = Cast<ID3D11DomainShader*>(pShaderBuffer.Get());
		if (FAILED(m_pDevice->CreateDomainShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pShader)))
			return E_FAIL;
		pShaderBuffer = Cast<ID3D11DeviceChild*>(pShader);
		break;
	}
	case Engine::EShaderType::Compute:
	{
		ID3D11ComputeShader* pShader = Cast<ID3D11ComputeShader*>(pShaderBuffer.Get());
		if (FAILED(m_pDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pShader)))
			return E_FAIL;
		pShaderBuffer = Cast<ID3D11DeviceChild*>(pShader);
		break;
	}
	default:
		break;
	}

	pData->Set_Shader(strFileName, pBlob.Get(), pShaderBuffer.Get());

	return S_OK;
}


ID3DBlob* CShaderMgr::Read_ShaderBinary(const wstring& strFile)
{
	ifstream fin(strFile, ios::binary);
	if (fin.fail())
		return nullptr;

	fin.seekg(0, ios_base::end);
	ifstream::pos_type iSize = Cast<_int>(fin.tellg());
	fin.seekg(0, ios_base::beg);

	ID3DBlob* pBlob = nullptr;
	if (FAILED(D3DCreateBlob(iSize, &pBlob)))
		return nullptr;

	fin.read((_char*)pBlob->GetBufferPointer(), iSize);
	fin.close();

	return pBlob;
}


const ComPtr<ID3DBlob> CShaderMgr::Get_ShaderByte(const EShaderType eType, const wstring& strName) const
{
	_uint iIndex = ECast(eType);

	auto iter = m_mapShaderData[iIndex].find(strName);
	if (iter == m_mapShaderData[iIndex].end() || !(*iter).second->IsLoaded())
		return nullptr;

	return (*iter).second->Get_ShaderByte();
}



HRESULT CShaderMgr::Load_Effect(const wstring& strFileName, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements, const D3D_SHADER_MACRO** pShaderMacro)
{
	wstring strMacroDefine = TEXT("");
	if (pShaderMacro != nullptr)
	{
		_uint iErrorIter = 0U;
		_int i = 0;
		_char	szMacroId[MAX_PATH] = {};
		_int MacroIdIndex = 0;
		while (true)
		{
			// 이름이 nullptr 이면 종료한다.
			if ((pShaderMacro[i]) == nullptr)
				break;

			for (size_t j = 0; j < strlen((*pShaderMacro[i]).Name); j++)
			{
				szMacroId[MacroIdIndex++] = (*pShaderMacro[i]).Name[j];
				// 오류 방지
				if (++iErrorIter >= MAX_PATH)
					return E_FAIL;
			}

			szMacroId[MacroIdIndex++] = '_';
			// 오류 방지
			if (++iErrorIter >= MAX_PATH)
				return E_FAIL;
			
			for (size_t j = 0; j < strlen((*pShaderMacro[i]).Definition); j++)
			{
				szMacroId[MacroIdIndex++] = (*pShaderMacro[i]).Definition[j];
				// 오류 방지
				if (++iErrorIter >= MAX_PATH)
					return E_FAIL;
			}

			szMacroId[MacroIdIndex++] = '_';

			// 오류 방지
			if (++iErrorIter >= MAX_PATH)
				return E_FAIL;

			++i;
		}

		strMacroDefine = Make_Wstring(szMacroId);
	}

	wstring strSaveKey = strFileName + strMacroDefine;
	auto iter = m_mapEffects.find(strSaveKey);
	if (iter != m_mapEffects.end())
		return E_FAIL;

	_uint		iHlslFlag = 0;
	HRESULT		hr = 0;

#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	ComPtr<ID3DX11Effect> pEffect;
	if (FAILED(hr = D3DX11CompileEffectFromFile((m_strMainPath + strFileName).c_str(), ((pShaderMacro == nullptr) ? (nullptr) : (*pShaderMacro)),
		D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice.Get(), pEffect.GetAddressOf(), nullptr)))
		return E_FAIL;

	ID3DX11EffectTechnique* pTechnique = pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC tTechniqueDesc;
	pTechnique->GetDesc(&tTechniqueDesc);


	FEffectData* pEffectData = FEffectData::Create();
	pEffectData->tTechniqueDesc = tTechniqueDesc;
	pEffectData->pEffect = pEffect;

	for (_uint i = 0; i < tTechniqueDesc.Passes; i++)
	{
		ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(i);

		D3DX11_PASS_DESC	tPassDesc;
		pPass->GetDesc(&tPassDesc);

		ComPtr<ID3D11InputLayout> pInputLayout = { nullptr };
		if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElements, tPassDesc.pIAInputSignature, tPassDesc.IAInputSignatureSize, pInputLayout.GetAddressOf())))
			return E_FAIL;


		pEffectData->pInputLayouts.push_back(pInputLayout);
	}
	m_mapEffects.emplace(strSaveKey, pEffectData);

	return S_OK;
}

ID3DX11Effect* CShaderMgr::Find_Effect(const wstring& strKey) const
{
	auto iter = m_mapEffects.find(strKey);
	if (iter == m_mapEffects.end())
		return nullptr;

	return (*iter).second->pEffect.Get();
}

FEffectData* CShaderMgr::Find_EffectData(const wstring& strKey) const
{
	auto iter = m_mapEffects.find(strKey);
	if (iter == m_mapEffects.end())
		return nullptr;

	return (*iter).second;
}