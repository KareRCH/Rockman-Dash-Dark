#include "Component/ModelBufferComp.h"

#include "System/GameInstance.h"
#include "System/ModelMgr.h"

CModelBufferComp::CModelBufferComp(const DX11DEVICE_T tDevice)
    : Base(tDevice)
{
}

CModelBufferComp::CModelBufferComp(const CModelBufferComp& rhs)
    : Base(rhs)
{
}

HRESULT CModelBufferComp::Initialize()
{

    return S_OK;
}

HRESULT CModelBufferComp::Initialize(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey, const wstring& strModelKey)
{
	const FMeshData* pMesh = GameInstance()->Get_Mesh(eGroupIndex, strGroupKey, strModelKey);

	m_iVtxCount = Cast<_uint>(pMesh->vecVertices.size());
	m_iIndexCount = Cast<_uint>(pMesh->vecIndices.size());
	Set_Transform(pMesh->matTransform);

	VERTEX_MODEL_SKIN_T* vertices = new VERTEX_MODEL_SKIN_T[m_iVtxCount];
	if (!vertices)
		return E_FAIL;

	_uint* indices = new _uint[m_iIndexCount];
	if (!indices)
		return E_FAIL;

	// 정점 버퍼 제작
	for (_uint i = 0; i < m_iVtxCount; i++)
	{
		vertices[i].vPosition = pMesh->vecVertices[i].vPosition;
		vertices[i].vNormal = pMesh->vecVertices[i].vNormal;
		vertices[i].vTexCoord = pMesh->vecVertices[i].vTexCoord;
		vertices[i].vTangent = pMesh->vecVertices[i].vTangent;
		for (_uint j = 0; j < (_uint)pMesh->vecVertices[j].vecBoneID.size() / 4; j++)
		{
			_uint iIndex = j * 4;
			vertices[i].vBoneID.x = pMesh->vecVertices[i].vecBoneID[iIndex];
			vertices[i].vBoneID.y = pMesh->vecVertices[i].vecBoneID[iIndex + 1];
			vertices[i].vBoneID.z = pMesh->vecVertices[i].vecBoneID[iIndex + 2];
			vertices[i].vBoneID.w = pMesh->vecVertices[i].vecBoneID[iIndex + 3];

			vertices[i].vWeight.x = pMesh->vecVertices[i].vecWeights[iIndex];
			vertices[i].vWeight.y = pMesh->vecVertices[i].vecWeights[iIndex + 1];
			vertices[i].vWeight.z = pMesh->vecVertices[i].vecWeights[iIndex + 2];
			vertices[i].vWeight.w = pMesh->vecVertices[i].vecWeights[iIndex + 3];
		}
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_MODEL_SKIN_T) * m_iVtxCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	FAILED_CHECK_RETURN(m_pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, m_pVtxBuffer.GetAddressOf()), E_FAIL);

	// 인덱스 버퍼 제작
	for (_uint i = 0; i < m_iIndexCount; i++)
	{
		indices[i] = pMesh->vecIndices[i];
	}

	// 정적 인덱스 버퍼의 구조체를 설정한다.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(_uint) * m_iIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 정적 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	FAILED_CHECK_RETURN(m_pDevice->CreateBuffer(&indexBufferDesc, &indexData, m_pIndexBuffer.GetAddressOf()), E_FAIL);

	Safe_Delete_Array(vertices);
	Safe_Delete_Array(indices);

	return S_OK;
}

void CModelBufferComp::Priority_Tick(const _float& fTimeDelta)
{
}

_int CModelBufferComp::Tick(const _float& fTimeDelta)
{
    return 0;
}

void CModelBufferComp::Late_Tick(const _float& fTimeDelta)
{
}

void CModelBufferComp::Render()
{
	_uint iStride = sizeof(VERTEX_MODEL_SKIN_T);
	_uint iOffset = 0;

	// 정점 버퍼 활성
	m_pDeviceContext->IASetVertexBuffers(0, 1, m_pVtxBuffer.GetAddressOf(), &iStride, &iOffset);

	// 인텍스 버퍼 활성
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// 정점 버퍼 삼각형 리스트
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

CModelBufferComp* CModelBufferComp::Create(const DX11DEVICE_T tDevice)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("ModelBufferComp Create Failed");

		return nullptr;
	}

	return pInstance;
}

CPrimitiveComponent* CModelBufferComp::Clone(void* Arg)
{
    return new ThisClass(*this);
}

void CModelBufferComp::Free()
{
    SUPER::Free();
}
