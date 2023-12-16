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

	ImGui::Begin(u8"터레인");

	if (ImGui::BeginTabBar(u8"터레인 모드"))
	{
		if (ImGui::BeginTabItem(u8"생성"))
		{
			m_eEditMode = EEditMode::Create;

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"편집"))
		{
			m_eEditMode = EEditMode::Edit;

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"브러쉬"))
		{
			m_eEditMode = EEditMode::Brush;

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"높이"))
		{
			m_eEditMode = EEditMode::Brush;

			ImGui::EndTabItem();
		}
	}ImGui::EndTabBar();

	switch (m_eEditMode)
	{
	case EEditMode::Create:
		Layout_TerrainCreate(fTimeDelta);
		break;
	case EEditMode::Edit:
		Layout_TerrainEdit(fTimeDelta);
		break;
	case EEditMode::Brush:
		Layout_TerrainBrush(fTimeDelta);
		break;
	case EEditMode::Height:
		Layout_TerrainHeight(fTimeDelta);
		break;
	}
		

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

	Safe_Release(m_pPickedTerrain);
	Safe_Release(m_pDeviceComp);
}

void CImGuiWin_Terrain::Layout_TerrainCreate(const _float& fTimeDelta)
{
	// 생성 레이아웃을 다룸
	_bool	bCreate = false;

	ImGui::Text(u8"정점 개수");

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

	ImGui::Button(u8"Z##Terrrain VertexY Button");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::SameLine();
	ImGui::PopStyleVar();
	ImGui::PushItemWidth(80.f);
	if (ImGui::InputInt(u8"##Terrain VertexZ", &m_ivTerrainVertex_CountZ, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
	{

	}
	ImGui::PopItemWidth();

	// 더미
	ImGui::Dummy(ImVec2(0.f, 30.f));




	// 터레인 크기
	ImGui::Text(u8"크기");

	ImGui::Button(u8"X##Terrrain Width Button");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::SameLine();
	ImGui::PopStyleVar();
	ImGui::PushItemWidth(80.f);
	if (ImGui::InputInt(u8"##Terrain Width", &m_ivTerrainMaxWidth, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
	{

	}
	ImGui::PopItemWidth();


	ImGui::Separator();
	if (ImGui::Button(u8"생성##Terrain Create"))
	{
		bCreate = true;
	}


	// 변경 사항을 적용한다.
	if (bCreate
		&& m_ivTerrainVertex_CountX > 0 && m_ivTerrainVertex_CountZ > 0
		&& m_ivTerrainMaxWidth > 0)
	{
		CTerrain::FInitTerrain tInit = {};
		tInit.strHeightMapPath = TEXT("");
		tInit.iNumVertexCountX = m_ivTerrainVertex_CountX;
		tInit.iNumVertexCountZ = m_ivTerrainVertex_CountZ;
		tInit.iMaxWidth = m_ivTerrainMaxWidth;
		tInit.pShaderMacro = SHADER_MACRO_TOOL::Desc;

		GI()->Add_GameObject(m_pPickedTerrain = CTerrain::Create(tInit));
		Safe_AddRef(m_pPickedTerrain);

		// 텍스처를 바인딩
		CTerrainModelComp* pTerrainModel = m_pPickedTerrain->Get_Component<CTerrainModelComp>(TEXT("TerrainModelComp"));
		pTerrainModel->Bind_Texture(CTerrainModelComp::TYPE_DIFFUSE, TEXT("Textures/Study/Terrain/Grass_1.dds"));
	}
}

void CImGuiWin_Terrain::Layout_TerrainEdit(const _float& fTimeDelta)
{
	// 만들어진 터레인이 있을 때 수정하는 코드이다.
	if (nullptr == m_pPickedTerrain)
	{
		ImGui::Text(u8"선택된 터레인이 없습니다.");
		return;
	}

	_bool	bIsChanged = false;

	ImGui::Text(u8"정점 개수");

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

	// 더미
	ImGui::Dummy(ImVec2(0.f, 30.f));




	// 터레인 크기
	ImGui::Text(u8"크기");

	ImGui::Button(u8"X##Terrrain Width Button");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::SameLine();
	ImGui::PopStyleVar();
	ImGui::PushItemWidth(80.f);
	if (ImGui::InputInt(u8"##Terrain Width", &m_ivTerrainMaxWidth, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		bIsChanged = true;
	}
	ImGui::PopItemWidth();



	// 변경 사항을 적용한다.
	if (bIsChanged)
	{
		CTerrain::FInitTerrain tInit = {};
		tInit.strHeightMapPath = TEXT("");
		tInit.iNumVertexCountX = m_ivTerrainVertex_CountX;
		tInit.iNumVertexCountZ = m_ivTerrainVertex_CountZ;
		tInit.iMaxWidth = m_ivTerrainMaxWidth;
		m_pPickedTerrain->Create_Terrain(tInit);
	}


	if (ImGui::Button(u8"저장##Terrain Save File"))
	{
		Terrain_SaveFile();
	}
}

void CImGuiWin_Terrain::Layout_TerrainBrush(const _float& fTimeDelta)
{
	// 선택된 터레인이 있을 때 버튼을 눌러 편집 모드로 들어갈 수 있음.
	if (nullptr != m_pPickedTerrain)
	{
		if (!m_bIsEditing)
		{
			ImGui::Text(u8"버튼을 눌러 터레인을 수정");
			if (ImGui::Button(u8"터레인 수정"))
			{
				m_bIsEditing = true;
			}
			return;
		}
	}

	// 만들어진 터레인이 있을 때 수정하는 코드이다.
	if (false == IsCanEdit_Terrain())
	{
		ImGui::Text(u8"선택된 터레인이 없습니다.");
		return;
	}

}

void CImGuiWin_Terrain::Layout_TerrainHeight(const _float& fTimeDelta)
{
	// 선택된 터레인이 있을 때 버튼을 눌러 편집 모드로 들어갈 수 있음.
	if (nullptr != m_pPickedTerrain)
	{
		if (!m_bIsEditing)
		{
			ImGui::Text(u8"버튼을 눌러 터레인을 수정");
			if (ImGui::Button(u8"터레인 수정"))
			{
				m_bIsEditing = true;
			}
			return;
		}
	}

	// 만들어진 터레인이 있을 때 수정하는 코드이다.
	if (false == IsCanEdit_Terrain())
	{
		ImGui::Text(u8"선택된 터레인이 없습니다.");
		return;
	}

}

HRESULT CImGuiWin_Terrain::Terrain_SaveFile()
{
	if (m_pPickedTerrain == nullptr)
		return E_FAIL;


	/* 텍스쳐를 생성해보자. */
	D3D11_TEXTURE2D_DESC	TextureDesc = {};

	TextureDesc.Width = m_ivTerrainVertex_CountX;
	TextureDesc.Height = m_ivTerrainVertex_CountZ;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	// 초기화 데이터 입력
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

	// 초기화 데이터 주소, 크기 저장
	InitialData.pSysMem = pPixels;
	InitialData.SysMemPitch = TextureDesc.Width * 4;

	// 텍스처 생성 및 초기화
	ID3D11Texture2D* pTexture2D = { nullptr };
	if (FAILED(D3D11Device()->CreateTexture2D(&TextureDesc, &InitialData, &pTexture2D)))
		return E_FAIL;
	
	// 데이터 임의로 넣고 텍스처 수정 준비
	for (size_t i = 0; i < TextureDesc.Height; i++)
	{
		for (size_t j = 0; j < TextureDesc.Width; j++)
		{
			_uint		iIndex = i * TextureDesc.Width + j;

			pPixels[iIndex] = Cast<_float>(iIndex) / Cast<_float>((TextureDesc.Height * TextureDesc.Width)) * 10.f;
		}
	}

	/* 텍스쳐의 픽셀정보를 내 마음대로 조절해서 */
	D3D11_MAPPED_SUBRESOURCE		MappedSubResource = {};

	D3D11Context()->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubResource);

	memcpy(MappedSubResource.pData, pPixels, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height);

	D3D11Context()->Unmap(pTexture2D, 0);


	// 이미지 저장을 위해 Desc를 준비한다.
	Image image = {};
	image.width = TextureDesc.Width;
	image.height = TextureDesc.Height;
	image.format = TextureDesc.Format;
	ComputePitch(image.format, image.width, image.height, image.rowPitch, image.slicePitch);
	image.pixels = ReCast<_uint_8*>(pPixels);
	
	/* 다시 파일로 저장하기위해서. */
	// 텍스처가 범용적으로 저장되는 경로에 저장하도록 해준다.
	wstring strPath = GI()->Get_TextureMainPath() + TEXT("TestHeight.png");
	// 아무런 변형없이 값 그대로 저장한다.
	if (FAILED(SaveToWICFile(image, WIC_FLAGS_NONE, GetWICCodec(WIC_CODEC_PNG), strPath.c_str())))
		return E_FAIL;
	
	Safe_Delete_Array(pPixels);
	Safe_Release(pTexture2D);

	return S_OK;
}
