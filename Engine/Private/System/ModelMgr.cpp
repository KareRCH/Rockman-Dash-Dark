#include "System/ModelMgr.h"

FModelGroup* FModelGroup::Create(const _bool bPermanent, const _bool bLoaded)
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FModelGroup Create Failed");

		return nullptr;
	}

	pInstance->bPermanent = bPermanent;
	pInstance->bLoaded = bLoaded;

	return pInstance;
}

void FModelGroup::Free()
{
	Safe_Release(pMeshGroup);
	Safe_Release(pBoneGroup);
	Safe_Release(pAnimGroup);
}

// ----------------------- ModelMgr ---------------------

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
		Safe_Release(Pair.second);
	}
	m_mapModelGroup.clear();
}

void CModelMgr::Load_Model(const string& strFileName, const wstring& strGroupKey)
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

	FModelGroup* pModelGroup = Add_ModelGroup(strGroupKey);	// 모델 그룹 로드, 없으면 추가후 로드
	
	// 메쉬, 뼈, 재질 로드
	Load_MeshBoneMaterial(pModelGroup);

	// 애니메이션 로드
	Load_Anim(pModelGroup->pAnimGroup);

	// 계층 로드
	Load_Hierarchi(pModelGroup);
}

void CModelMgr::Load_MeshBoneMaterial(FModelGroup* pModelGroup)
{
	// 임시 메쉬 벡터
	m_vecMesh.resize(m_pScene->mNumMeshes);
	for (size_t i = 0; i < m_vecMesh.size(); i++)
		m_vecMesh[i] = FMeshData::Create();

	m_iMaterialCount = m_pScene->mNumMaterials;
	m_iBoneCount = 0;

	FMeshGroup* pMeshGroup = pModelGroup->pMeshGroup;
	FBoneGroup* pBoneGroup = pModelGroup->pBoneGroup;

	_float4x4 matTransformTest = {};
	aiNode* pRootNode = m_pScene->mRootNode;

	// 테스트용 메타데이터
	vector<pair<aiString*, aiMetadataEntry*>> pMetaDatas;
	pMetaDatas.reserve(m_pScene->mMetaData->mNumProperties);
	for (_uint i = 0; i < m_pScene->mMetaData->mNumProperties; i++)
	{
		aiMetadata* pMeta = m_pScene->mMetaData;
		aiString* str = &pMeta->mKeys[i];
		aiMetadataEntry* pEntry = &pMeta->mValues[i];
		pMetaDatas.push_back({ str, pEntry });
	}

	// 블렌더에서는 행렬이 변환되어 나오기 때문에 부모가 되는 Skeletal의 행렬은 쓰지 않는다.
	vector<_float4x4> vecMatrixTest;
	vecMatrixTest.reserve(pRootNode->mNumChildren);
	for (_uint i = 0; i < pRootNode->mNumChildren; i++)
	{
		aiNode* pChildNode = pRootNode->mChildren[i];
		aiMatrix4x4 aiTransform = pChildNode->mTransformation;
		vecMatrixTest.push_back(_float4x4(
			aiTransform.a1, aiTransform.a2, aiTransform.a3, aiTransform.a4,
			aiTransform.b1, aiTransform.b2, aiTransform.b3, aiTransform.b4,
			aiTransform.c1, aiTransform.c2, aiTransform.c3, aiTransform.c4,
			aiTransform.d1, aiTransform.d2, aiTransform.d3, aiTransform.d4
		));
	}

	for (_uint i = 0; i < m_pScene->mNumMeshes; i++)
	{
		aiNode* pChildNode = pRootNode->mChildren[i];
		aiMesh* pMesh = m_pScene->mMeshes[i];

		// 트랜스폼 저장
		aiMatrix4x4 aiTransform = pChildNode->mTransformation;
		_float4x4 matTransform = _float4x4(
			aiTransform.a1, aiTransform.a2, aiTransform.a3, aiTransform.a4,
			aiTransform.b1, aiTransform.b2, aiTransform.b3, aiTransform.b4,
			aiTransform.c1, aiTransform.c2, aiTransform.c3, aiTransform.c4,
			aiTransform.d1, aiTransform.d2, aiTransform.d3, aiTransform.d4
		);
		m_vecMesh[i]->matTransform = XMLoadFloat4x4(&matTransform);
		//m_vecMesh[i]->matTransform = matTransform;

		// 점
		for (_uint j = 0; j < pMesh->mNumVertices; j++)
		{
			_float3 vPos(&pMesh->mVertices[j].x);
			_float3 vNormal(&pMesh->mNormals[j].x);
			_float2 vTexCoord;
			if (pMesh->HasTextureCoords(0))
				vTexCoord = _float2(&pMesh->mTextureCoords[0][j].x);
			else
				vTexCoord = _float2(0.f, 0.f);

			VERTEX_MODEL vData = { vPos, vNormal, vTexCoord };
			m_vecMesh[i]->vecVertices.push_back(vData);
		}

		// 면 (인덱싱)
		for (_uint j = 0; j < pMesh->mNumFaces; j++)
		{
			aiFace& face = pMesh->mFaces[j];
			m_vecMesh[i]->vecIndices.push_back(face.mIndices[0]);
			m_vecMesh[i]->vecIndices.push_back(face.mIndices[1]);
			m_vecMesh[i]->vecIndices.push_back(face.mIndices[2]);
		}

		// 뼈
		m_vecMesh[i]->vecBone.reserve(pMesh->mNumBones);
		for (_uint j = 0; j < pMesh->mNumBones; j++)
		{
			aiBone* pBone = pMesh->mBones[j];
			FBoneData* pBoneData = FBoneData::Create();
			FMeshData::FVertexBoneData tVtxBoneData = {};

			pBoneData->strName = tVtxBoneData.strName = Make_Wstring(pBone->mName.C_Str());
			tVtxBoneData.vecVtxID.reserve(pBone->mNumWeights);
			tVtxBoneData.vecWeights.reserve(pBone->mNumWeights);

			for (_uint k = 0; k < pBone->mNumWeights; k++)
			{
				tVtxBoneData.vecVtxID.push_back(pBone->mWeights[k].mVertexId);
				tVtxBoneData.vecWeights.push_back(pBone->mWeights[k].mWeight);

				pBoneData->vecVtxIndex.push_back(pBone->mWeights[k].mVertexId);
				pBoneData->vecVtxWeights.push_back(pBone->mWeights[k].mWeight);
			}
			
			m_vecMesh[i]->vecBone.push_back(tVtxBoneData);
			pBoneGroup->Add_Bone(Make_Wstring(pBone->mName.C_Str()), pBoneData);
			int ttt = 0;
		}

		// 머터리얼
		for (_uint j = 0; j < m_pScene->mNumMaterials; j++)
		{
			aiMaterial* pMater = m_pScene->mMaterials[j];
			vector< aiMaterialProperty*> vecProp;
			vecProp.reserve(pMater->mNumProperties);

			for (_uint k = 0; k < pMater->mNumProperties; k++)
			{
				aiMaterialProperty* pProp = pMater->mProperties[k];
				wstring strN = Make_Wstring(pProp->mData);
				vecProp.push_back(pProp);
			}
			_uint tt = pMater->GetTextureCount(aiTextureType_DIFFUSE);
			//pMater->GetTexture(aiTextureType_DIFFUSE, )
			//string strTest = pMater->GetName().C_Str();
			//tt = -1;
		}

		// 파싱데이터 임포트
		{
			// 메쉬 저장
			FMeshGroup* pMeshGroup = pModelGroup->pMeshGroup;

			// MeshKey 설정 및 저장
			pMeshGroup->Add_Mesh(Make_Wstring(pMesh->mName.C_Str()), m_vecMesh[i]);

			//pBoneGroup->Add_Bone(Make_Wstring(pBone))
		}
	}

	// 임시 메쉬 해제
	m_vecMesh.clear();
}

void CModelMgr::Load_Anim(FAnimGroup* pAnimGroup)
{
	if (!pAnimGroup)
		return;

	// 애니메이션
	for (_uint i = 0; i < m_pScene->mNumAnimations; i++)
	{
		aiAnimation* pAnimAI = m_pScene->mAnimations[i];
		// 애님 데이터 생성
		FAnimData* pAnimData = FAnimData::Create();
		wstring AnimNameWithTK = Make_Wstring(pAnimAI->mName.C_Str());
		// 애니메이션 이름 추출
		size_t iTokkenInd = AnimNameWithTK.find_first_of(L'|') + Cast<size_t>(1);
		wstring AnimName = AnimNameWithTK.substr(iTokkenInd);

		for (_uint j = 0; j < pAnimAI->mNumChannels; j++)
		{
			aiNodeAnim* pNodeAnimAI = pAnimAI->mChannels[j];
			FAnimNodeData* pAnimNodeData = FAnimNodeData::Create();
			wstring AnimNodeName = Make_Wstring(pNodeAnimAI->mNodeName.C_Str());

			pAnimNodeData->vecPositions.reserve(pNodeAnimAI->mNumPositionKeys);
			pAnimNodeData->vecRotations.reserve(pNodeAnimAI->mNumRotationKeys);
			pAnimNodeData->vecScales.reserve(pNodeAnimAI->mNumScalingKeys);

			// Pos 값 추출
			for (_uint k = 0; k < pNodeAnimAI->mNumPositionKeys; k++)
			{
				auto fTime = pNodeAnimAI->mPositionKeys[k].mTime;
				auto vPos = pNodeAnimAI->mPositionKeys[k].mValue;
				FAnimNodeData::FAnimPosition tPosition = {
					Cast<_float>(fTime),
					{ vPos.x, vPos.y, vPos.z }
				};
				pAnimNodeData->vecPositions.push_back(tPosition);
			}

			// Rot 값 추출 (쿼터니언)
			for (_uint k = 0; k < pNodeAnimAI->mNumRotationKeys; k++)
			{
				auto fTime = pNodeAnimAI->mRotationKeys[k].mTime;
				auto vRot = pNodeAnimAI->mRotationKeys[k].mValue;
				FAnimNodeData::FAnimRotation tRotation = {
					Cast<_float>(fTime),
					{ vRot.w, vRot.x, vRot.y, vRot.z }
				};
				pAnimNodeData->vecRotations.push_back(tRotation);
			}

			// Scale 값 추출
			for (_uint k = 0; k < pNodeAnimAI->mNumScalingKeys; k++)
			{
				auto fTime = pNodeAnimAI->mScalingKeys[k].mTime;
				auto vScale = pNodeAnimAI->mScalingKeys[k].mValue;
				FAnimNodeData::FAnimScale tScale = {
					Cast<_float>(fTime),
					{ vScale.x, vScale.y, vScale.z }
				};
				pAnimNodeData->vecScales.push_back(tScale);
			}

			pAnimData->Add_AnimNodeData(AnimNodeName, pAnimNodeData);
		}
		pAnimGroup->Add_AnimData(AnimName, pAnimData);

		int t = 1;
	}
}

void CModelMgr::Load_Hierarchi(FModelGroup* pModelGroup)
{
}

const FMeshData* const CModelMgr::Get_Mesh(const wstring& strGroupKey, const wstring& strMeshKey)
{
	auto iter = m_mapModelGroup.find(strGroupKey);
	if (iter == m_mapModelGroup.end())
		return nullptr;

	return (*iter).second->pMeshGroup->Get_Mesh(strMeshKey);
}

FModelGroup* CModelMgr::Get_ModelGroup(const wstring& strGroupKey)
{
	auto iter = m_mapModelGroup.find(strGroupKey);
	if (iter == m_mapModelGroup.end())
		return nullptr;

	return (*iter).second;
}

FModelGroup* CModelMgr::Add_ModelGroup(const wstring& strGroupKey)
{
	auto iter = m_mapModelGroup.find(strGroupKey);
	if (iter != m_mapModelGroup.end())
		return (*iter).second;

	FModelGroup* pGroup = FModelGroup::Create(false, false);
	m_mapModelGroup.emplace(strGroupKey, pGroup);

	pGroup->pMeshGroup = FMeshGroup::Create();
	pGroup->pBoneGroup = FBoneGroup::Create();
	pGroup->pAnimGroup = FAnimGroup::Create();

	return pGroup;
}

FMeshGroup* CModelMgr::Get_MeshGroup(const wstring& strGroupKey)
{
	auto iter = m_mapModelGroup.find(strGroupKey);
	if (iter == m_mapModelGroup.end())
		return nullptr;

	return (*iter).second->pMeshGroup;
}

FBoneGroup* CModelMgr::Get_BoneGroup(const wstring& strGroupKey)
{
	auto iter = m_mapModelGroup.find(strGroupKey);
	if (iter == m_mapModelGroup.end())
		return nullptr;

	return (*iter).second->pBoneGroup;
}

FAnimGroup* CModelMgr::Get_AnimGroup(const wstring& strGroupKey)
{
	auto iter = m_mapModelGroup.find(strGroupKey);
	if (iter == m_mapModelGroup.end())
		return nullptr;

	return (*iter).second->pAnimGroup;
}


