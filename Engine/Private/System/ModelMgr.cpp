#include "System/ModelMgr.h"

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
		MSG_BOX("ModelMgr Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CModelMgr::Free()
{
	for (_uint i = 0; i < ECast(EModelGroupIndex::Size); i++)
	{
		for (auto& Pair : m_mapModelDatas[i])
			Safe_Release(Pair.second);
		m_mapModelDatas[i].clear();
	}
}

void CModelMgr::Load_Model(const EModelGroupIndex eGroupIndex, const string& strFileName, const wstring& strModelKey)
{
	Assimp::Importer importer;
	
	_uint iFlag;
	iFlag = aiProcessPreset_TargetRealtime_Fast |
		aiProcess_LimitBoneWeights |
		aiProcess_Triangulate |
		aiProcess_PopulateArmatureData |
		aiProcess_ConvertToLeftHanded |
		aiProcess_SortByPType;
	//iFlag = aiProcess_JoinIdenticalVertices |   // 동일한 꼭지점 결합, 인덱싱 최적화
	//	aiProcess_ValidateDataStructure |       // 로더의 출력을 검증
	//	aiProcess_ImproveCacheLocality |        // 출력 정점의 캐쉬위치를 개선
	//	aiProcess_RemoveRedundantMaterials |    // 중복된 매터리얼 제거
	//	aiProcess_GenUVCoords |                 // 구형, 원통형, 상자 및 평면 매핑을 적절한 UV로 변환
	//	aiProcess_TransformUVCoords |           // UV 변환 처리기 (스케일링, 변환...)
	//	aiProcess_FindInstances |               // 인스턴스된 매쉬를 검색하여 하나의 마스터에 대한 참조로 제거
	//	aiProcess_LimitBoneWeights |            // 정점당 뼈의 가중치를 최대 4개로 제한
	//	aiProcess_OptimizeMeshes |              // 가능한 경우 작은 매쉬를 조인
	//	aiProcess_GenSmoothNormals |            // 부드러운 노말벡터(법선벡터) 생성
	//	aiProcess_SplitLargeMeshes |			// 거대한 하나의 매쉬를 하위매쉬들로 분활(나눔)
	//	aiProcess_Triangulate |                 // 3개 이상의 모서리를 가진 다각형 면을 삼각형으로 만듬(나눔)
	//	aiProcess_ConvertToLeftHanded |         // D3D의 왼손좌표계로 변환
	//	aiProcess_PopulateArmatureData |		// Aramature의 정보를 뼈에 기록할 수 있게 하는 플래그
	//	aiProcess_SortByPType;					// 단일타입의 프리미티브로 구성된 '깨끗한' 매쉬를 만듬
	
	m_strLoadFilePath = strFileName;
	m_pScene = importer.ReadFile((m_strMainDir + strFileName).c_str(), iFlag);

	if (!m_pScene || m_pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_pScene->mRootNode)
	{
#ifdef _DEBUG
		cout << "모델 씬 로드 실패" << endl;
#endif
		return;
	}
	

	FModelData* pModelData = Add_ModelData(eGroupIndex, strModelKey);	// 모델 그룹 로드, 없으면 추가후 로드
	


	// 메쉬, 뼈, 재질 로드
	Load_Hierarchi(pModelData->pBoneGroup, m_pScene->mRootNode);

	Load_Material(pModelData);

	Load_Mesh(pModelData);

	// 애니메이션 로드
	Load_Animation(pModelData);

	importer.FreeScene();
}

void CModelMgr::Load_Mesh(FModelData* pModelData)
{
	// 테스트용 메타데이터
	/*vector<pair<aiString*, aiMetadataEntry*>> pMetaDatas;
	pMetaDatas.reserve(m_pScene->mMetaData->mNumProperties);
	for (_uint i = 0; i < m_pScene->mMetaData->mNumProperties; i++)
	{
		aiMetadata* pMeta = m_pScene->mMetaData;
		aiString* str = &pMeta->mKeys[i];
		aiMetadataEntry* pEntry = &pMeta->mValues[i];
		pMetaDatas.push_back({ str, pEntry });
	}*/

	// 블렌더에서는 행렬이 변환되어 나오기 때문에 부모가 되는 Skeletal의 행렬은 쓰지 않는다.
	/*vector<_float4x4> vecMatrixTest;
	_float4x4 matTest = {};
	vecMatrixTest.reserve(pRootNode->mNumChildren);
	for (_uint i = 0; i < pRootNode->mNumChildren; i++)
	{
		aiNode* pChildNode = pRootNode->mChildren[i];
		aiMatrix4x4 aiTransform = pChildNode->mTransformation;
		vecMatrixTest.push_back(ConvertAiMatrix_ToDXMatrix(aiTransform));
		matTest = ConvertAiMatrix_ToDXMatrix(aiTransform);
	}*/

	// 저장용 메쉬 생성, 메쉬 개수 만큼 만들기
	vector<FMeshData*>	vecMeshes;
	vecMeshes.resize(m_pScene->mNumMeshes);
	for (size_t i = 0; i < vecMeshes.size(); i++)
	{
		vecMeshes[i] = FMeshData::Create();
		vecMeshes[i]->iID = Cast<_uint>(i);
	}

	FMeshGroup* pMeshGroup = pModelData->pMeshGroup;
	aiNode* pRootNode = m_pScene->mRootNode;

	// 메쉬 로드
	for (_uint i = 0; i < m_pScene->mNumMeshes; i++)
	{
		aiNode* pChildNode = pRootNode->mChildren[i];
		aiMesh* pMesh = m_pScene->mMeshes[i];

		// MeshKey 설정 및 저장
		wstring strMeshName = Make_Wstring(pMesh->mName.C_Str());
		pMeshGroup->Add_Mesh(strMeshName, vecMeshes[i]);

		// 트랜스폼 저장
		aiMatrix4x4 aiTransform = pChildNode->mTransformation;
		vecMeshes[i]->matOffset = ConvertAiMatrix_ToDXMatrix(aiTransform);

		// 뼈가 있을 때 루트 노드를 설정한다. 계층을 로드하는데 쓰인다.
#pragma region 점
		// 점
		vector<FMeshVertexData>& pVecVertices = vecMeshes[i]->vecVertices;
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

			vecMeshes[i]->vecVertices[iVertexIndex] = { vPos, vNormal, vTexCoord, vTangent, vBiTangent };

			// 뼈 정보를 위해 공간 확보
			pVecVertices[iVertexIndex].vecBoneID.reserve(AI_LMW_MAX_WEIGHTS);
			pVecVertices[iVertexIndex].vecWeights.reserve(AI_LMW_MAX_WEIGHTS);
		}
#pragma endregion


#pragma region 뼈
		// 정점에 대한 뼈정보 로드
		wstring strSkeletalName = Make_Wstring(pMesh->mBones[0]->mArmature->mName.C_Str());
		for (_uint j = 0; j < pMesh->mNumBones; j++)
		{
			// 뼈 없으면 넘어감. 이거 걸리면 안되는 거임.
			FBoneGroup* pBoneGroup = pModelData->pBoneGroup;
			if (nullptr == pBoneGroup)
				continue;

			// 뼈 찾기
			wstring strBoneName = Make_Wstring(pMesh->mBones[j]->mName.C_Str());
			FBoneData* pBoneData = pBoneGroup->Find_BoneData(strBoneName);

			_int iBoneID = pBoneData->iID;
			aiBone* pBone = pMesh->mBones[j];

			// 뼈 정보들 저장
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

		// 채워지지 않은 뼈 정보를 할당한다.
		for (_uint j = 0; j < pMesh->mNumVertices; j++)
		{
			pVecVertices[j].vecBoneID.resize(AI_LMW_MAX_WEIGHTS, 0);
			pVecVertices[j].vecWeights.resize(AI_LMW_MAX_WEIGHTS, 0.f);
		}
#pragma endregion


#pragma region 면
		// 면 (인덱싱)
		vecMeshes[i]->vecIndices.reserve(pMesh->mNumFaces);
		for (_uint j = 0; j < pMesh->mNumFaces; j++)
		{
			aiFace& face = pMesh->mFaces[j];
			vecMeshes[i]->vecIndices.push_back(face.mIndices[0]);
			vecMeshes[i]->vecIndices.push_back(face.mIndices[1]);
			vecMeshes[i]->vecIndices.push_back(face.mIndices[2]);
		}
#pragma endregion



#pragma region 머터리얼
		vecMeshes[i]->iMaterialIndex = pMesh->mMaterialIndex;
#pragma endregion


		/*for (_uint j = 0; j < pMesh->mNumVertices; j++)
		{
			wstringstream ss;
			ss << pVecVertices[j].vecBoneID.size();
			OutputDebugString(wstring(wstring(L"Vertex VecBoneSize : ") + ss.str() + L"\n").c_str());
		}*/
	}
}

void CModelMgr::Load_Material(FModelData* pModelData)
{
	// 머터리얼 그룹 생성
	FMaterialGroup* pMatGroup = pModelData->pMaterialGroup;

	// 머터리얼 로드
	for (_uint i = 0; i < m_pScene->mNumMaterials; i++)
	{
		aiMaterial* pAIMaterial = m_pScene->mMaterials[i];
		wstring matName = Make_Wstring(pAIMaterial->GetName().C_Str());

		// 머터리얼 컨테이너 생성
		FMaterialData* pMaterialData = FMaterialData::Create();
		pMatGroup->Add_Material(matName, pMaterialData);

		for (_uint j = 1; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			_char		szDrive[MAX_PATH] = "";
			_char		szDirectory[MAX_PATH] = "";

			_splitpath_s(m_strLoadFilePath.c_str(), szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

			aiString	strPath;
			if (FAILED(pAIMaterial->GetTexture(Cast<aiTextureType>(j), 0, &strPath)))
				continue;

			_char		szFileName[MAX_PATH] = "";
			_char		szEXT[MAX_PATH] = "";

			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);

			_char		szTmp[MAX_PATH] = "";
			strcpy_s(szTmp, szDrive);
			strcat_s(szTmp, szDirectory);
			strcat_s(szTmp, szFileName);
			strcat_s(szTmp, szEXT);

			_tchar		szFullPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szTmp, strlen(szTmp), szFullPath, MAX_PATH);

			// 머터리얼에 데이터 저장
			pMaterialData->strTexture[j] = szFullPath;
		}
	}
}

void CModelMgr::Load_Animation(FModelData* pModelData)
{
	FBoneAnimGroup* pAnimGroup = pModelData->pAnimGroup;
	FBoneGroup* pBoneGroup = pModelData->pBoneGroup;

	if (!pAnimGroup || !pBoneGroup)
		return;

	// 애니메이션
	for (_uint i = 0; i < m_pScene->mNumAnimations; i++)
	{
		aiAnimation* pAnimAI = m_pScene->mAnimations[i];

		// 애님 데이터 생성
		FBoneAnimData* pAnimData = FBoneAnimData::Create();
		wstring AnimNameWithTK = Make_Wstring(pAnimAI->mName.C_Str());
		pAnimData->fDuration = pAnimAI->mDuration;
		pAnimData->fTickPerSecond = pAnimAI->mTicksPerSecond;

		// 애니메이션 이름 추출
		size_t iTokkenInd = AnimNameWithTK.find_first_of(L'|') + Cast<size_t>(1);
		wstring SkeletalName = AnimNameWithTK.substr(0, iTokkenInd - 1);
		wstring AnimName = AnimNameWithTK.substr(iTokkenInd);



		// 애니메이션 채널
		for (_uint j = 0; j < pAnimAI->mNumChannels; j++)
		{
			aiNodeAnim* pNodeAnimAI = pAnimAI->mChannels[j];
			FBoneAnimChannelData* pAnimNodeData = FBoneAnimChannelData::Create();
			wstring AnimNodeName = Make_Wstring(pNodeAnimAI->mNodeName.C_Str());

			// 뼈 없으면 넘어감. 이거 걸리면 안되는 거임.
			FBoneGroup* pBoneGroup = pModelData->pBoneGroup;
			if (nullptr == pBoneGroup)
				continue;
			FBoneData* pBoneData = pBoneGroup->Find_BoneData(AnimNodeName);

			pAnimNodeData->iBoneID = pBoneData->iID;
			pAnimNodeData->vecPositions.reserve(pNodeAnimAI->mNumPositionKeys);
			pAnimNodeData->vecRotations.reserve(pNodeAnimAI->mNumRotationKeys);
			pAnimNodeData->vecScales.reserve(pNodeAnimAI->mNumScalingKeys);

			// Pos 값 추출
			for (_uint k = 0; k < pNodeAnimAI->mNumPositionKeys; k++)
			{
				auto fTime = pNodeAnimAI->mPositionKeys[k].mTime;
				auto vPos = pNodeAnimAI->mPositionKeys[k].mValue;
				FBoneAnimChannelData::FPosition tPosition = {
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
				FBoneAnimChannelData::FRotation tRotation = {
					Cast<_float>(fTime),
					{ vRot.x, vRot.y, vRot.z, vRot.w }
				};
				pAnimNodeData->vecRotations.push_back(tRotation);
			}

			// Scale 값 추출
			for (_uint k = 0; k < pNodeAnimAI->mNumScalingKeys; k++)
			{
				auto fTime = pNodeAnimAI->mScalingKeys[k].mTime;
				auto vScale = pNodeAnimAI->mScalingKeys[k].mValue;
				FBoneAnimChannelData::FScale tScale = {
					Cast<_float>(fTime),
					{ vScale.x, vScale.y, vScale.z }
				};
				pAnimNodeData->vecScales.push_back(tScale);
			}

			pAnimData->Add_AnimNodeData(AnimNodeName, pAnimNodeData);
		}
		pAnimGroup->Add_BoneAnim(AnimName, pAnimData);

		int t = 1;
	}
}

void CModelMgr::Load_Hierarchi(FBoneGroup* pBoneGroup,  aiNode* pRootNode)
{
	if (pRootNode)
	{
		wstring strKey = Make_Wstring(pRootNode->mName.C_Str());
		m_iNodeID = 0;

		// ID 지정
		FBoneData* pRootNodeData = FBoneData::Create();
		pRootNodeData->iID = m_iNodeID++;
		pRootNodeData->strName = strKey;
		pRootNodeData->iParentID = -1;
		pRootNodeData->matOffset = ConvertAiMatrix_ToDXMatrix(pRootNode->mTransformation);		// 스켈레탈은 일반 행렬이 중요하다.
		pRootNodeData->matTransform = pRootNodeData->matOffset;

		pBoneGroup->Add_BoneData(strKey, pRootNodeData);

		for (_uint i = 0; i < pRootNode->mNumChildren; i++)
		{
			Load_HierarchiNode(pBoneGroup, pRootNode->mChildren[i], pRootNodeData);
		}
	}

	m_pRootArmature = nullptr;
}

void CModelMgr::Load_HierarchiNode(FBoneGroup* pBoneGroup, aiNode* pBoneNode, FBoneData* pParentNode)
{
	if (pBoneNode)
	{
		wstring strNodeKey = Make_Wstring(pBoneNode->mName.C_Str());

		FBoneData* pNodeData = FBoneData::Create();
		pNodeData->iID = m_iNodeID++;
		pNodeData->strName = strNodeKey;
		pNodeData->iParentID = pParentNode->iID;
		pNodeData->matOffset = ConvertAiMatrix_ToDXMatrix(pBoneNode->mTransformation);	// 뼈는 역행렬이 중요하다.
		//XMStoreFloat4x4(&pNodeData->matOffset, XMMatrixInverse(nullptr, XMLoadFloat4x4(&pNodeData->matOffset)));
		XMStoreFloat4x4(&pNodeData->matTransform, XMMatrixIdentity());
		pBoneGroup->Add_BoneData(strNodeKey, pNodeData);

		//_float4x4 matTemp = {};
		//XMStoreFloat4x4(&matTemp, XMLoadFloat4x4(&pNodeData->matTransform));// * XMLoadFloat4x4(&pNodeData->matTransform));

		//_float3 vPos = Get_PosFromMatrix(matTemp);
		//_float3 vRot = Get_RotEulerFromMatrix(matTemp);
		//_float3 vScale = Get_ScaleFromMatrix(matTemp);

		for (_uint i = 0; i < pBoneNode->mNumChildren; i++)
		{
			Load_HierarchiNode(pBoneGroup, pBoneNode->mChildren[i], pNodeData);
		}
	}
}

const FMeshData* const CModelMgr::Find_MeshData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const wstring& strMeshKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strModelKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	return (*iter).second->pMeshGroup->Find_Mesh(strMeshKey);
}

FBoneGroup* CModelMgr::Clone_BoneGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strModelKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	FModelData* pModelGroup = (*iter).second;
	return pModelGroup->pBoneGroup->Clone();
}

const FModelData* const CModelMgr::Find_ModelData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strModelKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	return (*iter).second;
}

FModelData* CModelMgr::Add_ModelData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strModelKey);
	if (iter != m_mapModelDatas[iIndex].end())
		return (*iter).second;
	
	FModelData* pGroup = FModelData::Create(false);
	m_mapModelDatas[iIndex].emplace(strModelKey, pGroup);

	return pGroup;
}

FMeshGroup* CModelMgr::Find_MeshGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strModelKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	return (*iter).second->pMeshGroup;
}

FBoneGroup* CModelMgr::Find_BoneGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strModelKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	return (*iter).second->pBoneGroup;
}

FBoneAnimGroup* CModelMgr::Find_AnimGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strModelKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	return (*iter).second->pAnimGroup;
}

FMaterialGroup* CModelMgr::Find_MaterialGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strModelKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	return (*iter).second->pMaterialGroup;
}

_float4x4 CModelMgr::ConvertAiMatrix_ToDXMatrix(aiMatrix4x4& matrix)
{
	_float4x4	matOffsetTG = _float4x4(
		matrix.a1, matrix.b1, matrix.c1, matrix.d1,
		matrix.a2, matrix.b2, matrix.c2, matrix.d2,
		matrix.a3, matrix.b3, matrix.c3, matrix.d3,
		matrix.a4, matrix.b4, matrix.c4, matrix.d4
	);

	return matOffsetTG;
}


