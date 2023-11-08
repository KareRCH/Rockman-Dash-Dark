#include "System/ShaderMgr.h"

CShaderMgr::CShaderMgr(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
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
		Engine::Safe_Release(pInstance);

		MSG_BOX("RenderMgr Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CShaderMgr::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	// 셰이더 코드 해제
	for (auto& Pair : m_mapShaderData)
		Pair.second->Free();
	m_mapShaderData.clear();
}

HRESULT CShaderMgr::Load_Shader(const wstring& strFileName, const wstring& strKey)
{
	FShaderData* pData = nullptr;
	auto iter = m_mapShaderData.find(strKey);
	if (iter != m_mapShaderData.end())
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

		m_mapShaderData.emplace(strKey, pData);
	}

	// 여기서 셰이더 컴파일 혹은 컴파일된 셰이더 코드를 가지고 Blob에 저장한다.
	ID3DBlob* pBlob = Read_ShaderBinary(m_strMainPath + strFileName);
	if (nullptr == pBlob)
		return E_FAIL;

	pData->Set_Shader(strFileName, pBlob);

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

ID3DBlob* CShaderMgr::Get_Shader(const wstring& strName) const
{
	auto iter = m_mapShaderData.find(strName);
	if (iter == m_mapShaderData.end() || !(*iter).second->IsLoaded())
		return nullptr;

	return (*iter).second->Get_Shader();
}
