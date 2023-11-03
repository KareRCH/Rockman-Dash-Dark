#include "System/ModelMgr.h"

CModelMgr::CModelMgr()
{
}

HRESULT CModelMgr::Initialize(const string& strMainDir)
{
	m_strMainDir = strMainDir;

	return S_OK;
}

CModelMgr* CModelMgr::Create(const string& strMainDir)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize(strMainDir)))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("ModelMgr Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CModelMgr::Free()
{
	for (auto& Pair : m_mapModelGroup)
	{
		Pair.second->Free();
	}
	m_mapModelGroup.clear();
}

void CModelMgr::Load_Model(const string& strFileName, const string& strGroupKey)
{
	Assimp::Importer importer;

	_uint iFlag;
	iFlag = aiProcess_JoinIdenticalVertices |   // ������ ������ ����, �ε��� ����ȭ
		aiProcess_ValidateDataStructure |       // �δ��� ����� ����
		aiProcess_ImproveCacheLocality |        // ��� ������ ĳ����ġ�� ����
		aiProcess_RemoveRedundantMaterials |    // �ߺ��� ���͸��� ����
		aiProcess_GenUVCoords |                 // ����, ������, ���� �� ��� ������ ������ UV�� ��ȯ
		aiProcess_TransformUVCoords |           // UV ��ȯ ó���� (�����ϸ�, ��ȯ...)
		aiProcess_FindInstances |               // �ν��Ͻ��� �Ž��� �˻��Ͽ� �ϳ��� �����Ϳ� ���� ������ ����
		aiProcess_LimitBoneWeights |            // ������ ���� ����ġ�� �ִ� 4���� ����
		aiProcess_OptimizeMeshes |              // ������ ��� ���� �Ž��� ����
		aiProcess_GenSmoothNormals |            // �ε巯�� �븻����(��������) ����
		aiProcess_SplitLargeMeshes |			// �Ŵ��� �ϳ��� �Ž��� �����Ž���� ��Ȱ(����)
		aiProcess_Triangulate |                 // 3�� �̻��� �𼭸��� ���� �ٰ��� ���� �ﰢ������ ����(����)
		aiProcess_ConvertToLeftHanded |         // D3D�� �޼���ǥ��� ��ȯ
		aiProcess_SortByPType;					// ����Ÿ���� ������Ƽ��� ������ '������' �Ž��� ����

	m_pScene = importer.ReadFile((m_strMainDir + strFileName).c_str(), iFlag);

	if (!m_pScene || m_pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_pScene->mRootNode)
	{
#ifdef _DEBUG
		cout << "�� �� �ε� ����" << endl;
#endif
		return;
	}

	m_vecMesh.resize(m_pScene->mNumMeshes);
	m_iMaterialCount = m_pScene->mNumMaterials;
	m_iBoneCount = 0;

	aiNode* pRootNode = m_pScene->mRootNode;
	for (_uint i = 0; i < pRootNode->mNumChildren; i++)
	{
		aiNode* pChildNode = pRootNode->mChildren[i];
		for (_uint j = 0; j < pChildNode->mNumMeshes; j++)
		{
			aiMesh* pMesh = m_pScene->mMeshes[pChildNode->mMeshes[j]];

			// ��
			for (_uint k = 0; k < pMesh->mNumVertices; k++)
			{
				_float3 vPos(&pMesh->mVertices[k].x);
				_float3 vNormal(&pMesh->mNormals[k].x);
				_float2 vTexCoord;
				if (pMesh->HasTextureCoords(0))
					vTexCoord = _float2(&pMesh->mTextureCoords[0][k].x);
				else
					vTexCoord = _float2(0.f, 0.f);

				VERTEX_MODEL vData = { vPos, vNormal, vTexCoord };
				m_vecMesh[j].vecVertices.push_back(vData);
			}

			// ��
			for (_uint k = 0; k < pMesh->mNumFaces; k++)
			{
				aiFace& face = pMesh->mFaces[k];
				m_vecMesh[j].vecIndices.push_back(face.mIndices[0]);
				m_vecMesh[j].vecIndices.push_back(face.mIndices[1]);
				m_vecMesh[j].vecIndices.push_back(face.mIndices[2]);
			}
			
			for (_uint k = 0; k < pMesh->mNumBones; k++)
			{
				aiBone* pBone = pMesh->mBones[k];
			}

			FModelGroup* pGroup = FModelGroup::Create(false, true);
			pGroup->mapMesh.emplace(pMesh->mName.C_Str(), m_vecMesh[j]);
			m_mapModelGroup.emplace(strGroupKey, pGroup);
		}
	}
	
}

const MESH* const CModelMgr::Get_Model(const string& strGroupKey, const string& strModelKey)
{
	auto iter = m_mapModelGroup.find(strGroupKey);
	if (iter == m_mapModelGroup.end())
		return nullptr;

	auto iterModel = (*iter).second->mapMesh.find(strModelKey);
	if (iterModel == (*iter).second->mapMesh.end())
		return nullptr;

	return &(*iterModel).second;
}
