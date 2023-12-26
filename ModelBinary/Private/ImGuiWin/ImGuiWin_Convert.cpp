#include "ImGuiWin/ImGuiWin_Convert.h"
#include <shlobj.h>
#include <Shlwapi.h>
#include <ModelLoad.h>

#include "System/GameInstance.h"

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {
    if (uMsg == BFFM_INITIALIZED) {
        // 초기 폴더 설정
        SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
    }
    return 0;
}

CImGuiWin_Convert::CImGuiWin_Convert()
{
}

HRESULT CImGuiWin_Convert::Initialize()
{
    m_bOpen = true;
    m_iLimit_Async = 12;

    return S_OK;
}

void CImGuiWin_Convert::Tick(const _float& fTimeDelta)
{
    if (!m_bOpen)
        return;


    ImGui::Begin(u8"FBX To Binary");

    if (ImGui::Button(u8"폴더 설정하기"))
    {
        BROWSEINFO bi = { 0 };
        LPITEMIDLIST pidl;
        TCHAR szDir[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, szDir);

        bi.hwndOwner = NULL;
        bi.pidlRoot = NULL;
        bi.pszDisplayName = szDir;
        bi.lpszTitle = _T("폴더 선택");
        bi.ulFlags = BIF_RETURNONLYFSDIRS;
        bi.lpfn = BrowseCallbackProc;
        // 프로그램이 실행된 경로를 초기 폴더로 설정합니다.
        bi.lParam = reinterpret_cast<LPARAM>(szDir);

        pidl = SHBrowseForFolder(&bi);

        if (pidl != NULL) {
            SHGetPathFromIDList(pidl, szDir);
            MessageBox(NULL, szDir, _T("선택된 폴더"), MB_OK);
            CoTaskMemFree(pidl);
            wstring text(szDir);
            m_strDirectory = string(text.begin(), text.end());
        }
    }

    ImGui::Text(u8"선택된 폴더 : ");
    if (!m_strDirectory.empty())
    {
        ImGui::SameLine();
        ImGui::Text(m_strDirectory.c_str());

    }

    if (ImGui::Button(u8"FBX 찾기"))
    {
        m_FBX_Paths.clear();
        SearchAllFBX(Make_Wstring(m_strDirectory));
    }

    ImGui::SameLine();
    if (ImGui::Button(u8"변환"))
    {
        ConvertToBinary();
        m_bShowComplete = true;
        
    }

    // 변환 완료 메시지
    {
        if (m_bShowComplete) {
            ImGui::OpenPopup(u8"변환됨");
        }

        if (ImGui::BeginPopupModal(u8"변환됨", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text(u8"변환이 완료되었습니다.");

            if (ImGui::Button(u8"확인")) {
                ImGui::CloseCurrentPopup();
                m_bShowComplete = false;
            }

            ImGui::EndPopup();
        }
    }

    ImGui::Text(u8"타겟팅 FBX");
    if (!m_FBX_Paths.empty())
    {
        for (_uint i = 0; i < m_FBX_Paths.size(); i++)
        {
            string strDisplay = string(m_FBX_Paths[i].begin(), m_FBX_Paths[i].end());
            ImGui::Text(strDisplay.c_str());
        }
    }

    


    SUPER::Tick(fTimeDelta);

    ImGui::End();
}

HRESULT CImGuiWin_Convert::Render()
{
    SUPER::Render();

    return S_OK;
}

CImGuiWin_Convert* CImGuiWin_Convert::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("CImGuiWin_Viewer Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CImGuiWin_Convert::Free()
{
    SUPER::Free();
}

void CImGuiWin_Convert::SearchAllFBX(const std::wstring& folderPath)
{
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((folderPath + _T("\\*")).c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        if (_tcscmp(findFileData.cFileName, _T(".")) == 0 || _tcscmp(findFileData.cFileName, _T("..")) == 0) {
            continue;
        }

        std::wstring filePath = folderPath + _T("\\") + findFileData.cFileName;

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // 현재 폴더 또는 작업 수행
            // 여기에서 필요한 추가 작업을 수행할 수 있습니다.

            // 재귀적으로 하위 폴더 탐색
            SearchAllFBX(filePath);
        }
        else if (_tcsicmp(PathFindExtension(findFileData.cFileName), _T(".fbx")) == 0) {
            // .fbx 파일을 찾으면 해당 경로 출력
            m_FBX_Paths.push_back(filePath);
            std::wcout << L"FBX File: " << filePath << std::endl;
        }

    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

void CImGuiWin_Convert::ConvertToBinary()
{
    ifstream ifs;

    vector<future<void>> vecFutures;

    for (_uint i = 0; i < m_FBX_Paths.size();)
    {
        // 추가된 비동기 함수들 돌면서 완료된 것. 삭제 후 다시 추가
        if (vecFutures.size() < m_iLimit_Async)
            vecFutures.push_back(async(launch::async, &CImGuiWin_Convert::Load_FBX, this, m_FBX_Paths[i++]));
        else
        {
            for (auto iter = vecFutures.begin(); iter != vecFutures.end();)
            {
                auto status = iter->wait_for(1ms);
                if (status == future_status::ready)
                {
                    iter->get();
                    iter = vecFutures.erase(iter);
                    break;
                }
                else if (status == future_status::timeout)
                {
                    iter = vecFutures.erase(iter);
                    continue;
                }

                ++iter;
            }
        }
    }

    // 모든 비동기 함수가 넣어졌을 때 대기하는 코드
    while (!vecFutures.empty())
    {
        for (auto iter = vecFutures.begin(); iter != vecFutures.end();)
        {
            auto status = iter->wait_for(1ms);
            if (status == future_status::ready)
            {
                iter->get();
                iter = vecFutures.erase(iter);
                if (iter == vecFutures.end())
                    break;
            }
            else if (status == future_status::timeout)
            {
                iter = vecFutures.erase(iter);
                continue;
            }

            ++iter;
        }
    }
    cout << "다 열음" << endl;
}

void CImGuiWin_Convert::Load_FBX(const wstring& strPath)
{
    Assimp::Importer importer;

    _uint iFlag = 0;
    iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
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

    string strConvert(strPath.begin(), strPath.end());
    const aiScene* pScene = importer.ReadFile(strConvert.c_str(), iFlag);

    if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
    {
#ifdef _DEBUG
        cout << "모델 씬 로드 실패" << endl;
#endif
        importer.FreeScene();
        return;
    }

    CModelLoad* pModel = CModelLoad::Create();

    if (FAILED(Load_Bones(pScene, pModel)))
    {
        Safe_Release(pModel);
        importer.FreeScene();
        return;
    }
    if (FAILED(Load_Animations(pScene, pModel)))
    {
        Safe_Release(pModel);
        importer.FreeScene();
        return;
    }
    if (FAILED(Load_Meshes(pScene, pModel)))
    {
        Safe_Release(pModel);
        importer.FreeScene();
        return;
    }
    if (FAILED(Load_Materials(pScene, pModel, strConvert)))
    {
        Safe_Release(pModel);
        importer.FreeScene();
        return;
    }

    size_t iDotIndex = strPath.find_last_of(L".");
    wstring strPathWithOutExt = strPath.substr(0, iDotIndex);
    strPathWithOutExt += TEXT(".amodel");
    string strPrint(strPathWithOutExt.begin(), strPathWithOutExt.end());
    if (FAILED(Save_Binary(strPathWithOutExt, pModel)))
    {
        
        cout << "저장 실패" << strPrint << endl;
        Safe_Release(pModel);
        return;
    }

    if (FAILED(Load_Binary(strPathWithOutExt, pModel)))
    {
        
    }
    cout<< "저장 성공" << strPrint << endl;

    Safe_Release(pModel);
    importer.FreeScene();
}

HRESULT CImGuiWin_Convert::Load_Bones(const aiScene* pScene, CModelLoad* pModel)
{
    auto pRootNode = pScene->mRootNode;
    TBone Bone = {};
    _uint iID = 0;

    Bone.iID = iID++;
    Bone.iParentID = -1;
    Bone.strName = Make_Wstring(pRootNode->mName.C_Str());
    _matrix TransformMatrix;
    memcpy(&TransformMatrix, &pRootNode->mTransformation, sizeof(_matrix));
    XMStoreFloat4x4(&Bone.matTransform, XMMatrixTranspose(TransformMatrix));
    pModel->vecBones.push_back(Bone);

    for (_uint i = 0; i < pRootNode->mNumChildren; i++)
    {
        Load_Bone(pRootNode->mChildren[i], pModel, iID, Bone);
    }

    return S_OK;
}

HRESULT CImGuiWin_Convert::Load_Bone(const aiNode* pNode, CModelLoad* pModel, _uint& iID, const TBone& ParentBone)
{
    TBone Bone = {};

    Bone.iID = iID++;                                       // ID
    Bone.iParentID = ParentBone.iID;                        // 부모
    Bone.strName = Make_Wstring(pNode->mName.C_Str());      // 이름
    _matrix TransformMatrix;                                // 행렬
    memcpy(&TransformMatrix, &pNode->mTransformation, sizeof(_matrix));
    XMStoreFloat4x4(&Bone.matTransform, XMMatrixTranspose(TransformMatrix));
    pModel->vecBones.push_back(Bone);

    for (_uint i = 0; i < pNode->mNumChildren; i++)
    {
        Load_Bone(pNode->mChildren[i], pModel, iID, Bone);
    }

    return S_OK;
}

HRESULT CImGuiWin_Convert::Load_Animations(const aiScene* pScene, CModelLoad* pModel)
{
    if (!pScene->HasAnimations())
        return S_OK;

    pModel->vecAnims.resize(pScene->mNumAnimations, {});
    for (_uint i = 0; i < pScene->mNumAnimations; i++)
    {
        auto pAnim = pScene->mAnimations[i];
        TAnim& AnimData = pModel->vecAnims[i];

        AnimData.fDuration = pAnim->mDuration;
        AnimData.fTickPerSeconds = pAnim->mTicksPerSecond;
        AnimData.iID = i;
        AnimData.strName = Make_Wstring(pAnim->mName.C_Str());

        AnimData.vecChannels.resize(pAnim->mNumChannels, {});
        for (_uint j = 0; j < pAnim->mNumChannels; j++)
        {
            auto pChannel = pAnim->mChannels[j];
            TChannel& ChannelData = AnimData.vecChannels[j];

            _uint		iBoneIndex = { 0 };
            vector<TBone>& vecBones = pModel->vecBones;
            auto	iter = find_if(vecBones.begin(), vecBones.end(), [&](TBone& Bone)
                {
                    if (Make_Wstring(pChannel->mNodeName.C_Str()) == Bone.strName)
                    {
                        return true;
                    }

                    ++iBoneIndex;

                    return false;
                });

            if (iter == vecBones.end())
                return E_FAIL;

            ChannelData.iBoneID = iBoneIndex;
            ChannelData.strName = Make_Wstring(pChannel->mNodeName.C_Str());


            _uint iNumKeyFrames = max(max(pChannel->mNumPositionKeys, pChannel->mNumRotationKeys), pChannel->mNumScalingKeys);
            _float3 vPos = {}, vScale = {};
            _float4 vRot = {};
            _float fTrackPos = 0.f;

            ChannelData.vecKeyFrames.resize(iNumKeyFrames, {});
            for (_uint k = 0; k < iNumKeyFrames; k++)
            {
                TKeyFrame& KeyFrame = ChannelData.vecKeyFrames[k];

                if (k < pChannel->mNumScalingKeys)
                {
                    memcpy(&vScale, &pChannel->mScalingKeys[k].mValue, sizeof(_float3));
                    fTrackPos = pChannel->mScalingKeys[k].mTime;
                }
                if (k < pChannel->mNumRotationKeys)
                {
                    vRot.x = pChannel->mRotationKeys[k].mValue.x;
                    vRot.y = pChannel->mRotationKeys[k].mValue.y;
                    vRot.z = pChannel->mRotationKeys[k].mValue.z;
                    vRot.w = pChannel->mRotationKeys[k].mValue.w;
                    fTrackPos = pChannel->mRotationKeys[k].mTime;
                }
                if (k < pChannel->mNumPositionKeys)
                {
                    memcpy(&vPos, &pChannel->mPositionKeys[k].mValue, sizeof(_float3));
                    fTrackPos = pChannel->mPositionKeys[k].mTime;
                }

                KeyFrame.vPos = vPos;
                KeyFrame.qtRot = vRot;
                KeyFrame.vScale = vScale;
                KeyFrame.fTrackPosition = fTrackPos;
            }
        }
    }

    return S_OK;
}

HRESULT CImGuiWin_Convert::Load_Meshes(const aiScene* pScene, CModelLoad* pModel)
{
    if (!pScene->HasMeshes())
        return S_OK;


    pModel->vecMeshes.resize(pScene->mNumMeshes, {});
    for (_uint i = 0; i < pScene->mNumMeshes; i++)
    {
        auto pMesh = pScene->mMeshes[i];
        TMesh& MeshData = pModel->vecMeshes[i];

        MeshData.iID = i;
        MeshData.strName = Make_Wstring(pMesh->mName.C_Str());
        MeshData.iMaterialIndex = pMesh->mMaterialIndex;
        
        MeshData.vecVertices.resize(pMesh->mNumVertices);
        for (_uint j = 0; j < pMesh->mNumVertices; j++)
        {
            memcpy(&MeshData.vecVertices[j].vPosition, &pMesh->mVertices[j], sizeof(_float3));
            memcpy(&MeshData.vecVertices[j].vTexCoord, &pMesh->mTextureCoords[0][j], sizeof(_float2));
            memcpy(&MeshData.vecVertices[j].vNormal, &pMesh->mNormals[j], sizeof(_float3));
            memcpy(&MeshData.vecVertices[j].vTangent, &pMesh->mTangents[j], sizeof(_float3));
            memcpy(&MeshData.vecVertices[j].vBiTangent, &pMesh->mBitangents[j], sizeof(_float3));

            MeshData.vecVertices[j].vecBoneID.reserve(AI_TEXTURE_TYPE_MAX);
            MeshData.vecVertices[j].vecWeights.reserve(AI_TEXTURE_TYPE_MAX);
        }

        MeshData.vecIndices.resize(pMesh->mNumFaces * 3);
        _uint iNumIndices = 0;
        for (_uint j = 0; j < pMesh->mNumFaces; j++)
        {
            MeshData.vecIndices[iNumIndices++] = pMesh->mFaces[j].mIndices[0];
            MeshData.vecIndices[iNumIndices++] = pMesh->mFaces[j].mIndices[1];
            MeshData.vecIndices[iNumIndices++] = pMesh->mFaces[j].mIndices[2];
        }

        MeshData.vecBones.resize(pMesh->mNumBones);
        for (_uint j = 0; j < pMesh->mNumBones; j++)
        {
            auto pAIBone = pMesh->mBones[j];
            auto OffsetMatrix = pAIBone->mOffsetMatrix;
            _matrix OffsetConvert;
            memcpy(&OffsetConvert, &OffsetMatrix, sizeof(_matrix));
            XMStoreFloat4x4(&MeshData.vecBones[j].matOffset, XMMatrixTranspose(OffsetConvert));

            _uint		iBoneIndex = { 0 };
            vector<TBone>& vecBones = pModel->vecBones;
            auto	iter = find_if(vecBones.begin(), vecBones.end(), [&](TBone& Bone)
            {
                if (Make_Wstring(pAIBone->mName.C_Str()) == Bone.strName)
                {
                    return true;
                }

                ++iBoneIndex;

                return false;
            });

            if (iter == vecBones.end())
                return E_FAIL;

            MeshData.vecBones[j].iBoneID = iBoneIndex;
            for (_uint k = 0; k < pAIBone->mNumWeights; k++)
            {
                _uint iVertexID = pAIBone->mWeights[k].mVertexId;
                _float fWeight = pAIBone->mWeights[k].mWeight;

                if (fWeight != 0.f)
                {
                    MeshData.vecVertices[iVertexID].vecBoneID.push_back(j);
                    MeshData.vecVertices[iVertexID].vecWeights.push_back(fWeight);
                }
            }
        }

        for (_uint j = 0; j < pMesh->mNumVertices; j++)
        {
        	MeshData.vecVertices[j].vecBoneID.resize(AI_LMW_MAX_WEIGHTS, 0);
        	MeshData.vecVertices[j].vecWeights.resize(AI_LMW_MAX_WEIGHTS, 0.f);
        }
    }

    return S_OK;
}

HRESULT CImGuiWin_Convert::Load_Materials(const aiScene* pScene, CModelLoad* pModel, const string& strModelFilePath)
{
    if (!pScene->HasMaterials())
        return S_OK;

    pModel->vecMaterials.resize(pScene->mNumMaterials);
    for (_uint i = 0; i < pScene->mNumMaterials; i++)
    {
        aiMaterial* pAIMaterial = pScene->mMaterials[i];
        TMaterial&	MatrialData = pModel->vecMaterials[i];

        MatrialData.strName = Make_Wstring(pAIMaterial->GetName().C_Str());

        for (size_t j = 1; j < AI_TEXTURE_TYPE_MAX; j++)
        {
            /*for (size_t k = 0; k < pAIMaterial->GetTextureCount(aiTextureType(j)); k++)
            {
                pAIMaterial->GetTexture(aiTextureType(j), k, );
            };*/

            _char		szDrive[MAX_PATH] = "";
            _char		szDirectory[MAX_PATH] = "";

            _splitpath_s(strModelFilePath.c_str(), szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

            aiString			strPath;
            if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strPath)))
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

            MatrialData.strTextures[j] = szFullPath;
        }
    }

    return S_OK;
}

HRESULT CImGuiWin_Convert::Save_Binary(const wstring& strPath, class CModelLoad* pModel)
{
    ofstream ofs;

    ofs.open(strPath.c_str(), ios::binary);
    if (ofs.is_open())
    {
        // 헤더부
        // 메쉬개수, 머터리얼, 뼈, 애니메이션 개수
        _uint iNumMeshes = Cast<_uint>(pModel->vecMeshes.size());
        _uint iNumMaterials = Cast<_uint>(pModel->vecMaterials.size());
        _uint iNumBones = Cast<_uint>(pModel->vecBones.size());
        _uint iNumAnims = Cast<_uint>(pModel->vecAnims.size());

        TModelHeader Header = {};
        Header.iID = 0xFF05;
        Header.iNumMeshes = Cast<_uint_16>(iNumMeshes);
        Header.iNumMaterials = Cast<_uint_16>(iNumMaterials);
        Header.iNumBones = Cast<_uint_16>(iNumBones);
        Header.iNumAnims = Cast<_uint_16>(iNumAnims);

        ofs.write(ReCast<const _char*>(&Header), sizeof(TModelHeader));

        // 메쉬, 머터리얼ID -> 정점, 인덱스, 뼈
        for (_uint i = 0; i < iNumMeshes; i++)
        {
            TMesh& Mesh = pModel->vecMeshes[i];
            

            // ID
            Mesh.iID = i;

            // 이름
            _uint iStrLength = Cast<_uint>(Mesh.strName.length());
            ofs.write(ReCast<const _char*>(&iStrLength), sizeof(_uint));
            if (iStrLength > 0)
                ofs.write(ReCast<const _char*>(Mesh.strName.data()), sizeof(_tchar) * (iStrLength + 1));

            // 머터리얼
            ofs.write(ReCast<const _char*>(&Mesh.iMaterialIndex), sizeof(_uint));

            // 정점
            _uint iNumVertices = Cast<_uint>(Mesh.vecVertices.size());
            ofs.write(ReCast<const _char*>(&iNumVertices), sizeof(_int));
            for (_uint j = 0; j < iNumVertices; j++)
            {
                TMeshVertex& Vertex = Mesh.vecVertices[j];
                
                ofs.write(ReCast<const _char*>(&Vertex), 4 * sizeof(_float3) + sizeof(_float2));
                /*ofs.write(ReCast<const _char*>(&Vertex.vPosition), sizeof(_float3));
                ofs.write(ReCast<const _char*>(&Vertex.vTexCoord), sizeof(_float2));
                ofs.write(ReCast<const _char*>(&Vertex.vNormal), sizeof(_float3));
                ofs.write(ReCast<const _char*>(&Vertex.vTangent), sizeof(_float3));
                ofs.write(ReCast<const _char*>(&Vertex.vBiTangent), sizeof(_float3));*/

                _uint iNumWeights = Cast<_uint>(Vertex.vecBoneID.size());
                ofs.write(ReCast<const _char*>(&iNumWeights), sizeof(_uint));
                if (iNumWeights > 0)
                {
                    ofs.write(ReCast<const _char*>(Vertex.vecBoneID.data()), sizeof(_int) * iNumWeights);
                    ofs.write(ReCast<const _char*>(Vertex.vecWeights.data()), sizeof(_float) * iNumWeights);
                }
            }

            // 인덱스
            _uint iNumIndices = Cast<_uint>(Mesh.vecIndices.size());
            ofs.write(ReCast<const _char*>(&iNumIndices), sizeof(_int));
            if (iNumIndices > 0)
                ofs.write(ReCast<const _char*>(Mesh.vecIndices.data()), sizeof(_int) * iNumIndices);
            
            // 뼈
            _uint iNumBones = Cast<_uint>(Mesh.vecBones.size());
            ofs.write(ReCast<const _char*>(&iNumBones), sizeof(_int));
            if (iNumBones > 0)
                ofs.write(ReCast<const _char*>(Mesh.vecBones.data()), sizeof(TMeshBone) * iNumBones);
        }
        


        // 머터리얼
        for (_uint i = 0; i < iNumMaterials; i++)
        {
            TMaterial& Mat = pModel->vecMaterials[i];
            
            // 이름
            _uint iStrLength = Cast<_uint>(Mat.strName.length());
            ofs.write(ReCast<const _char*>(&iStrLength), sizeof(_uint));
            if (iStrLength > 0)
                ofs.write(ReCast<const _char*>(Mat.strName.data()), sizeof(_tchar) * (iStrLength + 1));

            // 텍스처 개수
            _uint iNumTexures = Cast<_uint>(AI_TEXTURE_TYPE_MAX);
            ofs.write(ReCast<const _char*>(&iNumTexures), sizeof(_uint));
            for (_uint j = 0; j < iNumTexures; j++)
            {
                // 텍스처 이름
                _uint iStrLength = Cast<_uint>(Mat.strTextures[j].length());
                ofs.write(ReCast<const _char*>(&iStrLength), sizeof(_uint));
                if (iStrLength > 0)
                    ofs.write(ReCast<const _char*>(Mat.strTextures[j].data()), sizeof(_tchar) * (iStrLength + 1));
            }
        }



        // 뼈
        for (_uint i = 0; i < iNumBones; i++)
        {
            TBone& Bone = pModel->vecBones[i];
            
            // ID
            Bone.iID = i;

            // 뼈 이름
            _uint iStrLength = Cast<_uint>(Bone.strName.length());
            ofs.write(ReCast<const _char*>(&iStrLength), sizeof(_uint));
            if (iStrLength > 0)
                ofs.write(ReCast<const _char*>(Bone.strName.data()), sizeof(_tchar) * (iStrLength + 1));

            // 부모ID, 행렬
            ofs.write(ReCast<const _char*>(&Bone.iParentID), sizeof(_int));
            ofs.write(ReCast<const _char*>(&Bone.matTransform), sizeof(_float4x4));
        }



        // 애니메이션 개수
        for (_uint i = 0; i < iNumAnims; i++)
        {
            TAnim& Anim = pModel->vecAnims[i];
            


            // ID
            //Anim.iID = i;

            // 이름
            _uint iStrLength = Cast<_uint>(Anim.strName.length());
            ofs.write(ReCast<const _char*>(&iStrLength), sizeof(_uint));
            if (iStrLength > 0)
                ofs.write(ReCast<const _char*>(Anim.strName.data()), sizeof(_tchar) * (iStrLength + 1));

            // 시간
            ofs.write(ReCast<const _char*>(&Anim.fDuration), sizeof(_float));

            // 프레임당 시간
            ofs.write(ReCast<const _char*>(&Anim.fTickPerSeconds), sizeof(_float));

            // 채널
            _uint iNumChannels = Cast<_uint>(Anim.vecChannels.size());
            ofs.write(ReCast<const _char*>(&iNumChannels), sizeof(_uint));
            for (_uint j = 0; j < iNumChannels; j++)
            {
                TChannel& Channel = Anim.vecChannels[j];
                
                // ID
                ofs.write(ReCast<const _char*>(&Channel.iBoneID), sizeof(_uint));

                // 이름
                _uint iStrLength = Cast<_uint>(Channel.strName.length());
                ofs.write(ReCast<const _char*>(&iStrLength), sizeof(_uint));
                if (iStrLength > 0)
                    ofs.write(ReCast<const _char*>(Channel.strName.data()), sizeof(_tchar) * (iStrLength + 1));

                // 키프레임
                _uint iNumKeyFrames = Cast<_uint>(Channel.vecKeyFrames.size());
                ofs.write(ReCast<const _char*>(&iNumKeyFrames), sizeof(_uint));
                if (iNumKeyFrames > 0)
                    ofs.write(ReCast<const _char*>(Channel.vecKeyFrames.data()), sizeof(TKeyFrame) * iNumKeyFrames);
            }
        }


    }
    else
    {
        ofs.close();
        return E_FAIL;
    }

    ofs.close();

    return S_OK;
}

HRESULT CImGuiWin_Convert::Load_Binary(const wstring& strPath, CModelLoad* pModel)
{
    ifstream ifs;

    ifs.open(strPath.c_str(), ios::binary);
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
                ifs.read(ReCast<_char*>(&Anim.strName[0]), sizeof(_tchar)* (iStrLength + 1));

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
    else
    {
        ifs.close();
        return E_FAIL;
    }

    ifs.close();
    return S_OK;
}
