#include "System/ModelMgr.h"

FModelData* FModelData::Create(const _bool bLoaded)
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FModelData Create Failed");

		return nullptr;
	}

	pInstance->pMeshGroup = FMeshGroup::Create();
	pInstance->pBoneGroup = FBoneGroup::Create();
	pInstance->pAnimGroup = FBoneAnimGroup::Create();
	pInstance->pBoneGroup = FBoneGroup::Create();
	pInstance->bLoaded = bLoaded;

	return pInstance;
}

void FModelData::Free()
{
	Safe_Release(pMeshGroup);
	Safe_Release(pBoneGroup);
	Safe_Release(pAnimGroup);
	Safe_Release(pBoneGroup);
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
	for (_uint i = 0; i < ECast(EModelGroupIndex::Size); i++)
	{
		for (auto& Pair : m_mapModelDatas[i])
		{
			Safe_Release(Pair.second);
		}
		m_mapModelDatas[i].clear();
	}
	
}

void CModelMgr::Load_Model(const EModelGroupIndex eGroupIndex, const string& strFileName, const wstring& strGroupKey)
{
	Assimp::Importer importer;
	
	_uint iFlag;
	iFlag = //aiProcess_JoinIdenticalVertices |   // 동일한 꼭지점 결합, 인덱싱 최적화
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
		aiProcess_PopulateArmatureData |		// Aramature의 정보를 뼈에 기록할 수 있게 하는 플래그
		aiProcess_SortByPType;					// 단일타입의 프리미티브로 구성된 '깨끗한' 매쉬를 만듬
	
	m_pScene = importer.ReadFile((m_strMainDir + strFileName).c_str(), iFlag);

	if (!m_pScene || m_pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_pScene->mRootNode)
	{
#ifdef _DEBUG
		cout << "모델 씬 로드 실패" << endl;
#endif
		return;
	}
	

	FModelData* pModelGroup = Add_ModelGroup(eGroupIndex, strGroupKey);	// 모델 그룹 로드, 없으면 추가후 로드
	
	// 메쉬, 뼈, 재질 로드
	Load_MeshBoneMaterial(pModelGroup);

	// 애니메이션 로드
	Load_Anim(pModelGroup);

	// 계층 로드
	//Load_Hierarchi(pModelGroup->pModelNodeGroup, m_pRootArmature);
}

void CModelMgr::Load_MeshBoneMaterial(FModelData* pModelGroup)
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
	_float4x4 matTest = {};
	vecMatrixTest.reserve(pRootNode->mNumChildren);
	for (_uint i = 0; i < pRootNode->mNumChildren; i++)
	{
		aiNode* pChildNode = pRootNode->mChildren[i];
		aiMatrix4x4 aiTransform = pChildNode->mTransformation;
		vecMatrixTest.push_back(ConvertAiMatrix_ToDXMatrix(aiTransform));
		matTest = ConvertAiMatrix_ToDXMatrix(aiTransform);
	}

	for (_uint i = 0; i < m_pScene->mNumMeshes; i++)
	{
		aiNode* pChildNode = pRootNode->mChildren[i];
		aiMesh* pMesh = m_pScene->mMeshes[i];

		// 트랜스폼 저장
		aiMatrix4x4 aiTransform = pChildNode->mTransformation;
		m_vecMesh[i]->matTransform = matTest;//ConvertAiMatrix_ToDXMatrix(aiTransform);

		// 뼈가 있을 때 루트 노드를 설정한다. 계층을 로드하는데 쓰인다.
		if (pMesh->HasBones())
		{
			m_pRootArmature = pMesh->mBones[0]->mArmature;
			Load_Hierarchi(pModelGroup->pBoneGroup, m_pRootArmature);
		}

		// 점
		vector<FMeshVertexData>& pVecVertices = m_vecMesh[i]->vecVertices;
		pVecVertices.resize(pMesh->mNumVertices);
		for (_uint j = 0; j < pMesh->mNumVertices; j++)
		{
			_uint iVertexIndex = j;

			_float3 vPos(&pMesh->mVertices[j].x);
			_float3 vNormal(&pMesh->mNormals[j].x);
			_float2 vTexCoord;
			_float3 vTangent, vBiTangent;

			if (pMesh->HasTextureCoords(0))
				vTexCoord = _float2(&pMesh->mTextureCoords[0][j].x);
			else
				vTexCoord = _float2(0.f, 0.f);

			if (pMesh->HasTangentsAndBitangents())
			{
				vTangent = _float3(&pMesh->mTangents[j].x);
				vBiTangent = _float3(&pMesh->mBitangents[j].x);
			}
			else
				vTangent = vBiTangent = _float3(0.f, 0.f, 0.f);

			m_vecMesh[i]->vecVertices[iVertexIndex] = { vPos, vNormal, vTexCoord, vTangent, vBiTangent };

			// 뼈 정보를 위해 공간 확보
			pVecVertices[iVertexIndex].vecBoneID.reserve(AI_LMW_MAX_WEIGHTS);
			pVecVertices[iVertexIndex].vecWeights.reserve(AI_LMW_MAX_WEIGHTS);
		}

		// 뼈
		// 정점에 대한 뼈정보 로드
		wstring strArmatureKey = Make_Wstring(pMesh->mBones[0]->mArmature->mName.C_Str());
		for (_uint j = 0; j < pMesh->mNumBones; j++)
		{
			wstring strNodeKey = Make_Wstring(pMesh->mBones[j]->mName.C_Str());
			auto pModelNode = pModelGroup->pBoneGroup->Find_NodeData(strArmatureKey, strNodeKey);

			_int iBoneID = pModelNode->iID;
			aiBone* pBone = pMesh->mBones[j];

			for (_uint k = 0; k < pBone->mNumWeights; k++)
			{
				_uint iVertexID = pBone->mWeights[k].mVertexId;
				_float fWeight = pBone->mWeights[k].mWeight;

				// 정점 데이터에 뼈ID와 무게 정보 등록
				if (fWeight != 0.f)
				{
					pVecVertices[iVertexID].vecBoneID.push_back(iBoneID);
					pVecVertices[iVertexID].vecWeights.push_back(fWeight);
				}
			}

			++iBoneID;
		}

		for (_uint j = 0; j < pMesh->mNumVertices; j++)
		{
			// 뼈 정보를 위해 공간 확보
			pVecVertices[j].vecBoneID.resize(AI_LMW_MAX_WEIGHTS, 0);
			pVecVertices[j].vecWeights.resize(AI_LMW_MAX_WEIGHTS, 0.f);
		}

		/*for (_uint j = 0; j < pMesh->mNumVertices; j++)
		{
			wstringstream ss;
			ss << pVecVertices[j].vecBoneID.size();
			OutputDebugString(wstring(wstring(L"Vertex VecBoneSize : ") + ss.str() + L"\n").c_str());
		}*/

		

		// 면 (인덱싱)
		for (_uint j = 0; j < pMesh->mNumFaces; j++)
		{
			aiFace& face = pMesh->mFaces[j];
			m_vecMesh[i]->vecIndices.push_back(face.mIndices[0]);
			m_vecMesh[i]->vecIndices.push_back(face.mIndices[1]);
			m_vecMesh[i]->vecIndices.push_back(face.mIndices[2]);
		}

		// 머터리얼
		for (_uint j = 0; j < m_pScene->mNumMaterials; j++)
		{
			aiMaterial* pMater = m_pScene->mMaterials[j];
			vector<aiMaterialProperty*> vecProp;
			vecProp.reserve(pMater->mNumProperties);

			for (_uint k = 0; k < pMater->mNumProperties; k++)
			{
				aiMaterialProperty* pProp = pMater->mProperties[k];
				wstring strN = Make_Wstring(pProp->mData);
				vecProp.push_back(pProp);
			}
			_uint tt = pMater->GetTextureCount(aiTextureType_DIFFUSE);
		}

		// 파싱데이터 임포트
		{
			// 메쉬 저장
			FMeshGroup* pMeshGroup = pModelGroup->pMeshGroup;

			// MeshKey 설정 및 저장
			pMeshGroup->Add_Mesh(Make_Wstring(pMesh->mName.C_Str()), m_vecMesh[i]);
		}
	}

	// 임시 메쉬 해제
	m_vecMesh.clear();
}

void CModelMgr::Load_Anim(FModelData* pModelGroup)
{
	FBoneAnimGroup* pAnimGroup = pModelGroup->pAnimGroup;
	FBoneGroup* pModelNodeGroup = pModelGroup->pBoneGroup;

	if (!pAnimGroup || !pModelNodeGroup)
		return;

	// 애니메이션
	for (_uint i = 0; i < m_pScene->mNumAnimations; i++)
	{
		aiAnimation* pAnimAI = m_pScene->mAnimations[i];

		// 애님 데이터 생성
		FBoneAnimData* pAnimData = FBoneAnimData::Create();
		wstring AnimNameWithTK = Make_Wstring(pAnimAI->mName.C_Str());
		pAnimData->dfDuration = pAnimAI->mDuration;
		pAnimData->dfTickPerSecond = pAnimAI->mTicksPerSecond;

		// 애니메이션 이름 추출
		size_t iTokkenInd = AnimNameWithTK.find_first_of(L'|') + Cast<size_t>(1);
		wstring AnimName = AnimNameWithTK.substr(iTokkenInd);

		// 애니메이션 채널
		for (_uint j = 0; j < pAnimAI->mNumChannels; j++)
		{
			aiNodeAnim* pNodeAnimAI = pAnimAI->mChannels[j];
			FBoneAnimNodeData* pAnimNodeData = FBoneAnimNodeData::Create();
			wstring AnimNodeName = Make_Wstring(pNodeAnimAI->mNodeName.C_Str());

			pAnimNodeData->vecPositions.reserve(pNodeAnimAI->mNumPositionKeys);
			pAnimNodeData->vecRotations.reserve(pNodeAnimAI->mNumRotationKeys);
			pAnimNodeData->vecScales.reserve(pNodeAnimAI->mNumScalingKeys);

			// Pos 값 추출
			for (_uint k = 0; k < pNodeAnimAI->mNumPositionKeys; k++)
			{
				auto fTime = pNodeAnimAI->mPositionKeys[k].mTime;
				auto vPos = pNodeAnimAI->mPositionKeys[k].mValue;
				FBoneAnimNodeData::FPosition tPosition = {
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
				FBoneAnimNodeData::FRotation tRotation = {
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
				FBoneAnimNodeData::FScale tScale = {
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

void CModelMgr::Load_Hierarchi(FBoneGroup* pModelNodeGroup,  aiNode* pArmatureNode)
{
	if (pArmatureNode)
	{
		// 한번만 로드
		wstring strKey = Make_Wstring(pArmatureNode->mName.C_Str());	// 아마추어 키
		auto pArmature = pModelNodeGroup->Find_ArmatureData(strKey);
		// 이미 있으면 만들어진 것으로 판별
		if (pArmature)
			return;

		// 아마추어 컨테이너 생성
		pArmature = pModelNodeGroup->Create_ArmatureData(strKey);
		if (!pArmature)
			return;

		m_iNodeID = 0;

		// ID 지정
		FBoneNodeData* pRootNodeData = pModelNodeGroup->Create_NodeData(strKey, strKey);
		pRootNodeData->iID = m_iNodeID ++;
		pRootNodeData->eType = EModelNodeType::Armature;
		pRootNodeData->eBoneType = EModelBoneType::Null;
		pRootNodeData->strName = strKey;
		pRootNodeData->pParent = nullptr;
		pRootNodeData->matOffset = ConvertAiMatrix_ToDXMatrix(pArmatureNode->mTransformation);
		pRootNodeData->matTransform = pRootNodeData->matOffset;

		for (_uint i = 0; i < pArmatureNode->mNumChildren; i++)
		{
			Load_HierarchiNode(pModelNodeGroup, pArmatureNode->mChildren[i], pRootNodeData, pRootNodeData);
		}

		pModelNodeGroup->Appoint_ArmatureNode(strKey, strKey);
	}

	m_pRootArmature = nullptr;
}

void CModelMgr::Load_HierarchiNode(FBoneGroup* pModelNodeGroup, aiNode* pBoneNode, FBoneNodeData* pRootNode, FBoneNodeData* pParentNode)
{
	if (pBoneNode)
	{
		wstring strArmatureKey = pRootNode->strName;
		wstring strNodeKey = Make_Wstring(pBoneNode->mName.C_Str());

		FBoneNodeData* pNodeData = pModelNodeGroup->Create_NodeData(strArmatureKey, strNodeKey);
		pNodeData->iID = m_iNodeID++;
		pNodeData->eType = EModelNodeType::Bone;
		pNodeData->eBoneType = EModelBoneType::Base;
		pNodeData->strName = strNodeKey;
		pNodeData->pParent = pParentNode;
		pNodeData->matOffset = ConvertAiMatrix_ToDXMatrix(pBoneNode->mTransformation);
		pNodeData->matTransform = pNodeData->matOffset;
		pParentNode->vecChildren.push_back(pNodeData);
		
		if (pBoneNode->mNumChildren == 0)
			pNodeData->eBoneType = EModelBoneType::End;

		for (_uint i = 0; i < pBoneNode->mNumChildren; i++)
		{
			Load_HierarchiNode(pModelNodeGroup, pBoneNode->mChildren[i], pRootNode, pNodeData);
		}
	}
}

const FMeshData* const CModelMgr::Get_Mesh(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey, const wstring& strMeshKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strGroupKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	return (*iter).second->pMeshGroup->Get_Mesh(strMeshKey);
}

FArmatureData* CModelMgr::Find_Armature(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey, const wstring strModelNodeKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strGroupKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	FModelData* pModelGroup = (*iter).second;
	return pModelGroup->pBoneGroup->Find_ArmatureData(strModelNodeKey);
}

FArmatureData* CModelMgr::Clone_Armature(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey, const wstring strModelNodeKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strGroupKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	FModelData* pModelGroup = (*iter).second;
	return pModelGroup->pBoneGroup->Clone_ArmatureData(strModelNodeKey);
}

FModelData* CModelMgr::Get_ModelGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strGroupKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	return (*iter).second;
}

FModelData* CModelMgr::Add_ModelGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strGroupKey);
	if (iter != m_mapModelDatas[iIndex].end())
		return (*iter).second;
	
	FModelData* pGroup = FModelData::Create(false);
	m_mapModelDatas[iIndex].emplace(strGroupKey, pGroup);

	return pGroup;
}

FMeshGroup* CModelMgr::Get_MeshGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strGroupKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	return (*iter).second->pMeshGroup;
}

FBoneGroup* CModelMgr::Get_BoneGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strGroupKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	return (*iter).second->pBoneGroup;
}

FBoneAnimGroup* CModelMgr::Get_AnimGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strGroupKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	return (*iter).second->pAnimGroup;
}

_float4x4 CModelMgr::ConvertAiMatrix_ToDXMatrix(aiMatrix4x4& matrix)
{
	_float4x4	matOffsetTG = _float4x4(
		matrix.a1, matrix.a2, matrix.a3, matrix.a4,
		matrix.b1, matrix.b2, matrix.b3, matrix.b4,
		matrix.c1, matrix.c2, matrix.c3, matrix.c4,
		matrix.d1, matrix.d2, matrix.d3, matrix.d4
	);

	return matOffsetTG;
}

_float3 CModelMgr::Calculate_InterpolatedFloat3(_float fAnimTime, const _int iNumKeys, const _vector vVectorKey)
{


	return _float3();
}

_float4 CModelMgr::Calculate_InterpolatedQuaternion(_float fAnimTime, const _int iNumKeys, const _vector vVectorKey)
{
	return _float4();
}


