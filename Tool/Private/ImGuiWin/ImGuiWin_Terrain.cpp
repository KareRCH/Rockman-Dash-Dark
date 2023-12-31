#include "ImGuiWin/ImGuiWin_Terrain.h"

#include "ImGuiWin/ImGuiWin_Viewer.h"
#include "ImGuiWin/ImGuiWin_Browser.h"
#include "System/GameInstance.h"
#include "BaseClass/Terrain.h"

#include "DirectXTex.h"



CImGuiWin_Terrain::CImGuiWin_Terrain()
	: m_pGI(GI())
{
	Safe_AddRef(m_pGI);
}

HRESULT CImGuiWin_Terrain::Initialize()
{
	m_bOpen = true;

	m_pDeviceComp = Cast<CD3D11DeviceComp*>(m_pGI->Reference_PrototypeComp(L"GraphicDevComp"));
	m_pPipelineComp = Cast<CPipelineComp*>(m_pGI->Reference_PrototypeComp(L"CamViewComp"));
	
	

	return S_OK;
}

void CImGuiWin_Terrain::Tick(const _float& fTimeDelta)
{
	if (!m_bOpen)
		return;

	// 부모 창으로부터 뷰어하나에 대해 델리게이트를 연결
	Link_ToViewer();

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
			m_eEditMode = EEditMode::Draw;

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"높이"))
		{
			m_eEditMode = EEditMode::Height;

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
	case EEditMode::Draw:
		Layout_TerrainDraw(fTimeDelta);
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
	Safe_Release(m_pPipelineComp);
	Safe_Release(m_pGI);
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


	// 터레인을 생성한다.
	if (bCreate
		&& m_ivTerrainVertex_CountX > 0 && m_ivTerrainVertex_CountZ > 0
		&& m_ivTerrainMaxWidth > 0)
	{
		Safe_Release(m_pPickedTerrain);

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
		pTerrainModel->Bind_Texture(CTerrainModelComp::TYPE_DIFFUSE, TEXT("Textures/Study/Terrain/Grass_1.dds"), 1);
		pTerrainModel->Bind_Texture(CTerrainModelComp::TYPE_BRUSH, TEXT("Textures/Tool/Brushes/RoundHeightBrush.png"), 1);
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

void CImGuiWin_Terrain::Layout_TerrainDraw(const _float& fTimeDelta)
{
	if (nullptr == m_pPickedTerrain)
		m_bIsEditing = false;

	// 선택된 터레인이 있을 때 버튼을 눌러 편집 모드로 들어갈 수 있음.
	if (nullptr != m_pPickedTerrain)
	{
		if (!m_bIsEditing)
		{
			ImGui::Text(u8"버튼을 눌러 터레인을 수정");
			if (ImGui::Button(u8"터레인 수정"))
			{
				// 모드 바뀜 이벤트 전송
				OnModeSelected.Broadcast(true);
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
	if (nullptr == m_pPickedTerrain)
		m_bIsEditing = false;

	// 선택된 터레인이 있을 때 버튼을 눌러 편집 모드로 들어갈 수 있음.
	if (nullptr != m_pPickedTerrain)
	{
		if (!m_bIsEditing)
		{
			ImGui::Text(u8"버튼을 눌러 터레인을 수정");
			if (ImGui::Button(u8"터레인 수정"))
			{
				// 모드 바뀜 이벤트 전송
				OnModeSelected.Broadcast(true);
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

	if (ImGui::DragFloat(u8"브러쉬 크기", &m_fHeightBrush_Diameter, 0.1f, 0.1f, 1000.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
	{

	}
	if (ImGui::DragFloat(u8"브러쉬 강도", &m_fHeightBrush_Strength, 0.01f, -10.f, 10.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
	{

	}

	HeightBrush_Draw(fTimeDelta);
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
	_float* pPixels = new _float[TextureDesc.Width * TextureDesc.Height];
	size_t iTypeSize = sizeof(_float);

	// 텍스처 생성 및 초기화
	ID3D11Texture2D* pTexture2D = { nullptr };
	if (FAILED(D3D11Device()->CreateTexture2D(&TextureDesc, nullptr, &pTexture2D)))
		return E_FAIL;
	
	// 선택된 터레인의 위치를 옮긴다.
	SHADER_VTX_NORM* pVertices = new SHADER_VTX_NORM[TextureDesc.Width * TextureDesc.Height];
	CTerrainModelComp* pModelComp = m_pPickedTerrain->Get_Component<CTerrainModelComp>(TEXT("TerrainModelComp"));
	pModelComp->Copy_VBuffer(pVertices,TextureDesc.Width * TextureDesc.Height, sizeof(SHADER_VTX_NORM));

	for (size_t i = 0; i < TextureDesc.Height; i++)
	{
		for (size_t j = 0; j < TextureDesc.Width; j++)
		{
			_uint iIndex = j + i * TextureDesc.Height;
			pPixels[iIndex] = pVertices[iIndex].vPosition.y;
		}
	}

	/* 텍스쳐의 픽셀정보를 내 마음대로 조절해서 */
	D3D11_MAPPED_SUBRESOURCE		MappedSubResource = {};
	D3D11Context()->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubResource);

	_float* pSrcData = Cast<_float*>(MappedSubResource.pData);
	size_t iRowSize = iTypeSize * TextureDesc.Width;
	size_t iRowPitchElement = MappedSubResource.RowPitch / iTypeSize;
	memcpy(MappedSubResource.pData, pPixels, MappedSubResource.DepthPitch);

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
	if (SUCCEEDED(CoInitializeEx(nullptr, 0)))
	{
		wstring strPath = GI()->Get_TextureMainPath() + TEXT("TestHeight.png");
		// 아무런 변형없이 값 그대로 저장한다.
		if (FAILED(SaveToWICFile(image, WIC_FLAGS_NONE, GetWICCodec(WIC_CODEC_PNG), strPath.c_str())))
		{
		}
		CoUninitialize();
	}
	CoUninitialize();
	
	Safe_Delete_Array(pPixels);
	Safe_Delete_Array(pVertices);
	Safe_Release(pTexture2D);

	return S_OK;
}

HRESULT CImGuiWin_Terrain::Link_ToViewer()
{
	if (m_bIsLinkedToViewer || !m_pParentWin)
		return E_FAIL;

	CImGuiWin_Viewer* pViewer = { nullptr };
	m_pParentWin->Find_Child(&pViewer);
	if (pViewer != nullptr)
	{
		pViewer->Add_PickedObjectListener(MakeDelegate(this, &ThisClass::Handle_PickedObject));
		pViewer->Add_PickedBrushListener(MakeDelegate(this, &ThisClass::Handle_PickedBrush));
		m_bIsLinkedToViewer = true;
	}

	return S_OK;
}

void CImGuiWin_Terrain::Link_ToBrowser()
{
	if (m_bIsLinkedToBrowser || !m_pParentWin)
		return;

	CImGuiWin_Browser* pBrowser = { nullptr };
	m_pParentWin->Find_Child(&pBrowser);
	if (pBrowser != nullptr)
	{
		pBrowser->Add_PlaceModeSelectedListener(MakeDelegate(this, &ThisClass::Handle_PlaceModeSelected));
		m_bIsLinkedToBrowser = true;
	}
}

void CImGuiWin_Terrain::Handle_PickedObject(CGameObject* pGameObj)
{
	if (pGameObj == nullptr)
	{
		if (!m_bIsEditing)
		{
			Safe_Release(m_pPickedTerrain);
			m_pPickedTerrain = nullptr;
		}
		return;
	}

	CTerrain* pTerrain = DynCast<CTerrain*>(pGameObj);
	if (nullptr != pTerrain)
	{
		m_pPickedTerrain = pTerrain;
		Safe_AddRef(m_pPickedTerrain);
	}
}

void CImGuiWin_Terrain::Handle_PickedBrush(_float3 vPickedWorldPos)
{
	m_vPickedWorldPos = vPickedWorldPos;
	m_bIsClickedOnViewer = true;
}

void CImGuiWin_Terrain::Handle_PlaceModeSelected(_bool bIsSelected)
{
	if (bIsSelected)
	{
		m_bIsEditing = false;
	}
}

void CImGuiWin_Terrain::HeightBrush_Draw(const _float& fTimeDelta)
{
	if (nullptr == m_pPickedTerrain)
		return;

	_bool bIsPicking_Ready = false;
	CTerrainModelComp* pModel = { nullptr };
	pModel = m_pPickedTerrain->Get_Component<CTerrainModelComp>(TEXT("TerrainModelComp"));
	if (pModel)
	{
	    if (S_OK == pModel->IsRender_Ready())
	        bIsPicking_Ready = true;
	}
	
	if (m_bIsClickedOnViewer && bIsPicking_Ready && ImGui::IsMouseDown(ImGuiMouseButton_Left))
	{
		if (m_pPrevPickedTerrain != m_pPickedTerrain)
		{
			m_TerrainVertices.resize(pModel->Get_VertexCount());
			m_TerrainIndices.resize(pModel->Get_IndexCount());
			pModel->Copy_VBuffer(m_TerrainVertices.data(), m_TerrainVertices.size(), sizeof(SHADER_VTX_NORM));
			pModel->Copy_IBuffer(m_TerrainIndices.data(), m_TerrainIndices.size() * sizeof(_uint));
			
			m_iTerrainVertexCountX = pModel->Get_TerrainBufferComp()->Get_VertexCountX();
			m_iTerrainVertexCountZ = pModel->Get_TerrainBufferComp()->Get_VertexCountZ();

			m_pPrevPickedTerrain = m_pPickedTerrain;
		}

		_uint iVertexCount = Cast<_uint>(m_TerrainVertices.size());
		_uint iIndexCount = Cast<_uint>(m_TerrainIndices.size());

	    D3D11_VIEWPORT viewport = *GI()->Get_SystemViewportPtr(0);
	    _float3 vNear, vFar;
	    vNear = { m_vPickedWorldPos.x, m_vPickedWorldPos.y, 0.01f };
	    vFar = { m_vPickedWorldPos.x, m_vPickedWorldPos.y, 1000.f };
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
	    _float fRadius = m_fHeightBrush_Diameter * 0.5f;				// 브러쉬 범위
	    _vector vPlane = XMPlaneFromPointNormal(vsimPos1, vsimNormal);
	    // 교차점 구하기
		_vector vsimInterPoint = XMLoadFloat3(&m_vPickedWorldPos);//XMPlaneIntersectLine(vPlane, vMouseNear, vMouseFar);
		_float3 vInterPoint = {};
	    XMStoreFloat3(&vInterPoint, vsimInterPoint);
	    vInterPoint.y = 0.f;
		vsimInterPoint = XMLoadFloat3(&vInterPoint);

	    if (vPos1.x < vInterPoint.x && vInterPoint.x < vPos2.x
	        && vPos1.z < vInterPoint.z && vInterPoint.z < vPos2.z)
	    {
	        for (_uint i = 0; i < m_TerrainVertices.size(); i++)
	        {
				_float3 vVertexPos = m_TerrainVertices[i].vPosition;
				vVertexPos.y = 0.f;
	            _vector vsimVertexPos = XMLoadFloat3(&vVertexPos);
	            _float f = XMVector3Length(vsimInterPoint - vsimVertexPos).m128_f32[0];
	            if (f <= fRadius)
	                m_TerrainVertices[i].vPosition.y += m_fHeightBrush_Strength * fTimeDelta * (fRadius - f) / fRadius;
	        }

			_uint iNumIndices = 0;
			for (size_t i = 0; i < m_iTerrainVertexCountZ - 1; i++)
			{
				for (size_t j = 0; j < m_iTerrainVertexCountX - 1; j++)
				{
					_uint		iIndex = i * m_iTerrainVertexCountX + j;

					_uint		iIndices[4] = {
						iIndex + m_iTerrainVertexCountX,
						iIndex + m_iTerrainVertexCountX + 1,
						iIndex + 1,
						iIndex
					};

					_vector		vSourDir, vDestDir, vNormal;

					m_TerrainIndices[iNumIndices++] = iIndices[0];
					m_TerrainIndices[iNumIndices++] = iIndices[1];
					m_TerrainIndices[iNumIndices++] = iIndices[2];

					vSourDir = XMLoadFloat3(&m_TerrainVertices[iIndices[1]].vPosition) - XMLoadFloat3(&m_TerrainVertices[iIndices[0]].vPosition);
					vDestDir = XMLoadFloat3(&m_TerrainVertices[iIndices[2]].vPosition) - XMLoadFloat3(&m_TerrainVertices[iIndices[1]].vPosition);
					vNormal = XMVector3Cross(vSourDir, vDestDir);

					vNormal = XMLoadFloat3(&m_TerrainVertices[iIndices[0]].vNormal) + vNormal;
					XMStoreFloat3(&m_TerrainVertices[iIndices[0]].vNormal, XMVector3Normalize(vNormal));

					vNormal = XMLoadFloat3(&m_TerrainVertices[iIndices[1]].vNormal) + vNormal;
					XMStoreFloat3(&m_TerrainVertices[iIndices[1]].vNormal, XMVector3Normalize(vNormal));

					vNormal = XMLoadFloat3(&m_TerrainVertices[iIndices[2]].vNormal) + vNormal;
					XMStoreFloat3(&m_TerrainVertices[iIndices[2]].vNormal, XMVector3Normalize(vNormal));

					m_TerrainIndices[iNumIndices++] = iIndices[0];
					m_TerrainIndices[iNumIndices++] = iIndices[2];
					m_TerrainIndices[iNumIndices++] = iIndices[3];

					vSourDir = XMLoadFloat3(&m_TerrainVertices[iIndices[2]].vPosition) - XMLoadFloat3(&m_TerrainVertices[iIndices[0]].vPosition);
					vDestDir = XMLoadFloat3(&m_TerrainVertices[iIndices[3]].vPosition) - XMLoadFloat3(&m_TerrainVertices[iIndices[2]].vPosition);
					vNormal = XMVector3Cross(vSourDir, vDestDir);

					vNormal = XMLoadFloat3(&m_TerrainVertices[iIndices[0]].vNormal) + vNormal;
					XMStoreFloat3(&m_TerrainVertices[iIndices[0]].vNormal, XMVector3Normalize(vNormal));

					vNormal = XMLoadFloat3(&m_TerrainVertices[iIndices[2]].vNormal) + vNormal;
					XMStoreFloat3(&m_TerrainVertices[iIndices[2]].vNormal, XMVector3Normalize(vNormal));

					vNormal = XMLoadFloat3(&m_TerrainVertices[iIndices[3]].vNormal) + vNormal;
					XMStoreFloat3(&m_TerrainVertices[iIndices[3]].vNormal, XMVector3Normalize(vNormal));
				}
			}

	        pModel->Update_VBuffer(m_TerrainVertices.data(), m_TerrainVertices.size());
	    }
	    

	    // 결과를 XMFLOAT3로 변환
	    //XMStoreFloat3(&intersectionPoint, vInterPoint);
	    _int t = 0;
	}

	m_bIsClickedOnViewer = false;
}
