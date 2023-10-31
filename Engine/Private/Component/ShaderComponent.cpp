#include "Component/ShaderComponent.h"

CShaderComponent::CShaderComponent(ID3D11Device* pGraphicDev)
	: Base(pGraphicDev)
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
