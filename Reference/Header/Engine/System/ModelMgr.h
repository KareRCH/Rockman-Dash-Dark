#pragma once

#include "System/Data/MeshData.h"
#include "System/Data/BoneData.h"
#include "System/Data/AnimData.h"
#include "System/Data/ModelNodeData.h"

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
	static FModelGroup* Create(const _bool bPermanent, const _bool bLoaded);
	
private:
	virtual void Free() override;
	
public:
	void Set_AllLoaded() { bLoaded = true; }
	void Set_Permanent() { bPermanent = true; }
	void UnSet_Permanent() { bPermanent = false; }

public:
	_bool		bPermanent = false;			// 영구적으로 로드되는 데이터 인가
	_bool		bLoaded = false;			// 로드 되었는가
	FMeshGroup* pMeshGroup = { nullptr };
	FBoneGroup* pBoneGroup = { nullptr };
	FAnimGroup* pAnimGroup = { nullptr };

	vector<FModelRootNodeData*>	vecArmatures;	// 아마추어 루트 노드들
};

/// <summary>
/// Assimp라이브러리를 사용해 모델을 불러오는 클래스
/// 메쉬, 뼈, 애니메이션 정보를 저장합니다.
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
	void	Load_Model(const string& strFileName, const wstring& strGroupKey);
	void	Load_MeshBoneMaterial(FModelGroup* pModelGroup);
	void	Load_Anim(FAnimGroup* pAnimGroup);
	void	Load_Hierarchi(FModelGroup* pModelGroup, aiNode* pArmatureNode);
	void	Load_HierarchiNode(aiNode* pBoneNode, FModelNodeBaseData* pRootNode);


	const FMeshData* const	Get_Mesh(const wstring& strGroupKey, const wstring& strMeshKey);

private:
	FModelGroup* Get_ModelGroup(const wstring& strGroupKey);
	FModelGroup* Add_ModelGroup(const wstring& strGroupKey);

	FMeshGroup* Get_MeshGroup(const wstring& strGroupKey);
	FBoneGroup* Get_BoneGroup(const wstring& strGroupKey);
	FAnimGroup* Get_AnimGroup(const wstring& strGroupKey);

private:
	_matrix ConvertAiMatrix_ToDXMatrix(aiMatrix4x4& matrix);

private:
	const aiScene*		m_pScene = nullptr;				// 내부 통신용 씬 저장변수
	vector<FMeshData*>	m_vecMesh;						// 내부 통신용 메쉬 저장변수
	aiNode*				m_pRootArmature;				// 내부용, 아마추어 노드
	_uint				m_iMaterialCount = 0U;
	_uint				m_iBoneCount = 0U;

private:
	_unmap<wstring, FModelGroup*>	m_mapModelGroup;	// 모델 정보, 메쉬, 뼈, 머터리얼, 애니메이션

};

END