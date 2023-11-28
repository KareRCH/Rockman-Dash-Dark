#include "Component/ShaderComponent.h"

CShaderComponent::CShaderComponent(const CShaderComponent& rhs)
	: Base(rhs)
	, m_pDeviceComp(rhs.m_pDeviceComp)
{
	Safe_AddRef(m_pDeviceComp);
}


HRESULT CShaderComponent::Initialize_Prototype(void* Arg)
{
	m_pDeviceComp = CD3D11DeviceComp::Create();

	return S_OK;
}

void CShaderComponent::Free()
{
	Safe_Release(m_pDeviceComp);
}

void CShaderComponent::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hWnd, const _tchar* pShaderFileName)
{
	OutputDebugString(ReCast<const _tchar*>(errorMessage->GetBufferPointer()));

	Safe_Release(errorMessage);

	MessageBox(hWnd, L"Error Compiling Shader.", pShaderFileName, MB_OK);
}
