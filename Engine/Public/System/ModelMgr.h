#pragma once

#include <System/Define/ModelMgr_Define.h>
#include <System/Data/ModelData.h>
#include <System/Data/MeshData.h>
#include <System/Data/BoneData.h>
#include <System/Data/BoneAnimData.h>


BEGIN(Engine)

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
	explicit CModelMgr() = default;
	virtual ~CModelMgr() = default;

public:
	HRESULT	Initialize(const string& strMainDir);

public:
	static CModelMgr* Create(const string& strMainDir);
private:
	virtual void Free() override;


private:
	string	m_strMainDir = { "" };			// ������ ���� ���͸�


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
	const aiScene*		m_pScene = nullptr;				// ���� ��ſ� �� ���庯��
	aiNode*				m_pRootArmature;				// ���ο�, �Ƹ��߾� ���

	string				m_strLoadFilePath;				// ���ο�, �ε��� ���� ���.
	_int				m_iNodeID;						// ���ο�, ��� ��ȣ �ο���.
	_uint				m_iMaterialCount = 0U;
	_uint				m_iBoneCount = 0U;

private:
	// �� ����, �޽�, ��, ���͸���, �ִϸ��̼�
	_unmap<wstring, FModelData*>	m_mapModelDatas[ECast(EModelGroupIndex::Size)];

};

END