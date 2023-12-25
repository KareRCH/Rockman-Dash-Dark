#pragma once

#include <System/Define/ModelMgr_Define.h>
#include <System/Data/ModelData.h>
#include <System/Data/MeshData.h>
#include <System/Data/BoneData.h>
#include <System/Data/BoneAnimData.h>


BEGIN(Engine)

/// <summary>
/// Assimp라이브러리를 사용해 모델을 불러오는 클래스
/// 메쉬, 뼈, 애니메이션, 머터리얼 정보를 저장합니다.
/// 메쉬 : 정점과 인덱스, 뼈에 대한 웨이트를 저장합니다.
/// 뼈 : 뼈의 ID, 행렬을 저장하여 메쉬가 뼈에 대해 접근하기 위한 요소를 제공합니다. 계층 구조를 띕니다.
/// 애니메이션 : 뼈에 대해 시간선에 따른 변환 행렬을 제공합니다.
/// 머터리얼 : 재질에 대한 정보를 저장합니다. 필요하다면 텍스처 정보 또한 저장합니다.
/// 
/// [연계]
/// 텍스처 매니저 : 로드한 머터리얼의 파일 경로를 저장하여 텍스처를 로드해줍니다.
/// </summary>
class ENGINE_DLL_DBG CModelMgr final : public CBase
{
	DERIVED_CLASS(CBase, CModelMgr)

private:
	explicit CModelMgr() = default;
	explicit CModelMgr(const CModelMgr& rhs) = delete;
	virtual ~CModelMgr() = default;

public:
	HRESULT				Initialize(const wstring& strMainDir);

public:
	static CModelMgr*	Create(const wstring& strMainDir);
private:
	virtual void		Free() override;


private:
	wstring	m_strMainDir = { TEXT("") };			// 참조할 메인 디렉터리


public:
	HRESULT Load_Binary(const wstring& strFileName, class CModelLoad* pModel);
	HRESULT Load_Model(const EModelGroupIndex eGroupIndex, const wstring& strFileName);
	void Load_Meshes(FModelData* pModelData, class CModelLoad* pModel);
	void Load_Materials(FModelData* pModelData, class CModelLoad* pModel);
	void Load_Animations(FModelData* pModelData, class CModelLoad* pModel);
	void Load_Bones(FModelData* pModelData, class CModelLoad* pModel);

public:
	const FMeshData* const	Find_MeshData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const wstring& strMeshKey, const _uint iRangeIndex);
	const FMeshData* const	Find_MeshData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const _uint iIndex);
	CBoneGroup*				Clone_BoneGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);

public:
	const FModelData* const	Find_ModelData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);
	FModelData*				Add_ModelData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);

public:
	const FMaterialData* const Find_MaterialData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const _uint iIndex);

public:
	CMeshGroup*				Find_MeshGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);
	CBoneGroup*				Find_BoneGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);
	CBoneAnimGroup*			Find_AnimGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);
	CMaterialGroup*			Find_MaterialGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);

private:
	const aiScene*		m_pScene = nullptr;				// 내부 통신용 씬 저장변수
	aiNode*				m_pRootArmature;				// 내부용, 아마추어 노드

	string				m_strLoadFilePath;				// 내부용, 로드한 파일 경로.
	_int				m_iNodeID;						// 내부용, 노드 번호 부여용.
	_uint				m_iMaterialCount = 0U;
	_uint				m_iBoneCount = 0U;

private:
	// 모델 정보, 메쉬, 뼈, 머터리얼, 애니메이션
	_unmap<wstring, FModelData*>	m_mapModelDatas[ECast(EModelGroupIndex::Size)];

};

END