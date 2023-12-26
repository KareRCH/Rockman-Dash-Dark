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


        // 메쉬, 머터리얼ID -> 정점, 인덱스, 뼈
        pModel->vecMeshes.resize(Header.iNumMeshes, {});
        for (_uint i = 0; i < Header.iNumMeshes; i++)
        {
            TMesh& Mesh = pModel->vecMeshes[i];
            _uint iNumVertices;
            _uint iNumIndices;
            _uint iNumBones;

            // ID
            Mesh.iID = i;

            // 이름
            _uint iStrLength;
            ifs.read(ReCast<_char*>(&iStrLength), sizeof(_uint));
            Mesh.strName.resize(iStrLength, 0);
            if (iStrLength > 0)
                ifs.read(ReCast<_char*>(&Mesh.strName[0]), sizeof(_tchar) * (iStrLength + 1));

            // 머터리얼 인덱스
            ifs.read(ReCast<_char*>(&Mesh.iMaterialIndex), sizeof(_uint));

            // 정점
            ifs.read(ReCast<_char*>(&iNumVertices), sizeof(_int));
            Mesh.vecVertices.resize(iNumVertices, {});
            for (_uint j = 0; j < iNumVertices; j++)
            {
                TMeshVertex& Vertex = Mesh.vecVertices[j];
                _uint iNumWeights = Cast<_uint>(Vertex.vecBoneID.size());

                // 정점 정보
                ifs.read(ReCast<_char*>(&Vertex), 4 * sizeof(_float3) + sizeof(_float2));
                /*ifs.read(ReCast<_char*>(&Vertex.vPosition), sizeof(_float3));
                ifs.read(ReCast<_char*>(&Vertex.vTexCoord), sizeof(_float2));
                ifs.read(ReCast<_char*>(&Vertex.vNormal), sizeof(_float3));
                ifs.read(ReCast<_char*>(&Vertex.vTangent), sizeof(_float3));
                ifs.read(ReCast<_char*>(&Vertex.vBiTangent), sizeof(_float3));*/

                // 뼈ID, WeightID 개수
                ifs.read(ReCast<_char*>(&iNumWeights), sizeof(_uint));
                Vertex.vecBoneID.resize(iNumWeights, {});
                Vertex.vecWeights.resize(iNumWeights, {});
                if (iNumWeights > 0)
                {
                    ifs.read(ReCast<_char*>(&Vertex.vecBoneID[0]), sizeof(_int) * iNumWeights);
                    ifs.read(ReCast<_char*>(&Vertex.vecWeights[0]), sizeof(_float) * iNumWeights);
                }
            }

            // 인덱스
            ifs.read(ReCast<_char*>(&iNumIndices), sizeof(_int));
            Mesh.vecIndices.resize(iNumIndices, {});
            if (iNumIndices > 0)
                ifs.read(ReCast<_char*>(&Mesh.vecIndices[0]), sizeof(_int) * iNumIndices);

            // 뼈
            ifs.read(ReCast<_char*>(&iNumBones), sizeof(_int));
            Mesh.vecBones.resize(iNumBones, {});
            if (iNumBones > 0)
                ifs.read(ReCast<_char*>(&Mesh.vecBones[0]), sizeof(TMeshBone) * iNumBones);
        }



        // 머터리얼
        pModel->vecMaterials.resize(Header.iNumMaterials, {});
        for (_uint i = 0; i < Header.iNumMaterials; i++)
        {
            TMaterial& Mat = pModel->vecMaterials[i];
            _uint iNumTexures;

            // 이름
            _uint iStrLength;
            ifs.read(ReCast<_char*>(&iStrLength), sizeof(_uint));
            Mat.strName.resize(iStrLength, 0);
            if (iStrLength > 0)
                ifs.read(ReCast<_char*>(&Mat.strName[0]), sizeof(_tchar) * (iStrLength + 1));

            // 텍스처 개수
            ifs.read(ReCast<_char*>(&iNumTexures), sizeof(_uint));
            for (_uint j = 0; j < iNumTexures; j++)
            {
                // 텍스처 이름
                _uint iStrLength;
                ifs.read(ReCast<_char*>(&iStrLength), sizeof(_uint));
                Mat.strTextures[j].resize(iStrLength, 0);
                if (iStrLength > 0)
                    ifs.read(ReCast<_char*>(&Mat.strTextures[j][0]), sizeof(_tchar) * (iStrLength + 1));
            }
        }



        // 뼈
        pModel->vecBones.resize(Header.iNumBones, {});
        for (_uint i = 0; i < Header.iNumBones; i++)
        {
            TBone& Bone = pModel->vecBones[i];

            // ID
            Bone.iID = i;

            // 이름
            _uint iStrLength;
            ifs.read(ReCast<_char*>(&iStrLength), sizeof(_uint));
            Bone.strName.resize(iStrLength, 0);
            if (iStrLength > 0)
                ifs.read(ReCast<_char*>(&Bone.strName[0]), sizeof(_tchar) * (iStrLength + 1));

            // 부모 ID
            ifs.read(ReCast<_char*>(&Bone.iParentID), sizeof(_int));

            // 트랜스폼
            ifs.read(ReCast<_char*>(&Bone.matTransform), sizeof(_float4x4));
        }



        // 애니메이션 개수
        pModel->vecAnims.resize(Header.iNumAnims, {});
        for (_uint i = 0; i < Header.iNumAnims; i++)
        {
            TAnim& Anim = pModel->vecAnims[i];
            _uint iNumChannels;

            // ID
            Anim.iID = i;

            // 애님 이름
            _uint iStrLength;
            ifs.read(ReCast<_char*>(&iStrLength), sizeof(_uint));
            Anim.strName.resize(iStrLength, 0);
            if (iStrLength > 0)
                ifs.read(ReCast<_char*>(&Anim.strName[0]), sizeof(_tchar) * (iStrLength + 1));

            // 지속시간
            ifs.read(ReCast<_char*>(&Anim.fDuration), sizeof(_float));

            // 초당 프레임
            ifs.read(ReCast<_char*>(&Anim.fTickPerSeconds), sizeof(_float));

            // 채널 개수
            ifs.read(ReCast<_char*>(&iNumChannels), sizeof(_uint));
            Anim.vecChannels.resize(iNumChannels, {});
            for (_uint j = 0; j < iNumChannels; j++)
            {
                TChannel& Channel = Anim.vecChannels[j];
                _uint iNumKeyFrames;

                // ID
                ifs.read(ReCast<_char*>(&Channel.iBoneID), sizeof(_uint));

                // 채널 이름
                _uint iStrLength;
                ifs.read(ReCast<_char*>(&iStrLength), sizeof(_uint));
                Channel.strName.resize(iStrLength, 0);
                if (iStrLength > 0)
                    ifs.read(ReCast<_char*>(&Channel.strName[0]), sizeof(_tchar) * (iStrLength + 1));

                // 키프레임
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

HRESULT CModelMgr::Load_Model(const EModelGroupIndex eGroupIndex, const wstring& strFileName)
{
    FModelData* pModelData = ConCast<FModelData*>(Find_ModelData(eGroupIndex, strFileName));
    if (nullptr != pModelData)
        return S_OK;

	CModelLoad* pModel = CModelLoad::Create();

	if (FAILED(Load_Binary(strFileName, pModel)))
	{
		Safe_Release(pModel);
		return E_FAIL;
	}

    pModelData = Add_ModelData(eGroupIndex, strFileName);

	Load_Bones(pModelData, pModel);

	Load_Materials(pModelData, pModel);

	Load_Meshes(pModelData, pModel);

	Load_Animations(pModelData, pModel);

    Safe_Release(pModel);

    return S_OK;
}

void CModelMgr::Load_Meshes(FModelData* pModelData, CModelLoad* pModel)
{
    auto pMeshGroup = pModelData->pMeshGroup;

    pMeshGroup->vecMeshDatas.resize(pModel->vecMeshes.size());
    for (_uint i = 0; i < pModel->vecMeshes.size(); i++)
    {
        TMesh& tMesh = pModel->vecMeshes[i];
        FMeshData* pMeshData = FMeshData::Create(tMesh);
        if (pMeshData != nullptr)
            pMeshGroup->Add_Mesh(tMesh.strName, pMeshData);
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

const FMeshData* const CModelMgr::Find_MeshData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const _uint iIndex)
{
    _uint iGroupIndex = ECast(eGroupIndex);

    auto iter = m_mapModelDatas[iGroupIndex].find(strModelKey);
    if (iter == m_mapModelDatas[iGroupIndex].end())
        return nullptr;

    return (*iter).second->pMeshGroup->Find_Mesh(iIndex);
}

CBoneGroup* CModelMgr::Clone_BoneGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey)
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
	
	FModelData* pModelData = FModelData::Create(false);
	m_mapModelDatas[iIndex].emplace(strModelKey, pModelData);

	return pModelData;
}

const FMaterialData* const CModelMgr::Find_MaterialData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const _uint iIndex)
{
    _uint iGroupIndex = ECast(eGroupIndex);

    auto iter = m_mapModelDatas[iGroupIndex].find(strModelKey);
    if (iter == m_mapModelDatas[iGroupIndex].end())
        return nullptr;

    return (*iter).second->pMaterialGroup->Find_Material(iIndex);
}

CMeshGroup* CModelMgr::Find_MeshGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strModelKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	return (*iter).second->pMeshGroup;
}

CBoneGroup* CModelMgr::Find_BoneGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strModelKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	return (*iter).second->pBoneGroup;
}

CBoneAnimGroup* CModelMgr::Find_AnimGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strModelKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	return (*iter).second->pAnimGroup;
}

CMaterialGroup* CModelMgr::Find_MaterialGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey)
{
	_uint iIndex = ECast(eGroupIndex);

	auto iter = m_mapModelDatas[iIndex].find(strModelKey);
	if (iter == m_mapModelDatas[iIndex].end())
		return nullptr;

	return (*iter).second->pMaterialGroup;
}
