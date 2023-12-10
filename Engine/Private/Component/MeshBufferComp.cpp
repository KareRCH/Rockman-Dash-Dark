#include "Component/MeshBufferComp.h"

#include "Component/EffectComponent.h"
#include "System/Data/MeshData.h"

CMeshBufferComp::CMeshBufferComp(const CMeshBufferComp& rhs)
    : Base(rhs)
{
}

HRESULT CMeshBufferComp::Initialize_Prototype(void* Arg)
{
	if (FAILED(__super::Initialize_Prototype(Arg)))
		return E_FAIL;

    return S_OK;
}

HRESULT CMeshBufferComp::Initialize(void* Arg)
{
	if (FAILED(__super::Initialize(Arg)))
		return E_FAIL;

    return S_OK;
}

void CMeshBufferComp::Priority_Tick(const _float& fTimeDelta)
{
}

void CMeshBufferComp::Tick(const _float& fTimeDelta)
{
}

void CMeshBufferComp::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CMeshBufferComp::Render()
{
    return S_OK;
}

CMeshBufferComp* CMeshBufferComp::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("TextureComponent Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

CComponent* CMeshBufferComp::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(Arg)))
	{
		MSG_BOX("TextureComponent Copy Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return Cast<CComponent*>(pInstance);
}

void CMeshBufferComp::Free()
{
	SUPER::Free();
}

HRESULT CMeshBufferComp::Bind_EffectBoneMatrices(CEffectComponent* pEffect, const _char* pConstantName)
{
	_float4x4 BoneMatrices[256];

	/*for (_uint i = 0; i < m_pMeshData->; i++)
	{

	}*/

	return pEffect->Bind_Matrices(pConstantName, BoneMatrices, 256);
}
