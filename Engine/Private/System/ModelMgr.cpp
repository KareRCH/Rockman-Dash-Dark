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
	//iFlag = aiProcess_JoinIdenticalVertices |   // ������ ������ ����, �ε��� ����ȭ
	//	aiProcess_ValidateDataStructure |       // �δ��� ����� ����
	//	aiProcess_ImproveCacheLocality |        // ��� ������ ĳ����ġ�� ����
	//	aiProcess_RemoveRedundantMaterials |    // �ߺ��� ���͸��� ����
	//	aiProcess_GenUVCoords |                 // ����, ������, ���� �� ��� ������ ������ UV�� ��ȯ
	//	aiProcess_TransformUVCoords |           // UV ��ȯ ó���� (�����ϸ�, ��ȯ...)
	//	aiProcess_FindInstances |               // �ν��Ͻ��� �Ž��� �˻��Ͽ� �ϳ��� �����Ϳ� ���� ������ ����
	//	aiProcess_LimitBoneWeights |            // ������ ���� ����ġ�� �ִ� 4���� ����
	//	aiProcess_OptimizeMeshes |              // ������ ��� ���� �Ž��� ����
	//	aiProcess_GenSmoothNormals |            // �ε巯�� �븻����(��������) ����
	//	aiProcess_SplitLargeMeshes |			// �Ŵ��� �ϳ��� �Ž��� �����Ž���� ��Ȱ(����)
	//	aiProcess_Triangulate |                 // 3�� �̻��� �𼭸��� ���� �ٰ��� ���� �ﰢ������ ����(����)
	//	aiProcess_ConvertToLeftHanded |         // D3D�� �޼���ǥ��� ��ȯ
	//	aiProcess_PopulateArmatureData |		// Aramature�� ������ ���� ����� �� �ְ� �ϴ� �÷���
	//	aiProcess_SortByPType;					// ����Ÿ���� ������Ƽ��� ������ '������' �Ž��� ����
	
	m_strLoadFilePath = strFileName;
	m_pScene = importer.ReadFile((m_strMainDir + strFileName).c_str(), iFlag);

	if (!m_pScene || m_pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_pScene->mRootNode)
	{
#ifdef _DEBUG
		cout << "�� �� �ε� ����" << endl;
#endif
		return;
	}
	

	FModelData* pModelData = Add_ModelData(eGroupIndex, strModelKey);	// �� �׷� �ε�, ������ �߰��� �ε�
	


	// �޽�, ��, ���� �ε�
	Load_Hierarchi(pModelData->pBoneGroup, m_pScene->mRootNode);

	Load_Material(pModelData);

	Load_Mesh(pModelData);

	// �ִϸ��̼� �ε�
	Load_Animation(pModelData);

	importer.FreeScene();
}

void CModelMgr::Load_Mesh(FModelData* pModelData)
{
	// �׽�Ʈ�� ��Ÿ������
	/*vector<pair<aiString*, aiMetadataEntry*>> pMetaDatas;
	pMetaDatas.reserve(m_pScene->mMetaData->mNumProperties);
	for (_uint i = 0; i < m_pScene->mMetaData->mNumProperties; i++)
	{
		aiMetadata* pMeta = m_pScene->mMetaData;
		aiString* str = &pMeta->mKeys[i];
		aiMetadataEntry* pEntry = &pMeta->mValues[i];
		pMetaDatas.push_back({ str, pEntry });
	}*/

	// ���������� ����� ��ȯ�Ǿ� ������ ������ �θ� �Ǵ� Skeletal�� ����� ���� �ʴ´�.
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

	// ����� �޽� ����, �޽� ���� ��ŭ �����
	vector<FMeshData*>	vecMeshes;
	vecMeshes.resize(m_pScene->mNumMeshes);
	for (size_t i = 0; i < vecMeshes.size(); i++)
	{
		vecMeshes[i] = FMeshData::Create();
		vecMeshes[i]->iID = Cast<_uint>(i);
	}

	FMeshGroup* pMeshGroup = pModelData->pMeshGroup;
	aiNode* pRootNode = m_pScene->mRootNode;

	// �޽� �ε�
	for (_uint i = 0; i < m_pScene->mNumMeshes; i++)
	{
		aiNode* pChildNode = pRootNode->mChildren[i];
		aiMesh* pMesh = m_pScene->mMeshes[i];

		// MeshKey ���� �� ����
		wstring strMeshName = Make_Wstring(pMesh->mName.C_Str());
		pMeshGroup->Add_Mesh(strMeshName, vecMeshes[i]);

		// Ʈ������ ����
		aiMatrix4x4 aiTransform = pChildNode->mTransformation;
		vecMeshes[i]->matOffset = ConvertAiMatrix_ToDXMatrix(aiTransform);

		// ���� ���� �� ��Ʈ ��带 �����Ѵ�. ������ �ε��ϴµ� ���δ�.
#pragma region ��
		// ��
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

			// �� ������ ���� ���� Ȯ��
			pVecVertices[iVertexIndex].vecBoneID.reserve(AI_LMW_MAX_WEIGHTS);
			pVecVertices[iVertexIndex].vecWeights.reserve(AI_LMW_MAX_WEIGHTS);
		}
#pragma endregion


#pragma region ��
		// ������ ���� ������ �ε�
		wstring strSkeletalName = Make_Wstring(pMesh->mBones[0]->mArmature->mName.C_Str());
		for (_uint j = 0; j < pMesh->mNumBones; j++)
		{
			// �� ������ �Ѿ. �̰� �ɸ��� �ȵǴ� ����.
			FBoneGroup* pBoneGroup = pModelData->pBoneGroup;
			if (nullptr == pBoneGroup)
				continue;

			// �� ã��
			wstring strBoneName = Make_Wstring(pMesh->mBones[j]->mName.C_Str());
			FBoneData* pBoneData = pBoneGroup->Find_BoneData(strBoneName);

			_int iBoneID = pBoneData->iID;
			aiBone* pBone = pMesh->mBones[j];

			// �� ������ ����
			for (_uint k = 0; k < pBone->mNumWeights; k++)
			{
				_uint iVertexID = pBone->mWeights[k].mVertexId;
				_float fWeight = pBone->mWeights[k].mWeight;

				// ���� �����Ϳ� ��ID�� ���� ���� ���
				if (fWeight != 0.f)
				{
					pVecVertices[iVertexID].vecBoneID.push_back(iBoneID);
					pVecVertices[iVertexID].vecWeights.push_back(fWeight);
				}
			}

			++iBoneID;
		}

		// ä������ ���� �� ������ �Ҵ��Ѵ�.
		for (_uint j = 0; j < pMesh->mNumVertices; j++)
		{
			pVecVertices[j].vecBoneID.resize(AI_LMW_MAX_WEIGHTS, 0);
			pVecVertices[j].vecWeights.resize(AI_LMW_MAX_WEIGHTS, 0.f);
		}
#pragma endregion


#pragma region ��
		// �� (�ε���)
		vecMeshes[i]->vecIndices.reserve(pMesh->mNumFaces);
		for (_uint j = 0; j < pMesh->mNumFaces; j++)
		{
			aiFace& face = pMesh->mFaces[j];
			vecMeshes[i]->vecIndices.push_back(face.mIndices[0]);
			vecMeshes[i]->vecIndices.push_back(face.mIndices[1]);
			vecMeshes[i]->vecIndices.push_back(face.mIndices[2]);
		}
#pragma endregion



#pragma region ���͸���
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
	// ���͸��� �׷� ����
	FMaterialGroup* pMatGroup = pModelData->pMaterialGroup;

	// ���͸��� �ε�
	for (_uint i = 0; i < m_pScene->mNumMaterials; i++)
	{
		aiMaterial* pAIMaterial = m_pScene->mMaterials[i];
		wstring matName = Make_Wstring(pAIMaterial->GetName().C_Str());

		// ���͸��� �����̳� ����
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

			// ���͸��� ������ ����
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

	// �ִϸ��̼�
	for (_uint i = 0; i < m_pScene->mNumAnimations; i++)
	{
		aiAnimation* pAnimAI = m_pScene->mAnimations[i];

		// �ִ� ������ ����
		FBoneAnimData* pAnimData = FBoneAnimData::Create();
		wstring AnimNameWithTK = Make_Wstring(pAnimAI->mName.C_Str());
		pAnimData->fDuration = pAnimAI->mDuration;
		pAnimData->fTickPerSecond = pAnimAI->mTicksPerSecond;

		// �ִϸ��̼� �̸� ����
		size_t iTokkenInd = AnimNameWithTK.find_first_of(L'|') + Cast<size_t>(1);
		wstring SkeletalName = AnimNameWithTK.substr(0, iTokkenInd - 1);
		wstring AnimName = AnimNameWithTK.substr(iTokkenInd);



		// �ִϸ��̼� ä��
		for (_uint j = 0; j < pAnimAI->mNumChannels; j++)
		{
			aiNodeAnim* pNodeAnimAI = pAnimAI->mChannels[j];
			FBoneAnimChannelData* pAnimNodeData = FBoneAnimChannelData::Create();
			wstring AnimNodeName = Make_Wstring(pNodeAnimAI->mNodeName.C_Str());

			// �� ������ �Ѿ. �̰� �ɸ��� �ȵǴ� ����.
			FBoneGroup* pBoneGroup = pModelData->pBoneGroup;
			if (nullptr == pBoneGroup)
				continue;
			FBoneData* pBoneData = pBoneGroup->Find_BoneData(AnimNodeName);

			pAnimNodeData->iBoneID = pBoneData->iID;
			pAnimNodeData->vecPositions.reserve(pNodeAnimAI->mNumPositionKeys);
			pAnimNodeData->vecRotations.reserve(pNodeAnimAI->mNumRotationKeys);
			pAnimNodeData->vecScales.reserve(pNodeAnimAI->mNumScalingKeys);

			// Pos �� ����
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

			// Rot �� ���� (���ʹϾ�)
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

			// Scale �� ����
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

		// ID ����
		FBoneData* pRootNodeData = FBoneData::Create();
		pRootNodeData->iID = m_iNodeID++;
		pRootNodeData->strName = strKey;
		pRootNodeData->iParentID = -1;
		pRootNodeData->matOffset = ConvertAiMatrix_ToDXMatrix(pRootNode->mTransformation);		// ���̷�Ż�� �Ϲ� ����� �߿��ϴ�.
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
		pNodeData->matOffset = ConvertAiMatrix_ToDXMatrix(pBoneNode->mTransformation);	// ���� ������� �߿��ϴ�.
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


