#include "ImGuiWin/ImGuiWin_Convert.h"
#include <shlobj.h>
#include <Shlwapi.h>

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

    for (_uint i = 0; i < m_FBX_Paths.size(); i++)
    {
        vecFutures.push_back(async(launch::async, &CImGuiWin_Convert::Load_FBX, this, m_FBX_Paths[i]));
        //Load_FBX(m_FBX_Paths[i]);

        ifs.open(m_FBX_Paths[i].c_str());
        if (ifs.is_open())
        {
            //cout << "열림" << endl;
        }

        ifs.close();
    }

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

    if (!m_pAiScene || m_pAiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_pAiScene->mRootNode)
    {
#ifdef _DEBUG
        cout << "모델 씬 로드 실패" << endl;
#endif
        return;
    }

    Load_Bones(pScene);
    Load_Animations(pScene);
    Load_Meshes(pScene);
    Load_Materials(pScene);

    cout << "열림" << endl;
    importer.FreeScene();
}

void CImGuiWin_Convert::Load_Bones(const aiScene* pScene)
{
    auto pRootNode = pScene->mRootNode;

    for (_uint i = 0; i < pRootNode->mNumChildren; i++)
    {
        Load_Bone(pRootNode);
    }
}

void CImGuiWin_Convert::Load_Bone(const aiNode* pNode)
{
    for (_uint i = 0; i < pNode->mNumChildren; i++)
    {
        Load_Bone(pNode);
    }
}

void CImGuiWin_Convert::Load_Animations(const aiScene* pScene)
{
    if (!pScene->HasAnimations())
        return;

    for (_uint i = 0; i < pScene->mNumAnimations; i++)
    {
        auto pAnim = pScene->mAnimations[i];

        for (_uint j = 0; j < pAnim->mNumChannels; j++)
        {
            auto pChannel = pAnim->mChannels[j];


        }
    }
}

void CImGuiWin_Convert::Load_Meshes(const aiScene* pScene)
{
    if (!pScene->HasMeshes())
        return;

    for (_uint i = 0; i < pScene->mNumMeshes; i++)
    {
        auto pMesh = pScene->mMeshes[i];


    }

}

void CImGuiWin_Convert::Load_Materials(const aiScene* pScene)
{
    if (!pScene->HasMaterials())
        return;

    for (_uint i = 0; i < pScene->mNumMaterials; i++)
    {
        auto pMaterial = pScene->mMaterials[i];


    }

}
