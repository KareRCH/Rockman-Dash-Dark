#pragma once

#include "System/Define/ModelMgr_Define.h"
#include "System/Data/MeshData.h"
#include "System/Data/BoneData.h"
#include "System/Data/BoneAnimData.h"


BEGIN(Engine)

/// <summary>
/// � �𵨿� ���� �׷�
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
	_bool			bLoaded = false;						// �ε� �Ǿ��°�

	FMeshGroup*		pMeshGroup = { nullptr };				// �޽��� ��Ƴ��� �׷�
	FBoneAnimGroup* pAnimGroup = { nullptr };				// �ִϸ��̼� �׷�
	FBoneGroup*		pBoneGroup = { nullptr };				// ��� ���� �׷�
};

/// <summary>
/// Assimp���̺귯���� ����� ���� �ҷ����� Ŭ����
/// �޽�, ��, �ִϸ��̼�, ���͸��� ������ �����մϴ�.
/// �޽� : ������ �ε���, ���� ���� ����Ʈ�� �����մϴ�.
/// �� : ���� ID, ����� �����Ͽ� �޽��� ���� ���� �����ϱ� ���� ��Ҹ� �����մϴ�. ���� ������ ��ϴ�.
/// �ִϸ��̼� : ���� ���� �ð����� ���� ��ȯ ����� �����մϴ�.
/// ���͸��� : ������ ���� ������ �����մϴ�. �ʿ��ϴٸ� �ؽ�ó ���� ���� �����մϴ�.
/// 
/// [����]
/// �ؽ�ó �Ŵ��� : �ε��� ���͸����� ���� ��θ� �����Ͽ� �ؽ�ó�� �ε����ݴϴ�.
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
	string	m_strMainDir = "";			// ������ ���� ���͸�

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
	const aiScene*		m_pScene = nullptr;				// ���� ��ſ� �� ���庯��
	vector<FMeshData*>	m_vecMesh;						// ���� ��ſ� �޽� ���庯��
	aiNode*				m_pRootArmature;				// ���ο�, �Ƹ��߾� ���
	_int				m_iNodeID;						// ���ο�, ��� ��ȣ �ο���.
	_uint				m_iMaterialCount = 0U;
	_uint				m_iBoneCount = 0U;

private:
	// �� ����, �޽�, ��, ���͸���, �ִϸ��̼�
	_unmap<wstring, FModelGroup*>	m_mapModelGroup[Cast_EnumDef(EModelGroupIndex::Size)];

};

END