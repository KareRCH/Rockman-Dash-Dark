#pragma once

#include "System/Data/MeshData.h"
#include "System/Data/BoneData.h"
#include "System/Data/AnimData.h"
#include "System/Data/ModelNodeData.h"

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
	static FModelGroup* Create(const _bool bPermanent, const _bool bLoaded);
	
private:
	virtual void Free() override;
	
public:
	void Set_AllLoaded() { bLoaded = true; }
	void Set_Permanent() { bPermanent = true; }
	void UnSet_Permanent() { bPermanent = false; }

public:
	_bool		bPermanent = false;			// ���������� �ε�Ǵ� ������ �ΰ�
	_bool		bLoaded = false;			// �ε� �Ǿ��°�
	FMeshGroup* pMeshGroup = { nullptr };
	FBoneGroup* pBoneGroup = { nullptr };
	FAnimGroup* pAnimGroup = { nullptr };

	vector<FModelRootNodeData*>	vecArmatures;	// �Ƹ��߾� ��Ʈ ����
};

/// <summary>
/// Assimp���̺귯���� ����� ���� �ҷ����� Ŭ����
/// �޽�, ��, �ִϸ��̼� ������ �����մϴ�.
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
	const aiScene*		m_pScene = nullptr;				// ���� ��ſ� �� ���庯��
	vector<FMeshData*>	m_vecMesh;						// ���� ��ſ� �޽� ���庯��
	aiNode*				m_pRootArmature;				// ���ο�, �Ƹ��߾� ���
	_uint				m_iMaterialCount = 0U;
	_uint				m_iBoneCount = 0U;

private:
	_unmap<wstring, FModelGroup*>	m_mapModelGroup;	// �� ����, �޽�, ��, ���͸���, �ִϸ��̼�

};

END