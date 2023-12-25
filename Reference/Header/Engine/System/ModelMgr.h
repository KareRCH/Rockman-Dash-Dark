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
	explicit CModelMgr(const CModelMgr& rhs) = delete;
	virtual ~CModelMgr() = default;

public:
	HRESULT				Initialize(const wstring& strMainDir);

public:
	static CModelMgr*	Create(const wstring& strMainDir);
private:
	virtual void		Free() override;


private:
	wstring	m_strMainDir = { TEXT("") };			// ������ ���� ���͸�


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