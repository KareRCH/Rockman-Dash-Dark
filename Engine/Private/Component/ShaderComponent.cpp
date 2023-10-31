#include "Component/ShaderComponent.h"

CShaderComponent::CShaderComponent(ID3D11Device* pDevice)
	: Base(pDevice)
{
}

CShaderComponent::CShaderComponent(const CShaderComponent& rhs)
	: Base(rhs)
{
}

void CShaderComponent::Free()
{
	SUPER::Free();

	Safe_Release(m_pVertexShader);
	Safe_Release(m_pPixelShader);
	Safe_Release(m_pLayout);
	Safe_Release(m_pMatrixBuffer);
}

void CShaderComponent::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hWnd, const _tchar* pShaderFileName)
{
	OutputDebugString(ReCast<const _tchar*>(errorMessage->GetBufferPointer()));

	Safe_Release(errorMessage);

	MessageBox(hWnd, L"Error Compiling Shader.", pShaderFileName, MB_OK);
}
