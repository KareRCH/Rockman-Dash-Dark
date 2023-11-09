#include "Component/ShaderComponent.h"

CShaderComponent::CShaderComponent(const DX11DEVICE_T tDevice)
	: Base(tDevice)
{
}

CShaderComponent::CShaderComponent(const CShaderComponent& rhs)
	: Base(rhs)
{
}

void CShaderComponent::Free()
{
	SUPER::Free();

	Safe_Release(m_pLayout);
	Safe_Release(m_pMatrixBuffer);
	Safe_Release(m_pLightBuffer);
}

void CShaderComponent::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hWnd, const _tchar* pShaderFileName)
{
	OutputDebugString(ReCast<const _tchar*>(errorMessage->GetBufferPointer()));

	Safe_Release(errorMessage);

	MessageBox(hWnd, L"Error Compiling Shader.", pShaderFileName, MB_OK);
}
