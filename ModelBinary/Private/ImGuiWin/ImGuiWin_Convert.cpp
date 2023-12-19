#include "ImGuiWin/ImGuiWin_Convert.h"
#include <shlobj.h>
#include <Shlwapi.h>

#include "System/GameInstance.h"

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {
    if (uMsg == BFFM_INITIALIZED) {
        // �ʱ� ���� ����
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

    if (ImGui::Button(u8"���� �����ϱ�"))
    {
        BROWSEINFO bi = { 0 };
        LPITEMIDLIST pidl;
        TCHAR szDir[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, szDir);

        bi.hwndOwner = NULL;
        bi.pidlRoot = NULL;
        bi.pszDisplayName = szDir;
        bi.lpszTitle = _T("���� ����");
        bi.ulFlags = BIF_RETURNONLYFSDIRS;
        bi.lpfn = BrowseCallbackProc;
        // ���α׷��� ����� ��θ� �ʱ� ������ �����մϴ�.
        bi.lParam = reinterpret_cast<LPARAM>(szDir);

        pidl = SHBrowseForFolder(&bi);

        if (pidl != NULL) {
            SHGetPathFromIDList(pidl, szDir);
            MessageBox(NULL, szDir, _T("���õ� ����"), MB_OK);
            CoTaskMemFree(pidl);
            wstring text(szDir);
            m_strDirectory = string(text.begin(), text.end());
        }
    }

    ImGui::Text(u8"���õ� ���� : ");
    if (!m_strDirectory.empty())
    {
        ImGui::SameLine();
        ImGui::Text(m_strDirectory.c_str());

    }

    if (ImGui::Button(u8"FBX ã��"))
    {
        m_FBX_Paths.clear();
        SearchAllFBX(Make_Wstring(m_strDirectory));
    }

    ImGui::SameLine();
    if (ImGui::Button(u8"��ȯ"))
    {
        ConvertToBinary();
        m_bShowComplete = true;
        
    }

    // ��ȯ �Ϸ� �޽���
    {
        if (m_bShowComplete) {
            ImGui::OpenPopup(u8"��ȯ��");
        }

        if (ImGui::BeginPopupModal(u8"��ȯ��", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text(u8"��ȯ�� �Ϸ�Ǿ����ϴ�.");

            if (ImGui::Button(u8"Ȯ��")) {
                ImGui::CloseCurrentPopup();
                m_bShowComplete = false;
            }

            ImGui::EndPopup();
        }
    }

    ImGui::Text(u8"Ÿ���� FBX");
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
            // ���� ���� �Ǵ� �۾� ����
            // ���⿡�� �ʿ��� �߰� �۾��� ������ �� �ֽ��ϴ�.

            // ��������� ���� ���� Ž��
            SearchAllFBX(filePath);
        }
        else if (_tcsicmp(PathFindExtension(findFileData.cFileName), _T(".fbx")) == 0) {
            // .fbx ������ ã���� �ش� ��� ���
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
            //cout << "����" << endl;
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
    cout << "�� ����" << endl;
}

void CImGuiWin_Convert::Load_FBX(const wstring& strPath)
{
    Assimp::Importer importer;

    _uint iFlag = 0;
    iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
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

    string strConvert(strPath.begin(), strPath.end());
    const aiScene* pScene = importer.ReadFile(strConvert.c_str(), iFlag);

    if (!m_pAiScene || m_pAiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_pAiScene->mRootNode)
    {
#ifdef _DEBUG
        cout << "�� �� �ε� ����" << endl;
#endif
        return;
    }

    Load_Bones(pScene);
    Load_Animations(pScene);
    Load_Meshes(pScene);
    Load_Materials(pScene);

    cout << "����" << endl;
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
