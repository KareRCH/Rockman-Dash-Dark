#include "ImGuiWin/ImGuiWin_MapTool.h"

#include "Utility/RapidJsonSerial.h"
#include "BaseClass/GameObject.h"
#include "GameObject/GameObjectFactory.h"
#include "ImGuiFileDialog.h"
#include "ImGuiWin/ImGuiWin_Hierarchi.h"
#include "ImGuizmo.h"

HRESULT CImGuiWin_MapTool::Initialize()
{
	m_bOpen = true;
	m_bFirstLoop = true;

    m_pPickedObjects.reserve(100);
    m_Actions.Add_Action(EAction::CancelMove);
    m_Actions.Add_Action(EAction::ConfirmMove);

    m_pPipelineComp = Cast<CPipelineComp*>(GI()->Reference_PrototypeComp(L"CamViewComp"));

    TLIGHT_DESC			LightDesc{};
    LightDesc.eType = TLIGHT_DESC::TYPE_DIRECTIONAL;
    LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
    LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
    LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
    LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

    if (FAILED(GI()->Add_Light(LightDesc)))
        return E_FAIL;

	return S_OK;
}

void CImGuiWin_MapTool::Tick(const _float& fTimeDelta)
{
    if (!m_bOpen)
        return;

    ImGuiWindowFlags iMain_Flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar;
    ImGuiDockNodeFlags DockSpace_Flags = ImGuiDockNodeFlags_NoDockingInCentralNode;

    ImGui::Begin("Center", NULL, iMain_Flags);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("DockSpace_MapTool");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), DockSpace_Flags);

        if (m_bFirstLoop)
        {
            m_bFirstLoop = false;
            ImGui::DockBuilderRemoveNode(dockspace_id);
            ImGui::DockBuilderAddNode(dockspace_id, DockSpace_Flags | ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetWindowSize());
            ImVec2 vDockSize = ImGui::GetWindowSize();

            ImGuiID dock_hierarchi_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 250.f / vDockSize.x, NULL, &dockspace_id);
            ImGuiID dock_property_id = ImGui::DockBuilderSplitNode(dock_hierarchi_id, ImGuiDir_Down, 0.5f, NULL, &dock_hierarchi_id);
            
            ImGui::DockBuilderDockWindow(u8"뷰어", dockspace_id);
            ImGui::DockBuilderDockWindow(u8"계층", dock_hierarchi_id);
            ImGui::DockBuilderDockWindow(u8"속성", dock_property_id);
            ImGui::DockBuilderDockWindow(u8"터레인", dock_property_id);
            ImGui::DockBuilderDockWindow(u8"네비게이션", dock_property_id);

            ImGuiID dock_browser_scene = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 200.f / vDockSize.y, NULL, &dockspace_id);

            ImGui::DockBuilderDockWindow(u8"브라우저", dock_browser_scene);

            ImGui::DockBuilderFinish(dockspace_id);
        }
    }
    else
    {
        //ShowDockingDisabledMessage();
    }

    m_Actions.Reset();
    Shortcut_Manage();

    Layout_MenuBar(fTimeDelta);
    DoMove_PickedObjects(fTimeDelta);

    SUPER::Tick(fTimeDelta);


    ImGui::End();
}

HRESULT CImGuiWin_MapTool::Render()
{
    SUPER::Render();

	return S_OK;
}

CImGuiWin_MapTool* CImGuiWin_MapTool::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ImGuiMgr Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGuiWin_MapTool::Free()
{
	SUPER::Free();

    Safe_Release(m_pPipelineComp);
    for (auto& pObj : m_pPickedObjects)
        Safe_Release(pObj);
}

void CImGuiWin_MapTool::Layout_MenuBar(const _float& fTimeDelta)
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Level"))
        {
            if (ImGui::MenuItem("Open"))
            {
                ImGuiFileDialog::Instance()->OpenDialog("LevelLoad", "Load Level", ".alevel", "../Client/Resource/Levels/");
            }

            if (ImGui::MenuItem("Save"))
            {
                ImGuiFileDialog::Instance()->OpenDialog("LevelSave", "Save Level", ".alevel", "../Client/Resource/Levels/");
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    if (ImGuiFileDialog::Instance()->Display("LevelLoad"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            std::string filter = ImGuiFileDialog::Instance()->GetCurrentFilter();
            // here convert from string because a string was passed as a userDatas, but it can be what you want
            std::string userDatas;
            if (ImGuiFileDialog::Instance()->GetUserDatas())
                userDatas = std::string((const char*)ImGuiFileDialog::Instance()->GetUserDatas());
            auto selection = ImGuiFileDialog::Instance()->GetSelection(); // multiselection

            // action
            Load_Level(ConvertToWstring(filePathName));
        }
        // close
        ImGuiFileDialog::Instance()->Close();
    }

    if (ImGuiFileDialog::Instance()->Display("LevelSave"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            std::string filter = ImGuiFileDialog::Instance()->GetCurrentFilter();
            // here convert from string because a string was passed as a userDatas, but it can be what you want
            std::string userDatas;
            if (ImGuiFileDialog::Instance()->GetUserDatas())
                userDatas = std::string((const char*)ImGuiFileDialog::Instance()->GetUserDatas());
            auto selection = ImGuiFileDialog::Instance()->GetSelection(); // multiselection

            // action
            Save_Level(ConvertToWstring(filePathName));
        }
        // close
        ImGuiFileDialog::Instance()->Close();
    }
}

void CImGuiWin_MapTool::Shortcut_Manage()
{
    // 행동 취소, 모드 돌아오기
    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
    {
        m_Actions[EAction::CancelMove].Act();
        m_bIsMoveFirst = true;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Tab))
    {
        if (m_eToolMode == EMode::Picking)
        {
            m_eToolMode = EMode::Place;
            Escape_MovePickedObjects();
        }
        else if (m_eToolMode == EMode::Place)
            m_eToolMode = EMode::Picking;
    }

    if (ImGui::IsKeyPressed(ImGuiKey_N))
    {
        m_bIsSnap = !m_bIsSnap;
    }

    // 피킹 된게 없으면 MoveMode를 초기화
    if (m_pPickedObjects.empty())
    {
        m_eMoveMode = EMoveMode::None;
        m_bIsMoveFirst = true;
    }

    _bool bIsMoveFirstPrev = m_bIsMoveFirst;
    // 이동관련 단축키
    if (m_eToolMode == EMode::Picking && !m_pPickedObjects.empty())
    {
        // 이동 
        if (ImGui::IsKeyPressed(ImGuiKey_G))
        {
            m_eMoveMode = EMoveMode::Move;
            m_bIsMoveFirst = false;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_R))
        {
            m_eMoveMode = EMoveMode::Rotate;
            m_bIsMoveFirst = false;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_S))
        {
            m_eMoveMode = EMoveMode::Scale;
            m_bIsMoveFirst = false;
        }

        // 편집 축 선택
        if (ImGui::IsKeyPressed(ImGuiKey_X))
        {
            if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
                m_eMoveAxis = EMoveAxis::YZ;
            else
                m_eMoveAxis = EMoveAxis::X;
            m_bIsMoveFirst = false;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_Y))
        {
            if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
                m_eMoveAxis = EMoveAxis::ZX;
            else
                m_eMoveAxis = EMoveAxis::Y;
            m_bIsMoveFirst = false;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_Z))
        {
            if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
                m_eMoveAxis = EMoveAxis::XY;
            else
                m_eMoveAxis = EMoveAxis::Z;
            m_bIsMoveFirst = false;
        }
        
        if (!m_bIsMoveFirst && bIsMoveFirstPrev)
        {
            XMStoreFloat3(&m_vPosition[Origin], m_pPickedObjects[0]->Transform().Get_PositionVector());
            XMStoreFloat3(&m_vRotation[Origin], m_pPickedObjects[0]->Transform().Get_RotationEulerVector());
            XMStoreFloat3(&m_vScale[Origin], m_pPickedObjects[0]->Transform().Get_ScaleVector());

            POINT ptMouse = Get_MousePos_Window(g_hWnd);
            m_vMousePos[Origin] = { (_float)ptMouse.x, (_float)ptMouse.y };

            for (_uint i = 0; i < m_pPickedObjects.size(); i++)
            {
                memcpy(m_vTransforms[i].m[0], &m_pPickedObjects[i]->Transform().Get_PositionFloat3(), sizeof(_float3));
                memcpy(m_vTransforms[i].m[1], &m_pPickedObjects[i]->Transform().Get_RotationEulerFloat3(), sizeof(_float3));
                memcpy(m_vTransforms[i].m[2], &m_pPickedObjects[i]->Transform().Get_ScaleFloat3(), sizeof(_float3));
            }
        }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Enter))
    {
        m_Actions[EAction::ConfirmMove].Act();
        m_bIsMoveFirst = true;
    }
}

void CImGuiWin_MapTool::DoMove_PickedObjects(const _float& fTimeDelta)
{
    if (m_pPickedObjects.empty())
        return;

    /*ImGuizmo::BeginFrame();
    ImGuizmo::IsOver();
    ImGuizmo::Enable(true);*/

    /*if (m_pPickedObjects[0] != nullptr)
    {
        auto pObj = m_pPickedObjects[0];

        float matrixTranslation[3], matrixRotation[3], matrixScale[3];
        _float4x4 matrix = pObj->Transform().Get_TransformFloat4x4();
        _float fMatrix[16] = {};
        memcpy(fMatrix, &matrix, sizeof(fMatrix));
        ImGuizmo::DecomposeMatrixToComponents(fMatrix, matrixTranslation, matrixRotation, matrixScale);
        ImGui::InputFloat3("Tr", matrixTranslation);
        ImGui::InputFloat3("Rt", matrixRotation);
        ImGui::InputFloat3("Sc", matrixScale);
        ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, fMatrix);

        _float4x4 matView = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::View, ECamNum::One);
        _float4x4 matProj = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::Proj, ECamNum::One);
        ImGuiIO& io = ImGui::GetIO();
        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
        ImGuizmo::Manipulate((_float*)&matView, (_float*)&matProj, ImGuizmo::OPERATION::UNIVERSAL, ImGuizmo::MODE::WORLD, fMatrix);
    }*/

    POINT ptMouse = Get_MousePos_Window(g_hWnd);
    m_vMousePos[Dest] = { (_float)ptMouse.x, (_float)ptMouse.y };

    _float2 vSubMouse = { m_vMousePos[Dest].x - m_vMousePos[Origin].x, - m_vMousePos[Dest].y + m_vMousePos[Origin].y };
    _vector vNormalSubMouse = XMVector2Normalize(XMLoadFloat2(&vSubMouse));

    _matrix matView = PipelineComp().Get_CamInvMatrix(ECamType::Persp, ECamMatrix::View, ECamNum::One);
    _vector vRight = XMVector3Normalize(matView.r[0]);
    _vector vUp = XMVector3Normalize(matView.r[1]);
    _vector vLook = XMVector3Normalize(matView.r[2]);
    _vector vPos = matView.r[3];

    _float fDistance = 0.f;
    fDistance += XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vPosition[Origin]) - vPos));

    _vector vAxisX = XMVectorSet(1.f, 0.f, 0.f, 0.f);
    _vector vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);
    _vector vAxisZ = XMVectorSet(0.f, 0.f, 1.f, 0.f);

    switch (m_eMoveMode)
    {
    case EMoveMode::Move:
    {
        _vector vMoveValue = (vRight * vSubMouse.x + vUp * vSubMouse.y) / fDistance;
        if (m_bIsSnap)
            vMoveValue = XMVectorRound(vMoveValue);
        XMStoreFloat3(&m_vPosition[Dest], XMLoadFloat3(&m_vPosition[Origin]) + vMoveValue);
        break;
    }
    case EMoveMode::Rotate:
        XMStoreFloat3(&m_vRotation[Dest], m_pPickedObjects[0]->Transform().Get_RotationEulerVector());
        break;
    case EMoveMode::Scale:
        XMStoreFloat3(&m_vScale[Dest], m_pPickedObjects[0]->Transform().Get_ScaleVector());
        break;
    default:
        break;
    }

    for (_uint i = 0; i < m_pPickedObjects.size(); i++)
    {
        switch (m_eMoveMode)
        {
        case EMoveMode::Move:
            Move_PickedObjects(fTimeDelta, m_pPickedObjects[i], i);
            break;
        case EMoveMode::Rotate:
            Rotate_PickedObjects(fTimeDelta, m_pPickedObjects[i], i);
            break;
        case EMoveMode::Scale:
            Scale_PickedObjects(fTimeDelta, m_pPickedObjects[i], i);
            break;
        default:
            break;
        }
    }

    if (m_Actions.IsAct(EAction::ConfirmMove))
    {
        m_eMoveMode = EMoveMode::None;
        m_eMoveAxis = EMoveAxis::ALL;
    }
    if (m_Actions.IsAct(EAction::CancelMove))
    {
        m_eMoveMode = EMoveMode::None;
        m_eMoveAxis = EMoveAxis::ALL;

        for (_uint i = 0; i < m_pPickedObjects.size(); i++)
        {
            _float3 vResultPos = {};
            memcpy(&vResultPos, m_vTransforms[i].m[0], sizeof(_float3));
            m_pPickedObjects[i]->Transform().Set_Position(vResultPos);
        }
    }
}

void CImGuiWin_MapTool::Move_PickedObjects(const _float& fTimeDelta, CGameObject* pObj, _uint iIndex)
{
    _float3 vPos = {};
    memcpy(&vPos, m_vTransforms[iIndex].m[0], sizeof(_float3));

    _vector vDirection = XMLoadFloat3(&m_vPosition[Dest]) - XMLoadFloat3(&m_vPosition[Origin]);
    pObj->Transform().Set_Position(XMLoadFloat3(&vPos) + vDirection);
    switch (m_eMoveAxis)
    {
    case EMoveAxis::X:
    {
        pObj->Transform().Set_PositionY(vPos.y);
        pObj->Transform().Set_PositionZ(vPos.z);
        break;
    }
    case EMoveAxis::Y:
    {
        pObj->Transform().Set_PositionX(vPos.x);
        pObj->Transform().Set_PositionZ(vPos.z);
        break;
    }
    case EMoveAxis::Z:
    {
        pObj->Transform().Set_PositionX(vPos.x);
        pObj->Transform().Set_PositionY(vPos.y);
        break;
    }
    case EMoveAxis::XY:
    {
        pObj->Transform().Set_PositionZ(vPos.z);
        break;
    }
    case EMoveAxis::YZ:
    {
        pObj->Transform().Set_PositionX(vPos.x);
        break;
    }
    case EMoveAxis::ZX:
    {
        pObj->Transform().Set_PositionY(vPos.y);
        break;
    }
    case EMoveAxis::ALL:
        
        break;
    default:
        break;
    }
}

void CImGuiWin_MapTool::Rotate_PickedObjects(const _float& fTimeDelta, CGameObject* pObj, _uint iIndex)
{
    switch (m_eMoveAxis)
    {
    case EMoveAxis::X:

        break;
    case EMoveAxis::Y:

        break;
    case EMoveAxis::Z:

        break;
    case EMoveAxis::XY:

        break;
    case EMoveAxis::YZ:

        break;
    case EMoveAxis::ZX:

        break;
    case EMoveAxis::ALL:

        break;
    default:
        break;
    }
}

void CImGuiWin_MapTool::Scale_PickedObjects(const _float& fTimeDelta, CGameObject* pObj, _uint iIndex)
{
    switch (m_eMoveAxis)
    {
    case EMoveAxis::X:

        break;
    case EMoveAxis::Y:

        break;
    case EMoveAxis::Z:

        break;
    case EMoveAxis::XY:

        break;
    case EMoveAxis::YZ:

        break;
    case EMoveAxis::ZX:

        break;
    case EMoveAxis::ALL:

        break;
    default:
        break;
    }
}

void CImGuiWin_MapTool::Escape_MovePickedObjects()
{
    // 이동 편집 중이던 작업 취소
}

void CImGuiWin_MapTool::Load_Level(const wstring& strLoadPath)
{
    FSerialData LevelData;
    LevelData.Load_Data(strLoadPath);

    FSerialData ObjectDatas;
    LevelData.Get_Data("CloneObjects", ObjectDatas);

    list<CGameObject*> listLoadedGameObjects;

    _uint iNumObjects = ObjectDatas.Get_ArraySize("Objects");
    for (_uint i = 0; i < iNumObjects; i++)
    {
        FSerialData ObjectData;
        ObjectDatas.Get_ObjectFromArray("Objects", i, ObjectData);

        /*StringBuffer buffer;
        PrettyWriter<StringBuffer> writer(buffer);
        ObjectData.m_Doc.Accept(writer);

        cout << buffer.GetString() << endl;*/

        _uint iClassID = 0;
        if (FAILED(ObjectData.Get_Data("ClassID", iClassID)))
            return;

        CGameObject* pObj = CGameObjectFactory::Create(ObjectData);
        if (pObj == nullptr)
            return;

        listLoadedGameObjects.push_back(pObj);
    }

    // 검증 되었으면 추가한다.
    GI()->Clear_GameObject(TEXT("MapTool"));
    GI()->Set_LevelTag(TEXT("MapTool"));
    CImGuiWin_Hierarchi* pWinHierarchi = { nullptr };
    Find_Child<CImGuiWin_Hierarchi>(&pWinHierarchi);
    if (pWinHierarchi)
        pWinHierarchi->Reset_GameObjectList();
    for (auto iter = listLoadedGameObjects.begin(); iter != listLoadedGameObjects.end(); ++iter)
    {
        GI()->Add_GameObject((*iter));
        (*iter)->TurnOff_State(EGObjectState::Tick);

        if (pWinHierarchi)
            pWinHierarchi->Pushback_GameObject((*iter));
    }
}

void CImGuiWin_MapTool::Set_GameObject(CGameObject* pObj)
{
    Clear_GameObjects();

    if (nullptr == pObj)
        return;

    m_pPickedObjects.push_back(pObj);
    Safe_AddRef(m_pPickedObjects.back());

    m_vTransforms.push_back(_float3x3());
    memcpy(m_vTransforms.back().m[0], &pObj->Transform().Get_PositionFloat3(), sizeof(_float3));
    memcpy(m_vTransforms.back().m[1], &pObj->Transform().Get_RotationEulerFloat3(), sizeof(_float3));
    memcpy(m_vTransforms.back().m[2], &pObj->Transform().Get_ScaleFloat3(), sizeof(_float3));
}

void CImGuiWin_MapTool::Add_GameObject(CGameObject* pObj)
{
    if (nullptr == pObj)
        return;

    auto iter = find_if(m_pPickedObjects.begin(), m_pPickedObjects.end(),
        [&pObj](CGameObject* pSrc) {
            return pSrc == pObj;
        });
    if (iter != m_pPickedObjects.end())
        return;

    m_pPickedObjects.push_back(pObj);
    Safe_AddRef(m_pPickedObjects.back());

    m_vTransforms.push_back(_float3x3());
    memcpy(m_vTransforms.back().m[0], &pObj->Transform().Get_PositionFloat3(), sizeof(_float3));
    memcpy(m_vTransforms.back().m[1], &pObj->Transform().Get_RotationEulerFloat3(), sizeof(_float3));
    memcpy(m_vTransforms.back().m[2], &pObj->Transform().Get_ScaleFloat3(), sizeof(_float3));
}

void CImGuiWin_MapTool::Clear_GameObjects()
{
    for (auto& pObj : m_pPickedObjects)
    {
        Safe_Release(pObj);
    }
    m_pPickedObjects.clear();

    m_vTransforms.clear();
}

void CImGuiWin_MapTool::Save_Level(const wstring& strSavePath)
{
    auto vecGameObjects = GI()->Get_AllGameObjectFromLevel(TEXT("MapTool"));

    FSerialData LevelData;
    LevelData.Add_MemberString("Name", "GamePlay");

    FSerialData PrototypeData;
    
    for (size_t i = 0; i < vecGameObjects.size(); i++)
    {
        auto ObjData = vecGameObjects[i]->SerializeData_Prototype();

        string strProtoName;
        if (FAILED(ObjData.Get_Data("ProtoName", strProtoName)))
            return;

        if (strProtoName.empty())
        {
            MSG_BOX("프로토 이름이 비어있음");
            return;
        }

        _uint iNumComponents = ObjData.Get_ArraySize("Components");
        for (_uint j = 0; j < iNumComponents; j++)
        {
            FSerialData ComponentData;
            ObjData.Get_ObjectFromArray("Components", j, ComponentData);
            PrototypeData.Pushback_Member("Components", ComponentData);

            string strCompProtoName;
            if (FAILED(ComponentData.Get_Data("ProtoName", strCompProtoName)))
                return;

            if (strCompProtoName.empty())
            {
                MSG_BOX("프로토 이름이 비어있음");
                return;
            }
        }

        PrototypeData.Pushback_Member("Objects", ObjData);
    }
    LevelData.Add_Member("Prototypes", PrototypeData);

    FSerialData CloneData;
    for (size_t i = 0; i < vecGameObjects.size(); i++)
    {
        auto ObjData = vecGameObjects[i]->SerializeData();
        CloneData.Pushback_Member("Objects", ObjData);
    }
    LevelData.Add_Member("CloneObjects", CloneData);

    LevelData.Save_Data(strSavePath);
}


