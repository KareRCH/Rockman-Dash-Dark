#include "ImGuiWin/ImGuiWin_Viewer.h"

#include "ImGuiWin/ImGuiWin_Terrain.h"
#include "BaseClass/Terrain.h"
#include "GameObject/ToolCamera.h"



HRESULT CImGuiWin_Viewer::Initialize()
{
	m_bOpen = true;

    m_pPipelineComp = Cast<CPipelineComp*>(GI()->Reference_PrototypeComp(TEXT("CamViewComp")));

    GI()->Add_GameObject(m_pCamera = CToolCamera::Create());
    Safe_AddRef(m_pCamera);

	return S_OK;
}

void CImGuiWin_Viewer::Tick(const _float& fTimeDelta)
{
	if (!m_bOpen)
		return;

	ImGui::Begin(u8"뷰어");
	
    
    Layout_TopBar(fTimeDelta);
    Layout_View(fTimeDelta);


	SUPER::Tick(fTimeDelta);

	ImGui::End();
}

HRESULT CImGuiWin_Viewer::Render()
{
    SUPER::Render();

    m_pPrevSRV = m_pSRV;
    m_pSRV.Reset();

    // 백버퍼 텍스처 SRV로 복사하기
    GI()->Copy_BackBufferToTexture_ByViewport(m_pSRV, 0);

	return S_OK;
}

CImGuiWin_Viewer* CImGuiWin_Viewer::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CImGuiWin_Viewer Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CImGuiWin_Viewer::Free()
{
	SUPER::Free();

    m_pSRV.Reset();
    m_pPrevSRV.Reset();

    Safe_Release(m_pGuiTerrain);
    Safe_Release(m_pTerrain);
    Safe_Release(m_pCamera);
    Safe_Release(m_pPipelineComp);
}

void CImGuiWin_Viewer::Layout_TopBar(const _float& fTimeDelta)
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.ItemSpacing = ImVec2(0, 2);

    ImVec2 vAlignMargin = ImGui::GetContentRegionAvail();
    _float fMarginContent = 400.f;

    // 가운데 정렬
    ImGui::Dummy(ImVec2((vAlignMargin.x - fMarginContent) * 0.5f, 0.f));

    // 좌표계
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f); // 버튼을 비활성화 상태로 만듭니다.
    ImGui::SameLine();
    ImGui::Button(u8"좌표계");
    ImGui::PopStyleVar();


    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    if (ImGui::BeginCombo(u8"##좌표계", m_arrCoord_Item[m_iCoordItem_Index],
        ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_NoArrowButton))
    {
        for (int i = 0; i < m_arrCoord_Item.size(); i++)
        {
            bool isSelected = (m_iCoordItem_Index == i);
            if (ImGui::Selectable(m_arrCoord_Item[i], isSelected))
            {
                m_iCoordItem_Index = i;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    // 옆 마진
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(8.f, 0.f));

    // 피봇
    ImGui::SameLine();
    ImGui::Button(u8"피봇");

    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    if (ImGui::BeginCombo(u8"##피봇", m_arrPivot_Item[m_iPivotItem_Index],
        ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_NoArrowButton))
    {
        for (int i = 0; i < m_arrPivot_Item.size(); i++) {
            bool isSelected = (m_iPivotItem_Index == i);
            if (ImGui::Selectable(m_arrPivot_Item[i], isSelected)) {
                m_iPivotItem_Index = i;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    // 옆 마진
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(8.f, 0.f));

    if (m_bSnap_Active)
        Set_Button_ActiveColor();

    // 스냅
    ImGui::SameLine();
    if (ImGui::Button(u8"스냅"))
    {
        m_bSnap_Active = !m_bSnap_Active;
    }

    Set_Button_ReturnColor();

    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    if (ImGui::BeginCombo(u8"##스냅", m_arrSnap_Item[m_iSnapItem_Index],
        ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_NoArrowButton))
    {
        for (int i = 0; i < m_arrSnap_Item.size(); i++) {
            bool isSelected = (m_iSnapItem_Index == i);
            if (ImGui::Selectable(m_arrSnap_Item[i], isSelected)) {
                m_iSnapItem_Index = i;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    // 아이템 간격 복원
    ImGui::GetStyle().ItemSpacing = ImVec2(8, 4);

    // 가운데 정렬
    ImGui::SameLine();
    ImGui::Dummy(ImVec2((vAlignMargin.x - fMarginContent) * 0.5f, 0.f));
}

void CImGuiWin_Viewer::Layout_View(const _float& fTimeDelta)
{
    // 콘텐츠 크기 구하기
    ImVec2 contentSize = ImGui::GetContentRegionAvail();    // 윈도우에 들어갈 수 있는 남은 공간
    ImVec2 clipPos = {};
    ImVec2 clipSize = {};
    m_vViewerSize = _float2(contentSize.x, contentSize.y);
    _uint iResolutionX = GI()->ResolutionX();
    _uint iResolutionY = GI()->ResolutionY();
    _float fResolutionRatio = GI()->ResolutionRatio();
    _float fTest = m_vViewerSize.y * fResolutionRatio;

    // 출력 화면은 콘텐츠에 맞추되, 실제 복사되는 텍스처는 가장 긴 (해상도에 맞춘) 창의 길이에 맞춘다.
    if (m_vViewerSize.x >= m_vViewerSize.y * fResolutionRatio)
    {
        // 실제 그려져야 하는 출력 공간 설정
        GI()->Set_ResolutionX_MaintainRatio(Cast<_uint>(m_vViewerSize.x));
        GI()->Set_SystemViewport(0, { 0.f, 0.f, m_vViewerSize.x, m_vViewerSize.x / fResolutionRatio, 0.f, 1.f });
        clipSize = { 1.f, (m_vViewerSize.y * fResolutionRatio) / m_vViewerSize.x };
    }
    else
    {
        // 실제 그려져야 하는 출력 공간 설정
        GI()->Set_ResolutionY_MaintainRatio(Cast<_uint>(m_vViewerSize.y));
        GI()->Set_SystemViewport(0, { 0.f, 0.f, m_vViewerSize.y * fResolutionRatio, m_vViewerSize.y, 0.f, 1.f });
        clipSize = { m_vViewerSize.x / (m_vViewerSize.y * fResolutionRatio), 1.f };
    }

    m_pPrevSRV.Reset();
    if (m_pSRV != nullptr)
        ImGui::Image((void*)m_pSRV.Get(), contentSize, ImVec2(0, 0), clipSize);
    
    // 마우스가 화면 안에 있는지 체크
    POINT ptMouse = Get_MousePos_Client(g_hWnd);
    ImVec2 vMouse = { Cast<_float>(ptMouse.x), Cast<_float>(ptMouse.y) };
    ImVec2 vItemMin = ImGui::GetWindowContentRegionMin() - ImGui::GetWindowPos();
    ImRect rcItemRect = { vItemMin + ImGui::GetItemRectMin(), vItemMin + ImGui::GetItemRectMax() };
    if (rcItemRect.Min.x < vMouse.x && rcItemRect.Max.x > vMouse.x
        && rcItemRect.Min.y < vMouse.y && rcItemRect.Max.y > vMouse.y)
    {
        m_bMouseOnViewer = true;
    }
    else
        m_bMouseOnViewer = false;

    // 기타 뷰어 시작점, 가운데점 저장
    m_vViewerMin = { rcItemRect.Min.x, rcItemRect.Min.y };
    m_vViewerCenter = { (rcItemRect.Min.x + rcItemRect.Max.x) * 0.5f,  (rcItemRect.Min.y + rcItemRect.Max.y) * 0.5f };

    m_vMousePosOnViewer.x = vMouse.x - rcItemRect.Min.x;
    m_vMousePosOnViewer.y = vMouse.y - rcItemRect.Min.y;
    
    // 피킹
    Mouse_Picking(fTimeDelta);
}

void CImGuiWin_Viewer::Set_Button_ActiveColor()
{
    m_pStyle = ImGui::GetStyle();
    ImGuiStyle& style = ImGui::GetStyle();
    m_pStyle.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_Button];
    m_pStyle.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_ButtonHovered];
    m_pStyle.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_ButtonActive];

    style.Colors[ImGuiCol_Button] = ImVec4(0.12f, 0.56f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.70f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.40f, 0.90f, 1.00f);
}

void CImGuiWin_Viewer::Set_Button_NonActiveColor()
{
    m_pStyle = ImGui::GetStyle();
    ImGuiStyle& style = ImGui::GetStyle();
    m_pStyle.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_Button];
    m_pStyle.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_ButtonHovered];
    m_pStyle.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_ButtonActive];

    style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_Button];
    style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_Button];
    style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_Button];
}

void CImGuiWin_Viewer::Set_Button_ReturnColor()
{
    ImGui::GetStyle().Colors[ImGuiCol_Button] = m_pStyle.Colors[ImGuiCol_Button];
    ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = m_pStyle.Colors[ImGuiCol_ButtonHovered];
    ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = m_pStyle.Colors[ImGuiCol_ButtonActive];
}

void CImGuiWin_Viewer::LoadTextureFromFIle(const string& strFileName)
{
    //// Load from disk into a raw RGBA buffer
    //int image_width = 0;
    //int image_height = 0;
    //unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    //if (image_data == NULL)
    //    return;

    //// Create texture
    //D3D11_TEXTURE2D_DESC desc;
    //ZeroMemory(&desc, sizeof(desc));
    //desc.Width = image_width;
    //desc.Height = image_height;
    //desc.MipLevels = 1;
    //desc.ArraySize = 1;
    //desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //desc.SampleDesc.Count = 1;
    //desc.Usage = D3D11_USAGE_DEFAULT;
    //desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    //desc.CPUAccessFlags = 0;

    //ID3D11Texture2D* pTexture = NULL;
    //D3D11_SUBRESOURCE_DATA subResource;
    //subResource.pSysMem = image_data;
    //subResource.SysMemPitch = desc.Width * 4;
    //subResource.SysMemSlicePitch = 0;
    //GI()->Get_GraphicDev()->CreateTexture2D(&desc, &subResource, &pTexture);

    //// Create texture view
    //D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    //ZeroMemory(&srvDesc, sizeof(srvDesc));
    //srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    //srvDesc.Texture2D.MipLevels = desc.MipLevels;
    //srvDesc.Texture2D.MostDetailedMip = 0;
    //GI()->Get_GraphicDev()->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
    //pTexture->Release();

    //stbi_image_free(image_data);

    //return true;
}

void CImGuiWin_Viewer::Mouse_Picking(const _float& fTimeDelta)
{
    // 카메라 움직임 초기화
    m_pCamera->Set_IsCanMove(false);

    // 마우스가 뷰어 안에 있어야 함.
    if (!m_bMouseOnViewer)
        return;

    if (FAILED(Link_GuiTerrain()))
        return;

    if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
    {
        if (m_pCamera)
        {
            m_pCamera->Set_IsCanMove(true);
            ::SetMousePos(g_hWnd, { Cast<_long>(m_vViewerCenter.x), Cast<_long>(m_vViewerCenter.y) });
        }
    }

    // 누름
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        _bool bIsPicking_Ready = false;
        CTerrain* pTerrain = m_pGuiTerrain->Get_Terrain();
        CTerrainModelComp* pModel = { nullptr };
        if (pTerrain)
        {
            pModel = pTerrain->Get_Component<CTerrainModelComp>(TEXT("TerrainModelComp"));
            if (pModel)
            {
                if (S_OK == pModel->IsRender_Ready())
                    bIsPicking_Ready = true;
            }
        }

        if (bIsPicking_Ready)
        {
            m_TerrainVertices.resize(pModel->Get_VertexCount());
            pModel->Copy_VBuffer(m_TerrainVertices.data(), m_TerrainVertices.size(), sizeof(VERTEX_TYPE));

            /*srand(_uint(time(nullptr)));
            for (_uint i = 0; i < m_TerrainVertices.size(); i++)
            {
                m_TerrainVertices[i].vPosition.y = (rand() % m_TerrainVertices.size()) * 0.0001f;
            }

            pModel->Update_VBuffer(m_TerrainVertices.data(), m_TerrainVertices.size());*/
            D3D11_VIEWPORT viewport = *GI()->Get_SystemViewportPtr(0);
            _float3 vNear, vFar;
            vNear = { m_vMousePosOnViewer.x, m_vMousePosOnViewer.y, 0.01f };
            vFar = { m_vMousePosOnViewer.x, m_vMousePosOnViewer.y, 1000.f };
            _matrix matProj = PipelineComp().Get_CamMatrix(ECamType::Persp, ECamMatrix::Proj, ECamNum::One);
            _matrix matView = PipelineComp().Get_CamMatrix(ECamType::Persp, ECamMatrix::View, ECamNum::One);
            //_matrix matWorld = XMMatrixInverse(nullptr, matView);
            _vector vMouseNear = XMVector3Unproject(XMLoadFloat3(&vNear),
                viewport.TopLeftX, viewport.TopLeftY, viewport.Width, viewport.Height, viewport.MinDepth, viewport.MaxDepth,
                matProj, matView, XMMatrixIdentity()
            );
            _vector vMouseFar = XMVector3Unproject(XMLoadFloat3(&vFar),
                viewport.TopLeftX, viewport.TopLeftY, viewport.Width, viewport.Height, viewport.MinDepth, viewport.MaxDepth,
                matProj, matView, XMMatrixIdentity()
            );
            // 카메라에서 쏘는 광선
            _vector vRay = XMVector3Normalize(vMouseFar - vMouseNear);

            // 야매 피킹
            _float3 vPos1 = m_TerrainVertices.front().vPosition;
            vPos1.y = 0.f;
            _float3 vPos2 = m_TerrainVertices.back().vPosition;
            vPos1.y = 0.f;
            _float3 vPos3 = { vPos2.x, 0.f, vPos1.z };
            _vector vsimPos1 = XMLoadFloat3(&vPos1);
            _vector vsimPos2 = XMLoadFloat3(&vPos2);
            _vector vsimPos3 = XMLoadFloat3(&vPos3);
            _vector vsimNormal = XMVectorSet(0.f, 1.f, 0.f, 0.f);

            // 평면을 만들고 교차점을 찾아낸다.
            _float fRadius = 5.f;   // 임시용 브러쉬 범위
            _vector vPlane = XMPlaneFromPointNormal(vsimPos1, vsimNormal);
            // 교차점 구하기
            _vector vsimInterPoint = XMPlaneIntersectLine(vPlane, vMouseNear, vMouseFar);
            _float3 vInterPoint;
            XMStoreFloat3(&vInterPoint, vsimInterPoint);
            vInterPoint.y = 0.f;

            if (vPos1.x < vInterPoint.x && vInterPoint.x < vPos2.x
                && vPos1.z < vInterPoint.z && vInterPoint.z < vPos2.z)
            {
                for (_uint i = 0; i < m_TerrainVertices.size(); i++)
                {
                    _vector vsimVertexPos = XMLoadFloat3(&m_TerrainVertices[i].vPosition);
                    _float f = XMVector3Length(vsimInterPoint - vsimVertexPos).m128_f32[0];
                    if (f <= fRadius)
                        m_TerrainVertices[i].vPosition.y += fTimeDelta;
                }

                pModel->Update_VBuffer(m_TerrainVertices.data(), m_TerrainVertices.size());
            }
            

            // 결과를 XMFLOAT3로 변환
            //XMStoreFloat3(&intersectionPoint, vInterPoint);
            

            _int t = 0;
        }
    }
}

HRESULT CImGuiWin_Viewer::Link_GuiTerrain()
{
    if (m_pGuiTerrain)
        return S_OK;

    if (!m_pParentWin)
        return E_FAIL;

    // 부모가 ImGuiWin_Terrain을 가지고 있어야 끝남.
    if (FAILED(m_pParentWin->Find_Child(&m_pGuiTerrain)))
        return E_FAIL;

    Safe_AddRef(m_pGuiTerrain);

    return S_OK;
}
