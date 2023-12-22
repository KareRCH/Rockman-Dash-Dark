#include "System/ModelMgr.h"

#include "ModelLoad.h"

HRESULT CModelMgr::Initialize(const wstring& strMainDir)
{
	m_strMainDir = strMainDir;

	return S_OK;
}

CModelMgr* CModelMgr::Create(const wstring& strMainDir)
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



//void CModelMgr::Load_Model(const EModelGroupIndex eGroupIndex, const string& strFileName, const wstring& strModelKey)
//{
//	Assimp::Importer importer;
//	
//	_uint iFlag = 0;
//	iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
//	//iFlag = aiProcess_JoinIdenticalVertices |   // ������ ������ ����, �ε��� ����ȭ
//	//	aiProcess_ValidateDataStructure |       // �δ��� ����� ����
//	//	aiProcess_ImproveCacheLocality |        // ��� ������ ĳ����ġ�� ����
//	//	aiProcess_RemoveRedundantMaterials |    // �ߺ��� ���͸��� ����
//	//	aiProcess_GenUVCoords |                 // ����, ������, ���� �� ��� ������ ������ UV�� ��ȯ
//	//	aiProcess_TransformUVCoords |           // UV ��ȯ ó���� (�����ϸ�, ��ȯ...)
//	//	aiProcess_FindInstances |               // �ν��Ͻ��� �Ž��� �˻��Ͽ� �ϳ��� �����Ϳ� ���� ������ ����
//	//	aiProcess_LimitBoneWeights |            // ������ ���� ����ġ�� �ִ� 4���� ����
//	//	aiProcess_OptimizeMeshes |              // ������ ��� ���� �Ž��� ����
//	//	aiProcess_GenSmoothNormals |            // �ε巯�� �븻����(��������) ����
//	//	aiProcess_SplitLargeMeshes |			// �Ŵ��� �ϳ��� �Ž��� �����Ž���� ��Ȱ(����)
//	//	aiProcess_Triangulate |                 // 3�� �̻��� �𼭸��� ���� �ٰ��� ���� �ﰢ������ ����(����)
//	//	aiProcess_ConvertToLeftHanded |         // D3D�� �޼���ǥ��� ��ȯ
//	//	aiProcess_PopulateArmatureData |		// Aramature�� ������ ���� ����� �� �ְ� �ϴ� �÷���
//	//	aiProcess_SortByPType;					// ����Ÿ���� ������Ƽ��� ������ '������' �Ž��� ����
//	
//	m_strLoadFilePath = strFileName;
//	m_pScene = importer.ReadFile((m_strMainDir + strFileName).c_str(), iFlag);
//
//	if (!m_pScene || m_pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_pScene->mRootNode)
//	{
//#ifdef _DEBUG
//		cout << "�� �� �ε� ����" << endl;
//#endif
//		return;
//	}
//	
//
//	FModelData* pModelData = Add_ModelData(eGroupIndex, strModelKey);	// �� �׷� �ε�, ������ �߰��� �ε�
//	
//
//
//	// �޽�, ��, ���� �ε�
//	Load_Hierarchi(pModelData->pBoneGroup, m_pScene->mRootNode);
//
//	Load_Material(pModelData);
//
//	Load_Mesh(pModelData);
//
//	// �ִϸ��̼� �ε�
//	Load_Animation(pModelData);
//
//	importer.FreeScene();
//}
//
//void CModelMgr::Load_Mesh(FModelData* pModelData)
//{
//	// �׽�Ʈ�� ��Ÿ������
//	/*vector<pair<aiString*, aiMetadataEntry*>> pMetaDatas;
//	pMetaDatas.reserve(m_pScene->mMetaData->mNumProperties);
//	for (_uint i = 0; i < m_pScene->mMetaData->mNumProperties; i++)
//	{
//		aiMetadata* pMeta = m_pScene->mMetaData;
//		aiString* str = &pMeta->mKeys[i];
//		aiMetadataEntry* pEntry = &pMeta->mValues[i];
//		pMetaDatas.push_back({ str, pEntry });
//	}*/
//
//	// ���������� ����� ��ȯ�Ǿ� ������ ������ �θ� �Ǵ� Skeletal�� ����� ���� �ʴ´�.
//	/*vector<_float4x4> vecMatrixTest;
//	_float4x4 matTest = {};
//	vecMatrixTest.reserve(pRootNode->mNumChildren);
//	for (_uint i = 0; i < pRootNode->mNumChildren; i++)
//	{
//		aiNode* pChildNode = pRootNode->mChildren[i];
//		aiMatrix4x4 aiTransform = pChildNode->mTransformation;
//		vecMatrixTest.push_back(ConvertAiMatrix_ToDXMatrix(aiTransform));
//		matTest = ConvertAiMatrix_ToDXMatrix(aiTransform);
//	}*/
//
//	// ����� �޽� ����, �޽� ���� ��ŭ �����
//	vector<FMeshData*>	vecMeshes;
//	vecMeshes.resize(m_pScene->mNumMeshes);
//	for (size_t i = 0; i < vecMeshes.size(); i++)
//	{
//		vecMeshes[i] = FMeshData::Create();
//		vecMeshes[i]->iID = Cast<_uint>(i);
//	}
//
//	FMeshGroup* pMeshGroup = pModelData->pMeshGroup;
//	aiNode* pRootNode = m_pScene->mRootNode;
//
//	// �޽� �ε�
//	for (_uint i = 0; i < m_pScene->mNumMeshes; i++)
//	{
//		aiNode* pChildNode = pRootNode->mChildren[i];
//		aiMesh* pMesh = m_pScene->mMeshes[i];
//
//		// MeshKey ���� �� ����
//		wstring strMeshName = Make_Wstring(pMesh->mName.C_Str());
//		pMeshGroup->Add_Mesh(strMeshName, vecMeshes[i]);
//
//		// ���� ���� �� ��Ʈ ��带 �����Ѵ�. ������ �ε��ϴµ� ���δ�.
//#pragma region ��
//		// ��
//		vector<FMeshVertexData>& pVecVertices = vecMeshes[i]->vecVertices;
//		pVecVertices.resize(pMesh->mNumVertices);
//		for (_uint j = 0; j < pMesh->mNumVertices; j++)
//		{
//			_uint iVertexIndex = j;
//
//			_float3 vPos(&pMesh->mVertices[j].x);
//			_float3 vNormal(&pMesh->mNormals[j].x);
//			_float2 vTexCoord;
//			_float3 vTangent, vBiTangent;
//
//			if (pMesh->HasTextureCoords(0))
//				vTexCoord = _float2(&pMesh->mTextureCoords[0][j].x);
//			else
//				vTexCoord = _float2(0.f, 0.f);
//
//			if (pMesh->HasTangentsAndBitangents())
//			{
//				vTangent = _float3(&pMesh->mTangents[j].x);
//				vBiTangent = _float3(&pMesh->mBitangents[j].x);
//			}
//			else
//				vTangent = vBiTangent = _float3(0.f, 0.f, 0.f);
//
//			vecMeshes[i]->vecVertices[iVertexIndex] = { vPos, vNormal, vTexCoord, vTangent, vBiTangent };
//
//			// �� ������ ���� ���� Ȯ��
//			pVecVertices[iVertexIndex].vecBoneID.reserve(AI_LMW_MAX_WEIGHTS);
//			pVecVertices[iVertexIndex].vecWeights.reserve(AI_LMW_MAX_WEIGHTS);
//		}
//#pragma endregion
//
//
//#pragma region ��
//		// ������ ���� ������ �ε�
//		vecMeshes[i]->vecMeshBoneDatas.reserve(pMesh->mNumBones);
//		for (_uint j = 0; j < pMesh->mNumBones; j++)
//		{
//			// �� ������ �Ѿ. �̰� �ɸ��� �ȵǴ� ����.
//			FBoneGroup* pBoneGroup = pModelData->pBoneGroup;
//			if (nullptr == pBoneGroup)
//				continue;
//
//			// �� ã��
//			wstring strBoneName = Make_Wstring(pMesh->mBones[j]->mName.C_Str());
//			FBoneData* pBoneData = pBoneGroup->Find_BoneData(strBoneName);
//
//			// ���� ��İ� ID ����
//			_int iBoneID = pBoneData->iID;
//			aiBone* pBone = pMesh->mBones[j];
//			_float4x4 matOffset = ConvertAiMatrix_ToDXMatrix(pBone->mOffsetMatrix);
//			FMeshBoneData tMeshBoneData = { iBoneID, matOffset };
//			vecMeshes[i]->vecMeshBoneDatas.push_back(tMeshBoneData);
//
//			// �� ������ ����
//			for (_uint k = 0; k < pBone->mNumWeights; k++)
//			{
//				_uint iVertexID = pBone->mWeights[k].mVertexId;
//				_float fWeight = pBone->mWeights[k].mWeight;
//
//				// ���� �����Ϳ� ��ID�� ���� ���� ���
//				if (fWeight != 0.f)
//				{
//					pVecVertices[iVertexID].vecBoneID.push_back(j);
//					pVecVertices[iVertexID].vecWeights.push_back(fWeight);
//				}
//			}
//		}
//
//		// ä������ ���� �� ������ �Ҵ��Ѵ�.
//		for (_uint j = 0; j < pMesh->mNumVertices; j++)
//		{
//			pVecVertices[j].vecBoneID.resize(AI_LMW_MAX_WEIGHTS, 0);
//			pVecVertices[j].vecWeights.resize(AI_LMW_MAX_WEIGHTS, 0.f);
//		}
//#pragma endregion
//
//
//#pragma region ��
//		// �� (�ε���)
//		vecMeshes[i]->vecIndices.reserve(pMesh->mNumFaces);
//		for (_uint j = 0; j < pMesh->mNumFaces; j++)
//		{
//			aiFace& face = pMesh->mFaces[j];
//			vecMeshes[i]->vecIndices.push_back(face.mIndices[0]);
//			vecMeshes[i]->vecIndices.push_back(face.mIndices[1]);
//			vecMeshes[i]->vecIndices.push_back(face.mIndices[2]);
//		}
//#pragma endregion
//
//
//
//#pragma region ���͸���
//		vecMeshes[i]->iMaterialIndex = pMesh->mMaterialIndex;
//#pragma endregion
//
//
//		/*for (_uint j = 0; j < pMesh->mNumVertices; j++)
//		{
//			wstringstream ss;
//			ss << pVecVertices[j].vecBoneID.size();
//			OutputDebugString(wstring(wstring(L"Vertex VecBoneSize : ") + ss.str() + L"\n").c_str());
//		}*/
//	}
//}
//
//void CModelMgr::Load_Material(FModelData* pModelData)
//{
//	// ���͸��� �׷� ����
//	FMaterialGroup* pMatGroup = pModelData->pMaterialGroup;
//
//	// ���͸��� �ε�
//	for (_uint i = 0; i < m_pScene->mNumMaterials; i++)
//	{
//		aiMaterial* pAIMaterial = m_pScene->mMaterials[i];
//		wstring matName = Make_Wstring(pAIMaterial->GetName().C_Str());
//
//		// ���͸��� �����̳� ����
//		FMaterialData* pMaterialData = FMaterialData::Create();
//		pMatGroup->Add_Material(matName, pMaterialData);
//
//		for (_uint j = 1; j < AI_TEXTURE_TYPE_MAX; j++)
//		{
//			_char		szDrive[MAX_PATH] = "";
//			_char		szDirectory[MAX_PATH] = "";
//
//			_splitpath_s(m_strLoadFilePath.c_str(), szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
//
//			aiString	strPath;
//			if (FAILED(pAIMaterial->GetTexture(Cast<aiTextureType>(j), 0, &strPath)))
//				continue;
//
//			_char		szFileName[MAX_PATH] = "";
//			_char		szEXT[MAX_PATH] = "";
//
//			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);
//
//			_char		szTmp[MAX_PATH] = "";
//			strcpy_s(szTmp, szDrive);
//			strcat_s(szTmp, szDirectory);
//			strcat_s(szTmp, szFileName);
//			strcat_s(szTmp, szEXT);
//
//			_tchar		szFullPath[MAX_PATH] = TEXT("");
//
//			MultiByteToWideChar(CP_ACP, 0, szTmp, Cast<_int>(strlen(szTmp)), szFullPath, MAX_PATH);
//
//			// ���͸��� ������ ����
//			pMaterialData->strTexture[j] = szFullPath;
//		}
//	}
//}
//
//void CModelMgr::Load_Animation(FModelData* pModelData)
//{
//	FBoneAnimGroup* pAnimGroup = pModelData->pAnimGroup;
//	FBoneGroup* pBoneGroup = pModelData->pBoneGroup;
//
//	if (!pAnimGroup || !pBoneGroup)
//		return;
//
//	// �ִϸ��̼�
//	for (_uint i = 0; i < m_pScene->mNumAnimations; i++)
//	{
//		aiAnimation* pAnimAI = m_pScene->mAnimations[i];
//
//		// �ִ� ������ ����
//		FBoneAnimData* pAnimData = FBoneAnimData::Create();
//		wstring AnimNameWithTK = Make_Wstring(pAnimAI->mName.C_Str());
//		pAnimData->fDuration = Cast<_float>(pAnimAI->mDuration);
//		pAnimData->fTickPerSecond = Cast<_float>(pAnimAI->mTicksPerSecond);
//		
//
//		// �ִϸ��̼� �̸� ����
//		size_t iTokkenInd = AnimNameWithTK.find_first_of(L'|') + Cast<size_t>(1);
//		wstring SkeletalName = AnimNameWithTK.substr(0, iTokkenInd - 1);
//		wstring AnimName = AnimNameWithTK.substr(iTokkenInd);
//		pAnimData->strName = AnimName;
//		pAnimData->iID = i;
//
//		// �ִϸ��̼� ä��
//		for (_uint j = 0; j < pAnimAI->mNumChannels; j++)
//		{
//			aiNodeAnim* pChannelAI = pAnimAI->mChannels[j];
//			FBoneAnimChannelData* pAnimChannelData = FBoneAnimChannelData::Create();
//			wstring AnimChannelName = Make_Wstring(pChannelAI->mNodeName.C_Str());
//
//			// �� ������ �Ѿ. �̰� �ɸ��� �ȵǴ� ����.
//			FBoneGroup* pBoneGroup = pModelData->pBoneGroup;
//			if (nullptr == pBoneGroup)
//				continue;
//			FBoneData* pBoneData = pBoneGroup->Find_BoneData(AnimChannelName);
//
//			// ä��, ��, ���� ID�� �����Ѵ�.
//			pAnimChannelData->iBoneID = pBoneData->iID;
//			// ���� �� Ű���������� ������ �����.
//			pAnimChannelData->m_iNumKeyFrames = max(pChannelAI->mNumPositionKeys, max(pChannelAI->mNumRotationKeys, pChannelAI->mNumScalingKeys));
//			pAnimChannelData->vecKeyFrames.reserve(pAnimChannelData->m_iNumKeyFrames);
//
//			// ���� ���ʴ�� ������ ���̴�.
//			_float3 vScale = {};
//			_float4 vRotation = {};
//			_float3 vPosition = {};
//
//			// ������ ����
//			for (_uint k = 0; k < pAnimChannelData->m_iNumKeyFrames; k++)
//			{
//				FKeyFrame KeyFrame = {};
//				if (k < pChannelAI->mNumScalingKeys)
//				{
//					auto vChannelScale = pChannelAI->mScalingKeys[k].mValue;
//					vScale = { vChannelScale.x, vChannelScale.y, vChannelScale.z };
//					KeyFrame.fTrackPosition = Cast<_float>(pChannelAI->mScalingKeys[k].mTime);
//				}
//
//				if (k < pChannelAI->mNumRotationKeys)
//				{
//					auto vChannelRot = pChannelAI->mRotationKeys[k].mValue;
//					vRotation = { vChannelRot.x, vChannelRot.y, vChannelRot.z, vChannelRot.w };
//					KeyFrame.fTrackPosition = Cast<_float>(pChannelAI->mRotationKeys[k].mTime);
//				}
//
//				if (k < pChannelAI->mNumPositionKeys)
//				{
//					auto vChannelPos = pChannelAI->mPositionKeys[k].mValue;
//					vPosition = { vChannelPos.x, vChannelPos.y, vChannelPos.z };
//					KeyFrame.fTrackPosition = Cast<_float>(pChannelAI->mPositionKeys[k].mTime);
//				}
//
//				KeyFrame.vScale = vScale;
//				KeyFrame.qtRot = vRotation;
//				KeyFrame.vPos = vPosition;
//
//				pAnimChannelData->vecKeyFrames.push_back(KeyFrame);
//			}
//
//			pAnimData->Add_AnimChannelData(AnimChannelName, pAnimChannelData);
//		}
//		pAnimGroup->Add_BoneAnim(AnimName, pAnimData);
//
//		int t = 1;
//	}
//}
//
//void CModelMgr::Load_Hierarchi(FBoneGroup* pBoneGroup,  aiNode* pRootNode)
//{
//	if (pRootNode)
//	{
//		wstring strKey = Make_Wstring(pRootNode->mName.C_Str());
//		m_iNodeID = 0;
//
//		// ID ����
//		FBoneData* pRootNodeData = FBoneData::Create();
//		pRootNodeData->iID = m_iNodeID++;
//		pRootNodeData->strName = strKey;
//		pRootNodeData->iParentID = -1;
//		pRootNodeData->matTransform = ConvertAiMatrix_ToDXMatrix(pRootNode->mTransformation);
//		pRootNodeData->matFinalTransform = pRootNodeData->matTransform;
//
//		pBoneGroup->Add_BoneData(strKey, pRootNodeData);
//
//		for (_uint i = 0; i < pRootNode->mNumChildren; i++)
//		{
//			Load_HierarchiNode(pBoneGroup, pRootNode->mChildren[i], pRootNodeData);
//		}
//	}
//
//	m_pRootArmature = nullptr;
//}
//
//void CModelMgr::Load_HierarchiNode(FBoneGroup* pBoneGroup, aiNode* pBoneNode, FBoneData* pParentNode)
//{
//	if (pBoneNode)
//	{
//		wstring strNodeKey = Make_Wstring(pBoneNode->mName.C_Str());
//
//		FBoneData* pNodeData = FBoneData::Create();
//		pNodeData->iID = m_iNodeID++;
//		pNodeData->strName = strNodeKey;
//		pNodeData->iParentID = pParentNode->iID;
//		pNodeData->matTransform = ConvertAiMatrix_ToDXMatrix(pBoneNode->mTransformation);	// ���� ������� �߿��ϴ�.
//		//XMStoreFloat4x4(&pNodeData->matOffset, XMMatrixInverse(nullptr, XMLoadFloat4x4(&pNodeData->matOffset)));
//		XMStoreFloat4x4(&pNodeData->matFinalTransform, XMMatrixIdentity());
//		pBoneGroup->Add_BoneData(strNodeKey, pNodeData);
//
//		//_float4x4 matTemp = {};
//		//XMStoreFloat4x4(&matTemp, XMLoadFloat4x4(&pNodeData->matTransform));// * XMLoadFloat4x4(&pNodeData->matTransform));
//
//		//_float3 vPos = Get_PosFromMatrix(matTemp);
//		//_float3 vRot = Get_RotEulerFromMatrix(matTemp);
//		//_float3 vScale = Get_ScaleFromMatrix(matTemp);
//
//		for (_uint i = 0; i < pBoneNode->mNumChildren; i++)
//		{
//			Load_HierarchiNode(pBoneGroup, pBoneNode->mChildren[i], pNodeData);
//		}
//	}
//}

HRESULT CModelMgr::Load_Binary(const wstring& strFileName, CModelLoad* pModel)
{
	ifstream ifs;

	ifs.open(m_strMainDir + strFileName, ios::binary);
	if (ifs.is_open())
	{
        TModelHeader Header = {};
        ifs.read(ReCast<_char*>(&Header), sizeof(Header));
        if (Header.iID != 0xFF05)
        {
            ifs.close();
            return E_FAIL;
        }


        // �޽�, ���͸���ID -> ����, �ε���, ��
        pModel->vecMeshes.resize(Header.iNumMeshes, {});
        for (_uint i = 0; i < Header.iNumMeshes; i++)
        {
            TMesh& Mesh = pModel->vecMeshes[i];
            _uint iNumVertices;
            _uint iNumIndices;
            _uint iNumBones;

            // ID
            Mesh.iID = i;

            // �̸�
            _uint iStrLength;
            ifs.read(ReCast<_char*>(&iStrLength), sizeof(_uint));
            Mesh.strName.resize(iStrLength, 0);
            if (iStrLength > 0)
                ifs.read(ReCast<_char*>(&Mesh.strName[0]), sizeof(_tchar) * (iStrLength + 1));

            // ���͸��� �ε���
            ifs.read(ReCast<_char*>(&Mesh.iMaterialIndex), sizeof(_uint));

            // ����
            ifs.read(ReCast<_char*>(&iNumVertices), sizeof(_int));
            Mesh.vecVertices.resize(iNumVertices, {});
            for (_uint j = 0; j < iNumVertices; j++)
            {
                TMeshVertex& Vertex = Mesh.vecVertices[j];
                _uint iNumWeights = Cast<_uint>(Vertex.vecBoneID.size());

                // ���� ����
                ifs.read(ReCast<_char*>(&Vertex), 4 * sizeof(_float3) + sizeof(_float2));
                /*ifs.read(ReCast<_char*>(&Vertex.vPosition), sizeof(_float3));
                ifs.read(ReCast<_char*>(&Vertex.vTexCoord), sizeof(_float2));
                ifs.read(ReCast<_char*>(&Vertex.vNormal), sizeof(_float3));
                ifs.read(ReCast<_char*>(&Vertex.vTangent), sizeof(_float3));
                ifs.read(ReCast<_char*>(&Vertex.vBiTangent), sizeof(_float3));*/

                // ��ID, WeightID ����
                ifs.read(ReCast<_char*>(&iNumWeights), sizeof(_uint));
                Vertex.vecBoneID.resize(iNumWeights, {});
                Vertex.vecWeights.resize(iNumWeights, {});
                if (iNumWeights > 0)
                {
                    ifs.read(ReCast<_char*>(&Vertex.vecBoneID[0]), sizeof(_int) * iNumWeights);
                    ifs.read(ReCast<_char*>(&Vertex.vecWeights[0]), sizeof(_float) * iNumWeights);
                }
            }

            // �ε���
            ifs.read(ReCast<_char*>(&iNumIndices), sizeof(_int));
            Mesh.vecIndices.resize(iNumIndices, {});
            if (iNumIndices > 0)
                ifs.read(ReCast<_char*>(&Mesh.vecIndices[0]), sizeof(_int) * iNumIndices);

            // ��
            ifs.read(ReCast<_char*>(&iNumBones), sizeof(_int));
            Mesh.vecBones.resize(iNumBones, {});
            if (iNumBones > 0)
                ifs.read(ReCast<_char*>(&Mesh.vecBones[0]), sizeof(TMeshBone) * iNumBones);
        }



        // ���͸���
        pModel->vecMaterials.resize(Header.iNumMaterials, {});
        for (_uint i = 0; i < Header.iNumMaterials; i++)
        {
            TMaterial& Mat = pModel->vecMaterials[i];
            _uint iNumTexures;

            // �̸�
            _uint iStrLength;
            ifs.read(ReCast<_char*>(&iStrLength), sizeof(_uint));
            Mat.strName.resize(iStrLength, 0);
            if (iStrLength > 0)
                ifs.read(ReCast<_char*>(&Mat.strName[0]), sizeof(_tchar) * (iStrLength + 1));

            // �ؽ�ó ����
            ifs.read(ReCast<_char*>(&iNumTexures), sizeof(_uint));
            for (_uint j = 0; j < iNumTexures; j++)
            {
                // �ؽ�ó �̸�
                _uint iStrLength;
                ifs.read(ReCast<_char*>(&iStrLength), sizeof(_uint));
                Mat.strTextures[j].resize(iStrLength, 0);
                if (iStrLength > 0)
                    ifs.read(ReCast<_char*>(&Mat.strTextures[j][0]), sizeof(_tchar) * (iStrLength + 1));
            }
        }



        // ��
        pModel->vecBones.resize(Header.iNumBones, {});
        for (_uint i = 0; i < Header.iNumBones; i++)
        {
            TBone& Bone = pModel->vecBones[i];

            // ID
            Bone.iID = i;

            // �̸�
            _uint iStrLength;
            ifs.read(ReCast<_char*>(&iStrLength), sizeof(_uint));
            Bone.strName.resize(iStrLength, 0);
            if (iStrLength > 0)
                ifs.read(ReCast<_char*>(&Bone.strName[0]), sizeof(_tchar) * (iStrLength + 1));

            // �θ� ID
            ifs.read(ReCast<_char*>(&Bone.iParentID), sizeof(_int));

            // Ʈ������
            ifs.read(ReCast<_char*>(&Bone.matTransform), sizeof(_float4x4));
        }



        // �ִϸ��̼� ����
        pModel->vecAnims.resize(Header.iNumAnims, {});
        for (_uint i = 0; i < Header.iNumAnims; i++)
        {
            TAnim& Anim = pModel->vecAnims[i];
            _uint iNumChannels;

            // ID
            Anim.iID = i;

            // �ִ� �̸�
            _uint iStrLength;
            ifs.read(ReCast<_char*>(&iStrLength), sizeof(_uint));
            Anim.strName.resize(iStrLength, 0);
            if (iStrLength > 0)
                ifs.read(ReCast<_char*>(&Anim.strName[0]), sizeof(_tchar) * (iStrLength + 1));

            // ���ӽð�
            ifs.read(ReCast<_char*>(&Anim.fDuration), sizeof(_float));

            // �ʴ� ������
            ifs.read(ReCast<_char*>(&Anim.fTickPerSeconds), sizeof(_float));

            // ä�� ����
            ifs.read(ReCast<_char*>(&iNumChannels), sizeof(_uint));
            Anim.vecChannels.resize(iNumChannels, {});
            for (_uint j = 0; j < iNumChannels; j++)
            {
                TChannel& Channel = Anim.vecChannels[j];
                _uint iNumKeyFrames;

                // ID
                ifs.read(ReCast<_char*>(&Channel.iBoneID), sizeof(_uint));

                // ä�� �̸�
                _uint iStrLength;
                ifs.read(ReCast<_char*>(&iStrLength), sizeof(_uint));
                Channel.strName.resize(iStrLength, 0);
                if (iStrLength > 0)
                    ifs.read(ReCast<_char*>(&Channel.strName[0]), sizeof(_tchar) * (iStrLength + 1));

                // Ű������
                ifs.read(ReCast<_char*>(&iNumKeyFrames), sizeof(_uint));
                Channel.vecKeyFrames.resize(iNumKeyFrames, {});
                if (iNumKeyFrames > 0)
                    ifs.read(ReCast<_char*>(&Channel.vecKeyFrames[0]), sizeof(TKeyFrame) * iNumKeyFrames);
            }
        }
	}

	ifs.close();

	return S_OK;
}

void CModelMgr::Load_Model(const EModelGroupIndex eGroupIndex, const wstring& strFileName)
{
	CModelLoad* pModel = CModelLoad::Create();

	if (FAILED(Load_Binary(strFileName, pModel)))
	{
		Safe_Release(pModel);
		return;
	}

    FModelData* pModelData = Add_ModelData(eGroupIndex, strFileName);

	Load_Bones(pModelData, pModel);

	Load_Materials(pModelData, pModel);

	Load_Meshes(pModelData, pModel);

	Load_Animations(pModelData, pModel);

    Safe_Release(pModel);
}

void CModelMgr::Load_Meshes(FModelData* pModelData, CModelLoad* pModel)
{
    auto pMeshGroup = pModelData->pMeshGroup;

    pMeshGroup->vecMeshDatas.resize(pModel->vecMeshes.size());
    for (_uint i = 0; i < pModel->vecMeshes.size(); i++)
    {
        TMesh& tMesh = pModel->vecMeshes[i];
        FMeshData* pMeshData = FMeshData::Create();
        
        pMeshData->iID = tMesh.iID;
        pMeshData->iMaterialIndex = tMesh.iMaterialIndex;
        pMeshGroup->Add_Mesh(tMesh.strName, pMeshData);
        
        pMeshData->vecVertices.resize(tMesh.vecVertices.size());
        for (_uint j = 0; j < tMesh.vecVertices.size(); j++)
        {
            TMeshVertex& tMeshVtx = tMesh.vecVertices[j];
            pMeshData->vecVertices[j].vPosition = tMeshVtx.vPosition;
            pMeshData->vecVertices[j].vTexCoord = tMeshVtx.vTexCoord;
            pMeshData->vecVertices[j].vNormal = tMeshVtx.vNormal;
            pMeshData->vecVertices[j].vTangent = tMeshVtx.vTangent;
            pMeshData->vecVertices[j].vBiTangent = tMeshVtx.vBiTangent;


            pMeshData->vecVertices[j].vecBoneID.resize(tMeshVtx.vecBoneID.size());
            pMeshData->vecVertices[j].vecWeights.resize(tMeshVtx.vecWeights.size());
            for (_uint k = 0; k < tMeshVtx.vecBoneID.size(); k++)
            {
                pMeshData->vecVertices[j].vecBoneID[k] = tMeshVtx.vecBoneID[k];
                pMeshData->vecVertices[j].vecWeights[k] = tMeshVtx.vecWeights[k];
            }
        }

        pMeshData->vecIndices.resize(tMesh.vecIndices.size());
        for (_uint j = 0; j < tMesh.vecIndices.size(); j++)
        {
            pMeshData->vecIndices[j] = tMesh.vecIndices[j];
        }

        pMeshData->vecMeshBoneDatas.resize(tMesh.vecBones.size());
        for (_uint j = 0; j < tMesh.vecBones.size(); j++)
        {
            TMeshBone& tMeshBone = tMesh.vecBones[j];

            pMeshData->vecMeshBoneDatas[j].iBoneID = tMeshBone.iBoneID;
            pMeshData->vecMeshBoneDatas[j].matOffset = tMeshBone.matOffset;
        }
    }
}

void CModelMgr::Load_Materials(FModelData* pModelData, CModelLoad* pModel)
{
    auto pMaterialGroup = pModelData->pMaterialGroup;

    for (_uint i = 0; i < pModel->vecMaterials.size(); i++)
    {
        TMaterial& Material = pModel->vecMaterials[i];
        FMaterialData* pMaterialData = FMaterialData::Create();
        pMaterialGroup->Add_Material(Material.strName, pMaterialData);

        for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
        {
            pMaterialData->strTexture[j] = Material.strTextures[j];
        }
    }
}

void CModelMgr::Load_Animations(FModelData* pModelData, CModelLoad* pModel)
{
    auto pAnimGroup = pModelData->pAnimGroup;

    for (_uint i = 0; i < pModel->vecAnims.size(); i++)
    {
        TAnim& Anim = pModel->vecAnims[i];
        FBoneAnimData* pBoneAnimData = FBoneAnimData::Create();
        pAnimGroup->Add_BoneAnim(Anim.strName, pBoneAnimData);

        pBoneAnimData->iID = Anim.iID;
        pBoneAnimData->strName = Anim.strName;
        pBoneAnimData->fDuration = Anim.fDuration;
        pBoneAnimData->fTickPerSecond = Anim.fTickPerSeconds;

        for (_uint j = 0; j < Anim.vecChannels.size(); j++)
        {
            TChannel& Channel = Anim.vecChannels[j];
            FBoneAnimChannelData* pChannelData = FBoneAnimChannelData::Create();
            
            pChannelData->iBoneID = Channel.iBoneID;
            pChannelData->m_iNumKeyFrames = Cast<_uint>(Channel.vecKeyFrames.size());
            pBoneAnimData->Add_AnimChannelData(Channel.strName, pChannelData);

            for (_uint k = 0; k < Channel.vecKeyFrames.size(); k++)
            {
                TKeyFrame& KeyFrame = Channel.vecKeyFrames[k];
                FKeyFrame KeyData = {};

                KeyData.fTrackPosition = KeyFrame.fTrackPosition;
                KeyData.vPos = KeyFrame.vPos;
                KeyData.qtRot = KeyFrame.qtRot;
                KeyData.vScale = KeyFrame.vScale;
                
                pChannelData->vecKeyFrames.push_back(KeyData);
            }
        }
    }
}

void CModelMgr::Load_Bones(FModelData* pModelData, CModelLoad* pModel)
{
    auto pBoneGroup = pModelData->pBoneGroup;

    for (_uint i = 0; i < pModel->vecBones.size(); i++)
    {
        TBone& tBone = pModel->vecBones[i];

        FBoneData* pBoneData = FBoneData::Create();
        pBoneData->iID = tBone.iID;
        pBoneData->strName = tBone.strName;
        pBoneData->iParentID = tBone.iParentID;
        pBoneData->matTransform = tBone.matTransform;

        pBoneGroup->Add_BoneData(tBone.strName, pBoneData);
    }
}

const FMeshData* const CModelMgr::Find_MeshData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const wstring& strMeshKey, const _uint iRangeIndex)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strModelKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	return (*iter).second->pMeshGroup->Find_Mesh(strMeshKey, iRangeIndex);
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
	_float4x4	matOffsetTG = {};
	memcpy(&matOffsetTG, &matrix, sizeof(_float4x4));

	XMStoreFloat4x4(&matOffsetTG, XMMatrixTranspose(XMLoadFloat4x4(&matOffsetTG)));

	return matOffsetTG;
}


