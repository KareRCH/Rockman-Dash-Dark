#include "System/ModelMgr.h"

FModelGroup* FModelGroup::Create(const _bool bLoaded)
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FModelGroup Create Failed");

		return nullptr;
	}

	pInstance->pMeshGroup = FMeshGroup::Create();
	pInstance->pBoneGroup = FBoneGroup::Create();
	pInstance->pAnimGroup = FAnimGroup::Create();
	pInstance->pModelNodeGroup = FModelNodeGroup::Create();
	pInstance->bLoaded = bLoaded;

	return pInstance;
}

void FModelGroup::Free()
{
	Safe_Release(pMeshGroup);
	Safe_Release(pBoneGroup);
	Safe_Release(pAnimGroup);
	Safe_Release(pModelNodeGroup);
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
	for (_uint i = 0; i < Cast_EnumDef(EModelGroupIndex::Size); i++)
	{
		for (auto& Pair : m_mapModelGroup[i])
		{
			Safe_Release(Pair.second);
		}
		m_mapModelGroup[i].clear();
	}
	
}

void CModelMgr::Load_Model(const EModelGroupIndex eGroupIndex, const string& strFileName, const wstring& strGroupKey)
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
		aiProcess_PopulateArmatureData |		// Aramature�� ������ ���� ����� �� �ְ� �ϴ� �÷���
		aiProcess_SortByPType;					// ����Ÿ���� ������Ƽ��� ������ '������' �Ž��� ����
	
	m_pScene = importer.ReadFile((m_strMainDir + strFileName).c_str(), iFlag);

	if (!m_pScene || m_pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_pScene->mRootNode)
	{
#ifdef _DEBUG
		cout << "�� �� �ε� ����" << endl;
#endif
		return;
	}
	

	FModelGroup* pModelGroup = Add_ModelGroup(eGroupIndex, strGroupKey);	// �� �׷� �ε�, ������ �߰��� �ε�
	
	// �޽�, ��, ���� �ε�
	Load_MeshBoneMaterial(pModelGroup);

	// �ִϸ��̼� �ε�
	Load_Anim(pModelGroup->pAnimGroup);

	// ���� �ε�
	Load_Hierarchi(pModelGroup->pModelNodeGroup, m_pRootArmature);
}

void CModelMgr::Load_MeshBoneMaterial(FModelGroup* pModelGroup)
{
	// �ӽ� �޽� ����
	m_vecMesh.resize(m_pScene->mNumMeshes);
	for (size_t i = 0; i < m_vecMesh.size(); i++)
		m_vecMesh[i] = FMeshData::Create();

	m_iMaterialCount = m_pScene->mNumMaterials;
	m_iBoneCount = 0;

	FMeshGroup* pMeshGroup = pModelGroup->pMeshGroup;
	FBoneGroup* pBoneGroup = pModelGroup->pBoneGroup;

	_float4x4 matTransformTest = {};
	aiNode* pRootNode = m_pScene->mRootNode;

	// �׽�Ʈ�� ��Ÿ������
	vector<pair<aiString*, aiMetadataEntry*>> pMetaDatas;
	pMetaDatas.reserve(m_pScene->mMetaData->mNumProperties);
	for (_uint i = 0; i < m_pScene->mMetaData->mNumProperties; i++)
	{
		aiMetadata* pMeta = m_pScene->mMetaData;
		aiString* str = &pMeta->mKeys[i];
		aiMetadataEntry* pEntry = &pMeta->mValues[i];
		pMetaDatas.push_back({ str, pEntry });
	}

	// ���������� ����� ��ȯ�Ǿ� ������ ������ �θ� �Ǵ� Skeletal�� ����� ���� �ʴ´�.
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

		// Ʈ������ ����
		aiMatrix4x4 aiTransform = pChildNode->mTransformation;
		m_vecMesh[i]->matTransform = matTest;//ConvertAiMatrix_ToDXMatrix(aiTransform);

		// ��
		vector<FMeshVertexData>& pVecVertices = m_vecMesh[i]->vecVertices;
		pVecVertices.resize(pMesh->mNumVertices);
		for (_uint j = 0; j < pMesh->mNumVertices; j++)
		{
			_uint iVertexIndex = j;

			_float3 vPos(&pMesh->mVertices[j].x);
			_float3 vNormal(&pMesh->mNormals[j].x);
			_float2 vTexCoord;

			if (pMesh->HasTextureCoords(0))
				vTexCoord = _float2(&pMesh->mTextureCoords[0][j].x);
			else
				vTexCoord = _float2(0.f, 0.f);

			m_vecMesh[i]->vecVertices[iVertexIndex] = { vPos, vNormal, vTexCoord };

			// �� ������ ���� ���� Ȯ��
			pVecVertices[iVertexIndex].vecBoneID.reserve(AI_LMW_MAX_WEIGHTS);
			pVecVertices[iVertexIndex].vecWeights.reserve(AI_LMW_MAX_WEIGHTS);
		}

		// ��
		// ������ ���� ������ �ε�
		for (_uint j = 0; j < pMesh->mNumBones; j++)
		{
			_int iBoneID = j;
			aiBone* pBone = pMesh->mBones[j];
			//FBoneData* pBoneData = FBoneData::Create();

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

			//pBoneGroup->Add_Bone(Make_Wstring(pBone->mName.C_Str()), pBoneData);
			++iBoneID;
		}

		/*for (_uint j = 0; j < pMesh->mNumVertices; j++)
		{
			wstringstream ss;
			ss << pVecVertices[j].vecBoneID.size();
			OutputDebugString(wstring(wstring(L"Vertex VecBoneSize : ") + ss.str() + L"\n").c_str());
		}*/

		// ���� ���� �� ��Ʈ ��带 �����Ѵ�. ���߿� ������ �ε��ϴµ� ���δ�.
		if (pMesh->HasBones())
		{
			m_pRootArmature = pMesh->mBones[0]->mArmature;
		}

		// �� (�ε���)
		for (_uint j = 0; j < pMesh->mNumFaces; j++)
		{
			aiFace& face = pMesh->mFaces[j];
			m_vecMesh[i]->vecIndices.push_back(face.mIndices[0]);
			m_vecMesh[i]->vecIndices.push_back(face.mIndices[1]);
			m_vecMesh[i]->vecIndices.push_back(face.mIndices[2]);
		}

		// ���͸���
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

		// �Ľ̵����� ����Ʈ
		{
			// �޽� ����
			FMeshGroup* pMeshGroup = pModelGroup->pMeshGroup;

			// MeshKey ���� �� ����
			pMeshGroup->Add_Mesh(Make_Wstring(pMesh->mName.C_Str()), m_vecMesh[i]);
		}
	}

	// �ӽ� �޽� ����
	m_vecMesh.clear();
}

void CModelMgr::Load_Anim(FAnimGroup* pAnimGroup)
{
	if (!pAnimGroup)
		return;

	// �ִϸ��̼�
	for (_uint i = 0; i < m_pScene->mNumAnimations; i++)
	{
		aiAnimation* pAnimAI = m_pScene->mAnimations[i];

		// �ִ� ������ ����
		FAnimData* pAnimData = FAnimData::Create();
		wstring AnimNameWithTK = Make_Wstring(pAnimAI->mName.C_Str());

		// �ִϸ��̼� �̸� ����
		size_t iTokkenInd = AnimNameWithTK.find_first_of(L'|') + Cast<size_t>(1);
		wstring AnimName = AnimNameWithTK.substr(iTokkenInd);

		// �ִϸ��̼� ä��
		for (_uint j = 0; j < pAnimAI->mNumChannels; j++)
		{
			aiNodeAnim* pNodeAnimAI = pAnimAI->mChannels[j];
			FAnimNodeData* pAnimNodeData = FAnimNodeData::Create();
			wstring AnimNodeName = Make_Wstring(pNodeAnimAI->mNodeName.C_Str());

			pAnimNodeData->vecPositions.reserve(pNodeAnimAI->mNumPositionKeys);
			pAnimNodeData->vecRotations.reserve(pNodeAnimAI->mNumRotationKeys);
			pAnimNodeData->vecScales.reserve(pNodeAnimAI->mNumScalingKeys);

			// Pos �� ����
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

			// Rot �� ���� (���ʹϾ�)
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

			// Scale �� ����
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

void CModelMgr::Load_Hierarchi(FModelNodeGroup* pModelNodeGroup,  aiNode* pArmatureNode)
{
	if (pArmatureNode)
	{
		FModelNodeData* pRootNodeData = FModelNodeData::Create();
		pRootNodeData->strname = Make_Wstring(pArmatureNode->mName.C_Str());
		pRootNodeData->matOffset = ConvertAiMatrix_ToDXMatrix(pArmatureNode->mTransformation);
		pRootNodeData->matTransform = pRootNodeData->matOffset;
		pModelNodeGroup->mapModelNodeData.emplace(pRootNodeData->strname, pRootNodeData);

		for (_uint i = 0; i < pArmatureNode->mNumChildren; i++)
		{
			Load_HierarchiNode(pArmatureNode->mChildren[i], pRootNodeData);
		}
	}

	m_pRootArmature = nullptr;
}

void CModelMgr::Load_HierarchiNode(aiNode* pBoneNode, FModelNodeBaseData* pRootNode)
{
	if (pBoneNode)
	{
		FModelNodeData* pRootNodeData = FModelNodeData::Create();
		pRootNodeData->strname = Make_Wstring(pBoneNode->mName.C_Str());
		pRootNodeData->matOffset = ConvertAiMatrix_ToDXMatrix(pBoneNode->mTransformation);
		pRootNodeData->matTransform = pRootNodeData->matOffset;
		pRootNode->vecChildren.push_back(pRootNodeData);

		for (_uint i = 0; i < pBoneNode->mNumChildren; i++)
		{
			Load_HierarchiNode(pBoneNode->mChildren[i], pRootNodeData);
		}
	}
}

const FMeshData* const CModelMgr::Get_Mesh(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey, const wstring& strMeshKey)
{
	_uint iIndex = Cast_EnumDef(eGroupIndex);

	auto iter = m_mapModelGroup[iIndex].find(strGroupKey);
	if (iter == m_mapModelGroup[iIndex].end())
		return nullptr;

	return (*iter).second->pMeshGroup->Get_Mesh(strMeshKey);
}

FModelGroup* CModelMgr::Get_ModelGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey)
{
	_uint iIndex = Cast_EnumDef(eGroupIndex);

	auto iter = m_mapModelGroup[iIndex].find(strGroupKey);
	if (iter == m_mapModelGroup[iIndex].end())
		return nullptr;

	return (*iter).second;
}

FModelGroup* CModelMgr::Add_ModelGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey)
{
	_uint iIndex = Cast_EnumDef(eGroupIndex);

	auto iter = m_mapModelGroup[iIndex].find(strGroupKey);
	if (iter != m_mapModelGroup[iIndex].end())
		return (*iter).second;
	
	FModelGroup* pGroup = FModelGroup::Create(false);
	m_mapModelGroup[iIndex].emplace(strGroupKey, pGroup);

	return pGroup;
}

FMeshGroup* CModelMgr::Get_MeshGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey)
{
	_uint iIndex = Cast_EnumDef(eGroupIndex);

	auto iter = m_mapModelGroup[iIndex].find(strGroupKey);
	if (iter == m_mapModelGroup[iIndex].end())
		return nullptr;

	return (*iter).second->pMeshGroup;
}

FBoneGroup* CModelMgr::Get_BoneGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey)
{
	_uint iIndex = Cast_EnumDef(eGroupIndex);

	auto iter = m_mapModelGroup[iIndex].find(strGroupKey);
	if (iter == m_mapModelGroup[iIndex].end())
		return nullptr;

	return (*iter).second->pBoneGroup;
}

FAnimGroup* CModelMgr::Get_AnimGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey)
{
	_uint iIndex = Cast_EnumDef(eGroupIndex);

	auto iter = m_mapModelGroup[iIndex].find(strGroupKey);
	if (iter == m_mapModelGroup[iIndex].end())
		return nullptr;

	return (*iter).second->pAnimGroup;
}

FModelNodeGroup* CModelMgr::Get_ModelNodeGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey)
{
	_uint iIndex = Cast_EnumDef(eGroupIndex);

	auto iter = m_mapModelGroup[iIndex].find(strGroupKey);
	if (iter == m_mapModelGroup[iIndex].end())
		return nullptr;

	return (*iter).second->pModelNodeGroup;
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

_float3 CModelMgr::Calculate_InterpolatedFloat3(_float fAnimTime, const _int iNumKeys, const _vec vVectorKey)
{


	return _float3();
}

_float4 CModelMgr::Calculate_InterpolatedQuaternion(_float fAnimTime, const _int iNumKeys, const _vec vVectorKey)
{
	return _float4();
}


