#include "ImGuiWin/ImGuiWin_Viewer.h"

HRESULT CImGuiWin_Viewer::Initialize()
{
	m_bOpen = true;

	return S_OK;
}

void CImGuiWin_Viewer::Tick(const _float& fTimeDelta)
{
	if (!m_bOpen)
		return;

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

		return nullptr;
	}

	return pInstance;
}

void CImGuiWin_Viewer::Free()
{
	SUPER::Free();

    m_pSRV.Reset();
    m_pPrevSRV.Reset();
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
    ImVec2 contentSize = ImGui::GetContentRegionAvail();
    ImVec2 clipPos = {};
    ImVec2 clipSize = {};
    m_vContentSize = _float2(contentSize.x, contentSize.y);
    _uint iResolutionX = GI()->ResolutionX();
    _uint iResolutionY = GI()->ResolutionY();
    _float fResolutionRatio = GI()->ResolutionRatio();
    _float fTest = m_vContentSize.y * fResolutionRatio;



    // ��� ȭ���� �������� ���ߵ�, ���� ����Ǵ� �ؽ�ó�� ���� �� (�ػ󵵿� ����) â�� ���̿� �����.
    if (m_vContentSize.x >= m_vContentSize.y * fResolutionRatio)
    {
        // ���� �׷����� �ϴ� ��� ���� ����
        GI()->Set_ResolutionX_MaintainRatio(Cast<_uint>(m_vContentSize.x));
        GI()->Set_SystemViewport(0, { 0.f, 0.f, m_vContentSize.x, m_vContentSize.x / fResolutionRatio, 0.f, 1.f });
        clipSize = { 1.f, (m_vContentSize.y * fResolutionRatio) / m_vContentSize.x };
    }
    else
    {
        // ���� �׷����� �ϴ� ��� ���� ����
        GI()->Set_ResolutionY_MaintainRatio(Cast<_uint>(m_vContentSize.y));
        GI()->Set_SystemViewport(0, { 0.f, 0.f, m_vContentSize.y * fResolutionRatio, m_vContentSize.y, 0.f, 1.f });
        clipSize = { m_vContentSize.x / (m_vContentSize.y * fResolutionRatio), 1.f };
    }

    //m_pSRV = GI()->Find_SRV(TEXT("Model/Character/RockVolnutt/Body.png"));

    m_pPrevSRV.Reset();
    if (m_pSRV != nullptr)
        ImGui::Image((void*)m_pSRV.Get(), contentSize, ImVec2(0, 0), clipSize);
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
