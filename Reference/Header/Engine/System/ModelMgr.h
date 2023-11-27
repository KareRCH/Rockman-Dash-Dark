#pragma once

#include "System/Define/ModelMgr_Define.h"
#include "System/Data/MeshData.h"
#include "System/Data/BoneData.h"
#include "System/Data/BoneAnimData.h"


BEGIN(Engine)

/// <summary>
/// 어떤 모델에 대한 그룹
/// </summary>
class FModelGroup final : public CBase
{
	DERIVED_CLASS(CBase, FModelGroup)
private:
	explicit FModelGroup() {}
	virtual ~FModelGroup() = default;

public:
	static FModelGroup* Create( const _bool bLoaded);
	
private:
	virtual void Free() override;
	
public:
	void Set_AllLoaded() { bLoaded = true; }

public:
	_bool			bLoaded = false;						// 로드 되었는가

	FMeshGroup*		pMeshGroup = { nullptr };				// 메쉬를 모아놓은 그룹
	FBoneAnimGroup* pAnimGroup = { nullptr };				// 애니메이션 그룹
	FBoneGroup*		pBoneGroup = { nullptr };				// 노드 정보 그룹
};

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
	explicit CModelMgr();
	virtual ~CModelMgr() = default;

public:
	HRESULT	Initialize(const string& strMainDir);

public:
	static CModelMgr* Create(const string& strMainDir);

private:
	virtual void Free() override;

private:
	string	m_strMainDir = "";			// 참조할 메인 디렉터리

public:
	void	Load_Model(const EModelGroupIndex eGroupIndex, const string& strFileName, const wstring& strGroupKey);
	void	Load_MeshBoneMaterial(FModelGroup* pModelGroup);
	void	Load_Anim(FModelGroup* pModelGroup);
	void	Load_Hierarchi(FBoneGroup* pModelNodeGroup, aiNode* pArmatureNode);
	void	Load_HierarchiNode(FBoneGroup* pModelNodeGroup, aiNode* pBoneNode, FBoneNodeData* pRootNode, FBoneNodeData* pParentNode);


	const FMeshData* const	Get_Mesh(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey, const wstring& strMeshKey);
	FArmatureData*			Find_Armature(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey, const wstring strModelNodeKey);
	FArmatureData*			Clone_Armature(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey, const wstring strModelNodeKey);

private:
	FModelGroup*	Get_ModelGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey);
	FModelGroup*	Add_ModelGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey);

	FMeshGroup*		Get_MeshGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey);
	FBoneGroup*		Get_BoneGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey);
	FBoneAnimGroup* Get_AnimGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey);
	FBoneGroup*		Get_ModelNodeGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey);

private:
	_float4x4	ConvertAiMatrix_ToDXMatrix(aiMatrix4x4& matrix);
	_float3		Calculate_InterpolatedFloat3(_float fAnimTime, const _int iNumKeys, const _vector vVectorKey);
	_float4		Calculate_InterpolatedQuaternion(_float fAnimTime, const _int iNumKeys, const _vector vVectorKey);

private:
	const aiScene*		m_pScene = nullptr;				// 내부 통신용 씬 저장변수
	vector<FMeshData*>	m_vecMesh;						// 내부 통신용 메쉬 저장변수
	aiNode*				m_pRootArmature;				// 내부용, 아마추어 노드
	_int				m_iNodeID;						// 내부용, 노드 번호 부여용.
	_uint				m_iMaterialCount = 0U;
	_uint				m_iBoneCount = 0U;

private:
	// 모델 정보, 메쉬, 뼈, 머터리얼, 애니메이션
	_unmap<wstring, FModelGroup*>	m_mapModelGroup[Cast_EnumDef(EModelGroupIndex::Size)];

};

END