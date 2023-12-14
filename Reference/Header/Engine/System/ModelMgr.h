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
	virtual ~CModelMgr() = default;

public:
	HRESULT	Initialize(const string& strMainDir);

public:
	static CModelMgr* Create(const string& strMainDir);
private:
	virtual void Free() override;


private:
	string	m_strMainDir = { "" };			// 참조할 메인 디렉터리


public:
	void	Load_Model(const EModelGroupIndex eGroupIndex, const string& strFileName, const wstring& strModelKey);
	void	Load_Mesh(FModelData* pModelData);
	void	Load_Material(FModelData* pModelData);
	void	Load_Animation(FModelData* pModelData);
	void	Load_Hierarchi(FBoneGroup* pBoneGroup, aiNode* pArmatureNode);
	void	Load_HierarchiNode(FBoneGroup* pBoneGroup, aiNode* pBoneNode, FBoneData* pParentNode);

public:
	const FMeshData* const	Find_MeshData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const wstring& strMeshKey);
	FBoneGroup*			Clone_BoneGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);

public:
	const FModelData* const	Find_ModelData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);
	FModelData*				Add_ModelData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);

public:
	FMeshGroup*				Find_MeshGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);
	FBoneGroup*				Find_BoneGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);
	FBoneAnimGroup*			Find_AnimGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);
	FMaterialGroup*			Find_MaterialGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);

private:
	_float4x4				ConvertAiMatrix_ToDXMatrix(aiMatrix4x4& matrix);

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