#include "Component/MeshComp.h"

#include "Component/EffectComponent.h"
#include "System/Data/MeshData.h"
#include "System/Data/BoneData.h"

CMeshComp::CMeshComp(const CMeshComp& rhs)
    : Base(rhs)
{
}

HRESULT CMeshComp::Initialize_Prototype(void* Arg)
{

    return S_OK;
}

HRESULT CMeshComp::Initialize(void* Arg)
{

    return S_OK;
}

CMeshComp* CMeshComp::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("TextureComponent Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMeshComp::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(Arg)))
	{
		MSG_BOX("TextureComponent Copy Failed");
		Safe_Release(pInstance);
	}

	return Cast<CComponent*>(pInstance);
}

void CMeshComp::Free()
{
	SUPER::Free();

	Safe_Release(m_pMeshData);
}

_uint CMeshComp::Get_MeshMaterialIndex() const
{
	if (nullptr == m_pMeshData)
		return 0;

	return m_pMeshData->iMaterialIndex;
}

HRESULT CMeshComp::Load_Mesh(EModelGroupIndex eGroupIndex, const wstring& strModelFilePath, _uint iIndex)
{
	if (nullptr == m_pGI)
		return E_FAIL;

	auto pMeshData = m_pGI->Find_MeshData(eGroupIndex, strModelFilePath, iIndex);
	if (nullptr == pMeshData)
		return E_FAIL;

	// 메시 데이터를 저장한다.
	m_pMeshData = ConCast<FMeshData*>(pMeshData);
	Safe_AddRef(m_pMeshData);

	return S_OK;
}

HRESULT CMeshComp::Bind_BoneMatricesToEffect(CEffectComponent* pEffect, const _char* pConstantName, const CBoneGroup& BoneGroup)
{
	if (nullptr == pEffect || nullptr == m_pMeshData)
		return E_FAIL;

	_float4x4 matBind[128] = {};

	size_t iMeshBoneCount = m_pMeshData->vecMeshBoneDatas.size();
	for (size_t i = 0; i < iMeshBoneCount; i++)
	{
		const _float4x4* const pBoneFinalTransform = BoneGroup.Provide_BoneFinalTransformPtr(m_pMeshData->vecMeshBoneDatas[i].iBoneID);
		if (!pBoneFinalTransform)
			return E_FAIL;

		XMStoreFloat4x4(&matBind[i],
			XMLoadFloat4x4(&m_pMeshData->vecMeshBoneDatas[i].matOffset) * XMLoadFloat4x4(pBoneFinalTransform));
	}

	return pEffect->Bind_Matrices(pConstantName, matBind, 128);
}

HRESULT CMeshComp::Bind_Buffer()
{
	if (nullptr == m_pGI || nullptr == m_pMeshData)
		return E_FAIL;

	if (nullptr == m_pMeshData->pVB || nullptr == m_pMeshData->pIB)
		return E_FAIL;

	ID3D11Buffer* pVBs[] = {
		m_pMeshData->pVB.Get()
	};

	_uint           iStrides[] = {
		m_pMeshData->iVertexStride
	};

	_uint           iOffsets[] = {
		0,
	};

	auto pContext = m_pGI->Get_GraphicContext();

	/* 어떤 버텍스 버퍼들을 이용할거다. */
	pContext->IASetVertexBuffers(0, 1, pVBs, iStrides, iOffsets);

	/* 어떤 인덱스 버퍼를 이용할거다. */
	pContext->IASetIndexBuffer(m_pMeshData->pIB.Get(), m_pMeshData->eIndexFormat, 0);

	/* 정점을 어떤식으로 이어서 그릴거다. */
	pContext->IASetPrimitiveTopology(m_pMeshData->eTopology);

	return S_OK;
}

HRESULT CMeshComp::Render_Buffer()
{
	if (nullptr == m_pGI || nullptr == m_pMeshData)
		return E_FAIL;

	m_pGI->Get_GraphicContext()->DrawIndexed(m_pMeshData->iNumIndices, 0, 0);

	return S_OK;
}
