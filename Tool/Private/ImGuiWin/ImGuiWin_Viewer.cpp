#include "ImGuiWin/ImGuiWin_Viewer.h"

#include "ImGuiWin/ImGuiWin_Terrain.h"
#include "ImGuiWin/ImGuiWin_Browser.h"
#include "BaseClass/Terrain.h"
#include "GameObject/ToolCamera.h"
#include "DirectXTex.h"



CImGuiWin_Viewer::CImGuiWin_Viewer()
{
    NULL_CHECK(m_pGI = GI());
    Safe_AddRef(m_pGI);

    NULL_CHECK(m_pPipelineComp = Cast<CPipelineComp*>(GI()->Reference_PrototypeComp(TEXT("CamViewComp"))));
}

HRESULT CImGuiWin_Viewer::Initialize()
{
	m_bOpen = true;

    GI()->Add_GameObject(m_pCamera = CToolCamera::Create());
    Safe_AddRef(m_pCamera);

    // �׽�Ʈ�� ���� ��ε� ĳ��Ʈ ��������Ʈ

	return S_OK;
}

void CImGuiWin_Viewer::Tick(const _float& fTimeDelta)
{
	if (!m_bOpen)
		return;

    Link_ToTerrainEditor();
    Link_ToBrowser();

	ImGui::Begin(u8"���");
    
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

    // ����� �ؽ�ó SRV�� �����ϱ�
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
	}

	return pInstance;
}

void CImGuiWin_Viewer::Free()
{
	SUPER::Free();

    m_pSRV.Reset();
    m_pPrevSRV.Reset();

    Safe_Release(m_pGI);
    Safe_Release(m_pCamera);
    Safe_Release(m_pPipelineComp);
}

void CImGuiWin_Viewer::Layout_TopBar(const _float& fTimeDelta)
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.ItemSpacing = ImVec2(0, 2);

    ImVec2 vAlignMargin = ImGui::GetContentRegionAvail();
    _float fMarginContent = 400.f;

    // ��� ����
    ImGui::Dummy(ImVec2((vAlignMargin.x - fMarginContent) * 0.5f, 0.f));

    // ��ǥ��
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f); // ��ư�� ��Ȱ��ȭ ���·� ����ϴ�.
    ImGui::SameLine();
    ImGui::Button(u8"��ǥ��");
    ImGui::PopStyleVar();


    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    if (ImGui::BeginCombo(u8"##��ǥ��", m_arrCoord_Item[m_iCoordItem_Index],
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

    // �� ����
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(8.f, 0.f));

    // �Ǻ�
    ImGui::SameLine();
    ImGui::Button(u8"�Ǻ�");

    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    if (ImGui::BeginCombo(u8"##�Ǻ�", m_arrPivot_Item[m_iPivotItem_Index],
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

    // �� ����
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(8.f, 0.f));

    if (m_bSnap_Active)
        Set_Button_ActiveColor();

    // ����
    ImGui::SameLine();
    if (ImGui::Button(u8"����"))
    {
        m_bSnap_Active = !m_bSnap_Active;
    }

    Set_Button_ReturnColor();

    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
    if (ImGui::BeginCombo(u8"##����", m_arrSnap_Item[m_iSnapItem_Index],
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

    // ������ ���� ����
    ImGui::GetStyle().ItemSpacing = ImVec2(8, 4);

    // ��� ����
    ImGui::SameLine();
    ImGui::Dummy(ImVec2((vAlignMargin.x - fMarginContent) * 0.5f, 0.f));
}

void CImGuiWin_Viewer::Layout_View(const _float& fTimeDelta)
{
    // ������ ũ�� ���ϱ�
    ImVec2 contentSize = ImGui::GetContentRegionAvail();    // �����쿡 �� �� �ִ� ���� ����
    ImVec2 clipPos = {};
    ImVec2 clipSize = {};
    m_vViewerSize = _float2(contentSize.x, contentSize.y);
    _uint iResolutionX = GI()->ResolutionX();
    _uint iResolutionY = GI()->ResolutionY();
    _float fResolutionRatio = GI()->ResolutionRatio();
    _float fTest = m_vViewerSize.y * fResolutionRatio;

    // ��� ȭ���� �������� ���ߵ�, ���� ����Ǵ� �ؽ�ó�� ���� �� (�ػ󵵿� ����) â�� ���̿� �����.
    if (m_vViewerSize.x >= m_vViewerSize.y * fResolutionRatio)
    {
        // ���� �׷����� �ϴ� ��� ���� ����
        GI()->Set_ResolutionX_MaintainRatio(Cast<_uint>(m_vViewerSize.x));
        GI()->Set_SystemViewport(0, { 0.f, 0.f, m_vViewerSize.x, m_vViewerSize.x / fResolutionRatio, 0.f, 1.f });
        clipSize = { 1.f, (m_vViewerSize.y * fResolutionRatio) / m_vViewerSize.x };
    }
    else
    {
        // ���� �׷����� �ϴ� ��� ���� ����
        GI()->Set_ResolutionY_MaintainRatio(Cast<_uint>(m_vViewerSize.y));
        GI()->Set_SystemViewport(0, { 0.f, 0.f, m_vViewerSize.y * fResolutionRatio, m_vViewerSize.y, 0.f, 1.f });
        clipSize = { m_vViewerSize.x / (m_vViewerSize.y * fResolutionRatio), 1.f };
    }

    m_pPrevSRV.Reset();
    if (m_pSRV != nullptr)
        ImGui::Image((void*)m_pSRV.Get(), contentSize, ImVec2(0, 0), clipSize);
    
    // ���콺�� ȭ�� �ȿ� �ִ��� üũ
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

    // ��Ÿ ��� ������, ����� ����
    m_vViewerMin = { rcItemRect.Min.x, rcItemRect.Min.y };
    m_vViewerCenter = { (rcItemRect.Min.x + rcItemRect.Max.x) * 0.5f,  (rcItemRect.Min.y + rcItemRect.Max.y) * 0.5f };

    m_vMousePosOnViewer.x = vMouse.x - rcItemRect.Min.x;
    m_vMousePosOnViewer.y = vMouse.y - rcItemRect.Min.y;
    
    // ��ŷ
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
    // ī�޶� ������ �ʱ�ȭ
    m_pCamera->Set_IsCanMove(false);

    // ���콺�� ��� �ȿ� �־�� ��.
    if (!m_bMouseOnViewer)
        return;

    if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
    {
        if (m_pCamera)
        {
            m_pCamera->Set_IsCanMove(true);
            ::SetMousePos(g_hWnd, { Cast<_long>(m_vViewerCenter.x), Cast<_long>(m_vViewerCenter.y) });
        }
    }

    // ����
    if (m_ePickingMode == EPickingMode::Object && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        CGameObject* pPickedObject = { nullptr };

        ComPtr<ID3D11Texture2D> pTexture = { nullptr };
        m_pGI->Copy_RenderTargetViewToTexture_ByViewport(pTexture, 1, 0);

        if (pTexture != nullptr)
        {
            D3D11_TEXTURE2D_DESC Desc = {};
            pTexture->GetDesc(&Desc);

            vector<_float4> vecPosAndIDs;
            vecPosAndIDs.resize(Desc.Width * Desc.Height);
            size_t iSize = sizeof(_float4) * vecPosAndIDs.size();
            
            // ����Ÿ�����κ��� ���� �����͸� ��ȸ�ؼ� ã�� �� �ְ� �ѵڿ� �ʿ��� ���� ã�Ƴ���.
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            m_pGI->Get_GraphicContext()->Map(pTexture.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
            _float4* sourcData = Cast<_float4*>(mappedResource.pData);
            _float4* destData = vecPosAndIDs.data();
            size_t iRowSize = sizeof(_float4) * Desc.Width;
            _uint iMappedRowPitchByte = mappedResource.RowPitch / sizeof(_float4);
            for (_uint i = 0; i < Desc.Height; i++)
            {
                memcpy(destData, sourcData, iRowSize);
                sourcData += iMappedRowPitchByte;
                destData += Desc.Width;
            }
            m_pGI->Get_GraphicContext()->Unmap(pTexture.Get(), 0);

            // ������ �Ű����� ��ŷ�� ����
            _uint iIndex = m_vMousePosOnViewer.x + m_vMousePosOnViewer.y * Desc.Width;
            _float4 test = vecPosAndIDs[iIndex];
            
            _int iObjectID = Cast<_int>(test.w);
            if (iObjectID != -1)
            {
                CGameObject* pObj = m_pGI->Find_GameObjectByID(iObjectID);
                if (pObj != nullptr)
                {
                    pPickedObject = pObj;
                }
            }
        }
        
        pTexture.Reset();
        OnObjectPicked.Broadcast(pPickedObject);
    }

    // �귯��
    if (m_ePickingMode == EPickingMode::Brush)// && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        CGameObject* pPickedObject = { nullptr };
        _float3 vPickedWorldPos = {};

        ComPtr<ID3D11Texture2D> pTexture = { nullptr };
        m_pGI->Copy_RenderTargetViewToTexture_ByViewport(pTexture, 1, 0);

        if (pTexture != nullptr)
        {
            D3D11_TEXTURE2D_DESC Desc = {};
            pTexture->GetDesc(&Desc);

            vector<_float4> vecPosAndIDs;
            vecPosAndIDs.resize(Desc.Width * Desc.Height);
            size_t iSize = sizeof(_float4) * vecPosAndIDs.size();

            // ����Ÿ�����κ��� ���� �����͸� ��ȸ�ؼ� ã�� �� �ְ� �ѵڿ� �ʿ��� ���� ã�Ƴ���.
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            m_pGI->Get_GraphicContext()->Map(pTexture.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
            _float4* sourcData = Cast<_float4*>(mappedResource.pData);
            _float4* destData = vecPosAndIDs.data();
            size_t iRowSize = sizeof(_float4) * Desc.Width;
            _uint iMappedRowPitchByte = mappedResource.RowPitch / sizeof(_float4);
            for (_uint i = 0; i < Desc.Height; i++)
            {
                memcpy(destData, sourcData, iRowSize);
                sourcData += iMappedRowPitchByte;
                destData += Desc.Width;
            }
            m_pGI->Get_GraphicContext()->Unmap(pTexture.Get(), 0);

            // ������ �Ű����� ��ŷ�� ����
            _uint iIndex = m_vMousePosOnViewer.x + m_vMousePosOnViewer.y * Desc.Width;
            _float4 vData = vecPosAndIDs[iIndex];

            _int iObjectID = Cast<_int>(vData.w);
            if (iObjectID != -1)
            {
                CGameObject* pObj = m_pGI->Find_GameObjectByID(iObjectID);
                if (pObj != nullptr)
                {
                    pPickedObject = pObj;                               // ���� ������Ʈ
                    vPickedWorldPos = { vData.x, vData.y, vData.z };    // ���� ��ġ
                }
            }
        }

        pTexture.Reset();
        if (pPickedObject != nullptr)
            OnBrushPicked.Broadcast(vPickedWorldPos);
    }

    if (m_ePickingMode == EPickingMode::Place && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        CGameObject* pPickedObject = { nullptr };
        _float3 vPickedWorldPos = {};

        ComPtr<ID3D11Texture2D> pTexture = { nullptr };
        m_pGI->Copy_RenderTargetViewToTexture_ByViewport(pTexture, 1, 0);

        if (pTexture != nullptr)
        {
            D3D11_TEXTURE2D_DESC Desc = {};
            pTexture->GetDesc(&Desc);

            vector<_float4> vecPosAndIDs;
            vecPosAndIDs.resize(Desc.Width * Desc.Height);
            size_t iSize = sizeof(_float4) * vecPosAndIDs.size();

            // ����Ÿ�����κ��� ���� �����͸� ��ȸ�ؼ� ã�� �� �ְ� �ѵڿ� �ʿ��� ���� ã�Ƴ���.
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            m_pGI->Get_GraphicContext()->Map(pTexture.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
            _float4* sourcData = Cast<_float4*>(mappedResource.pData);
            _float4* destData = vecPosAndIDs.data();
            size_t iRowSize = sizeof(_float4) * Desc.Width;
            _uint iMappedRowPitchByte = mappedResource.RowPitch / sizeof(_float4);
            for (_uint i = 0; i < Desc.Height; i++)
            {
                memcpy(destData, sourcData, iRowSize);
                sourcData += iMappedRowPitchByte;
                destData += Desc.Width;
            }
            m_pGI->Get_GraphicContext()->Unmap(pTexture.Get(), 0);

            // ������ �Ű����� ��ŷ�� ����
            _uint iIndex = m_vMousePosOnViewer.x + m_vMousePosOnViewer.y * Desc.Width;
            _float4 vData = vecPosAndIDs[iIndex];

            _int iObjectID = Cast<_int>(vData.w);
            if (iObjectID != -1)
            {
                CGameObject* pObj = m_pGI->Find_GameObjectByID(iObjectID);
                if (pObj != nullptr)
                {
                    pPickedObject = pObj;                               // ���� ������Ʈ
                    vPickedWorldPos = { vData.x, vData.y, vData.z };    // ���� ��ġ
                }
            }
        }

        pTexture.Reset();
        if (pPickedObject != nullptr)
            OnPlacePicked.Broadcast(vPickedWorldPos);
    }
}

void CImGuiWin_Viewer::Link_ToTerrainEditor()
{
    if (m_bIsLinkedToTerrainEditor || !m_pParentWin)
        return;

    CImGuiWin_Terrain* pTerrainEditor = { nullptr };
    m_pParentWin->Find_Child(&pTerrainEditor);
    if (pTerrainEditor != nullptr)
    {
        pTerrainEditor->Add_ModeSelectedListener(MakeDelegate(this, &ThisClass::Handle_ModeSelected));
        m_bIsLinkedToTerrainEditor = true;
    }
}

void CImGuiWin_Viewer::Link_ToBrowser()
{
    if (m_bIsLinkedToBrowser || !m_pParentWin)
        return;

    CImGuiWin_Browser* pBrowser = { nullptr };
    m_pParentWin->Find_Child(&pBrowser);
    if (pBrowser != nullptr)
    {
        pBrowser->Add_PlaceModeSelectedListener(MakeDelegate(this, &ThisClass::Handle_ModeSelectedPlace));
        m_bIsLinkedToTerrainEditor = true;
    }
}

void CImGuiWin_Viewer::Handle_ModeSelected(_bool bIsEdit)
{
    // ��带 �ٲ۴�.
    if (bIsEdit)
        m_ePickingMode = EPickingMode::Brush;
    else
        m_ePickingMode = EPickingMode::Object;
}

void CImGuiWin_Viewer::Handle_ModeSelectedPlace(_bool bIsPlaceMode)
{
    // ��带 �ٲ۴�.
    if (bIsPlaceMode)
        m_ePickingMode = EPickingMode::Place;
    else
        m_ePickingMode = EPickingMode::Object;
}
