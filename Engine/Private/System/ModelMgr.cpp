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
	iFlag = aiProcess_JoinIdenticalVertices |   // 동일한 꼭지점 결합, 인덱싱 최적화
		aiProcess_ValidateDataStructure |       // 로더의 출력을 검증
		aiProcess_ImproveCacheLocality |        // 출력 정점의 캐쉬위치를 개선
		aiProcess_RemoveRedundantMaterials |    // 중복된 매터리얼 제거
		aiProcess_GenUVCoords |                 // 구형, 원통형, 상자 및 평면 매핑을 적절한 UV로 변환
		aiProcess_TransformUVCoords |           // UV 변환 처리기 (스케일링, 변환...)
		aiProcess_FindInstances |               // 인스턴스된 매쉬를 검색하여 하나의 마스터에 대한 참조로 제거
		aiProcess_LimitBoneWeights |            // 정점당 뼈의 가중치를 최대 4개로 제한
		aiProcess_OptimizeMeshes |              // 가능한 경우 작은 매쉬를 조인
		aiProcess_GenSmoothNormals |            // 부드러운 노말벡터(법선벡터) 생성
		aiProcess_SplitLargeMeshes |			// 거대한 하나의 매쉬를 하위매쉬들로 분활(나눔)
		aiProcess_Triangulate |                 // 3개 이상의 모서리를 가진 다각형 면을 삼각형으로 만듬(나눔)
		aiProcess_ConvertToLeftHanded |         // D3D의 왼손좌표계로 변환
		aiProcess_SortByPType;					// 단일타입의 프리미티브로 구성된 '깨끗한' 매쉬를 만듬

	m_pScene = importer.ReadFile((m_strMainDir + strFileName).c_str(), iFlag);

	if (!m_pScene || m_pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_pScene->mRootNode)
	{
#ifdef _DEBUG
		cout << "모델 씬 로드 실패" << endl;
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

			// 점
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

			// 면
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
