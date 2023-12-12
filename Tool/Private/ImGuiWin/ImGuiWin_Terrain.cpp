#include "ImGuiWin/ImGuiWin_Terrain.h"

#include "BaseClass/Terrain.h"
#include "DirectXTex.h"

HRESULT CImGuiWin_Terrain::Initialize()
{
	m_bOpen = true;

	m_pDeviceComp = Cast<CD3D11DeviceComp*>(GI()->Reference_PrototypeComp(L"GraphicDevComp"));

	return S_OK;
}

void CImGuiWin_Terrain::Tick(const _float& fTimeDelta)
{
	if (!m_bOpen)
		return;

	ImGui::Begin(u8"�ͷ���");

	if (nullptr == m_pTerrain)
		Layout_TerrainCreate(fTimeDelta);
	else
		Layout_TerrainSetting(fTimeDelta);

	SUPER::Tick(fTimeDelta);

	ImGui::End();
}

HRESULT CImGuiWin_Terrain::Render()
{
	SUPER::Render();

	return S_OK;
}

CImGuiWin_Terrain* CImGuiWin_Terrain::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ImGuiMgr Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CImGuiWin_Terrain::Free()
{
	SUPER::Free();

	Safe_Release(m_pTerrain);
	Safe_Release(m_pDeviceComp);
}

void CImGuiWin_Terrain::Layout_TerrainCreate(const _float& fTimeDelta)
{
	// ���� ���̾ƿ��� �ٷ�

	_bool	bCreate = false;

	ImGui::Text(u8"���� ����");

	ImGui::Button(u8"X##Terrrain VertexX Button");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::SameLine();
	ImGui::PopStyleVar();
	ImGui::PushItemWidth(80.f);
	if (ImGui::InputInt(u8"##Terrain VertexX", &m_ivTerrainVertex_CountX, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
	{

	}
	ImGui::PopItemWidth();

	ImGui::SameLine();

	ImGui::Button(u8"Y##Terrrain VertexY Button");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::SameLine();
	ImGui::PopStyleVar();
	ImGui::PushItemWidth(80.f);
	if (ImGui::InputInt(u8"##Terrain VertexY", &m_ivTerrainVertex_CountZ, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
	{

	}
	ImGui::PopItemWidth();

	// ����
	ImGui::Dummy(ImVec2(0.f, 30.f));




	// �ͷ��� ũ��
	ImGui::Text(u8"ũ��");

	ImGui::Button(u8"X##Terrrain Width Button");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::SameLine();
	ImGui::PopStyleVar();
	ImGui::PushItemWidth(80.f);
	if (ImGui::InputInt(u8"##Terrain Width", &m_ivTerrainWidth, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
	{

	}
	ImGui::PopItemWidth();


	ImGui::SameLine();


	ImGui::Button(u8"Y##Terrrain Height Button");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::SameLine();
	ImGui::PopStyleVar();
	ImGui::PushItemWidth(80.f);
	if (ImGui::InputInt(u8"##Terrain Height", &m_ivTerrainHeight, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
	{

	}
	ImGui::PopItemWidth();

	ImGui::Separator();
	if (ImGui::Button(u8"����##Terrain Create"))
	{
		bCreate = true;
	}


	// ���� ������ �����Ѵ�.
	if (bCreate
		&& m_ivTerrainVertex_CountX > 0 && m_ivTerrainVertex_CountZ > 0
		&& m_ivTerrainWidth > 0 && m_ivTerrainHeight > 0)
	{
		GI()->Add_GameObject(m_pTerrain = CTerrain::Create());
		Safe_AddRef(m_pTerrain);

		CTerrain::FInitTerrain tInit = {};
		tInit.strHeightMapPath = TEXT("");
		tInit.iNumVertexCountX = m_ivTerrainVertex_CountX;
		tInit.iNumVertexCountZ = m_ivTerrainVertex_CountZ;
		tInit.iWidthX = m_ivTerrainWidth;
		tInit.iWidthZ = m_ivTerrainWidth;
		tInit.iHeight = m_ivTerrainHeight;
		m_pTerrain->Create_Terrain(tInit);
	}
}

void CImGuiWin_Terrain::Layout_TerrainSetting(const _float& fTimeDelta)
{
	// ������� �ͷ����� ���� �� �����ϴ� �ڵ��̴�.

	_bool	bIsChanged = false;

	ImGui::Text(u8"���� ����");

	ImGui::Button(u8"X##Terrrain VertexX Button");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::SameLine();
	ImGui::PopStyleVar();
	ImGui::PushItemWidth(80.f);
	if (ImGui::InputInt(u8"##Terrain VertexX", &m_ivTerrainVertex_CountX, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		bIsChanged = true;
	}
	ImGui::PopItemWidth();
	
	ImGui::SameLine();

	ImGui::Button(u8"Y##Terrrain VertexY Button");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::SameLine();
	ImGui::PopStyleVar();
	ImGui::PushItemWidth(80.f);
	if (ImGui::InputInt(u8"##Terrain VertexY", &m_ivTerrainVertex_CountZ, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		bIsChanged = true;
	}
	ImGui::PopItemWidth();

	// ����
	ImGui::Dummy(ImVec2(0.f, 30.f));




	// �ͷ��� ũ��
	ImGui::Text(u8"ũ��");

	ImGui::Button(u8"X##Terrrain Width Button");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::SameLine();
	ImGui::PopStyleVar();
	ImGui::PushItemWidth(80.f);
	if (ImGui::InputInt(u8"##Terrain Width", &m_ivTerrainWidth, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		bIsChanged = true;
	}
	ImGui::PopItemWidth();


	ImGui::SameLine();


	ImGui::Button(u8"Y##Terrrain Height Button");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::SameLine();
	ImGui::PopStyleVar();
	ImGui::PushItemWidth(80.f);
	if (ImGui::InputInt(u8"##Terrain Height", &m_ivTerrainHeight, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		bIsChanged = true;
	}
	ImGui::PopItemWidth();


	// ���� ������ �����Ѵ�.
	if (bIsChanged)
	{
		CTerrain::FInitTerrain tInit = {};
		tInit.strHeightMapPath = TEXT("");
		tInit.iNumVertexCountX = m_ivTerrainVertex_CountX;
		tInit.iNumVertexCountZ = m_ivTerrainVertex_CountZ;
		tInit.iWidthX = m_ivTerrainWidth;
		tInit.iWidthZ = m_ivTerrainWidth;
		tInit.iHeight = m_ivTerrainHeight;
		m_pTerrain->Create_Terrain(tInit);
	}


	if (ImGui::Button(u8"����##Terrain Save File"))
	{
		Terrain_SaveFile();
	}
}

HRESULT CImGuiWin_Terrain::Terrain_SaveFile()
{
	if (m_pTerrain == nullptr)
		return E_FAIL;


	/* �ؽ��ĸ� �����غ���. */
	D3D11_TEXTURE2D_DESC	TextureDesc = {};

	TextureDesc.Width = 1025;
	TextureDesc.Height = 1025;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	// �ʱ�ȭ ������ �Է�
	D3D11_SUBRESOURCE_DATA		InitialData = {};
	_float* pPixels = new _float[TextureDesc.Width * TextureDesc.Height];

	for (size_t i = 0; i < TextureDesc.Height; i++)
	{
		for (size_t j = 0; j < TextureDesc.Width; j++)
		{
			_uint		iIndex = i * TextureDesc.Width + j;

			pPixels[iIndex] = 0.f;
		}
	}

	// �ʱ�ȭ ������ �ּ�, ũ�� ����
	InitialData.pSysMem = pPixels;
	InitialData.SysMemPitch = TextureDesc.Width * 4;

	// �ؽ�ó ���� �� �ʱ�ȭ
	ID3D11Texture2D* pTexture2D = { nullptr };
	if (FAILED(D3D11Device()->CreateTexture2D(&TextureDesc, &InitialData, &pTexture2D)))
		return E_FAIL;
	
	// ������ ���Ƿ� �ְ� �ؽ�ó ���� �غ�
	for (size_t i = 0; i < TextureDesc.Height; i++)
	{
		for (size_t j = 0; j < TextureDesc.Width; j++)
		{
			_uint		iIndex = i * TextureDesc.Width + j;

			pPixels[iIndex] = Cast<_float>(iIndex);
		}
	}

	/* �ؽ����� �ȼ������� �� ������� �����ؼ� */
	D3D11_MAPPED_SUBRESOURCE		MappedSubResource = {};

	D3D11Context()->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubResource);

	memcpy(MappedSubResource.pData, pPixels, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height);

	D3D11Context()->Unmap(pTexture2D, 0);


	// �̹��� ������ ���� Desc�� �غ��Ѵ�.
	Image image = {};
	image.width = TextureDesc.Width;
	image.height = TextureDesc.Height;
	image.format = TextureDesc.Format;
	ComputePitch(image.format, image.width, image.height, image.rowPitch, image.slicePitch);
	image.pixels = ReCast<_uint_8*>(pPixels);
	
	/* �ٽ� ���Ϸ� �����ϱ����ؼ�. */
	// �ؽ�ó�� ���������� ����Ǵ� ��ο� �����ϵ��� ���ش�.
	wstring strPath = GI()->Get_TextureMainPath() + TEXT("TestHeight.dds");
	// �ƹ��� �������� �� �״�� �����Ѵ�.
	if (FAILED(SaveToDDSFile(image, DDS_FLAGS_NONE, strPath.c_str())))
		return E_FAIL;

	Safe_Delete_Array(pPixels);
	Safe_Release(pTexture2D);

	return S_OK;
}
