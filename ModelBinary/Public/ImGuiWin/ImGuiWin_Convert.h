#pragma once

#include "ImGuiWin.h"

// Assimp 라이브러리
#define AI_LMW_MAX_WEIGHTS 4
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <future>
#include <chrono>

BEGIN(ModelBinary)


struct FMesh
{

};

struct FAnim
{

};

struct FChannel
{

};

struct FBone
{
	wstring strName;
	_float4x4 matTransform;
};

struct FBones
{
	vector<FBone> vecBones;
};

class CImGuiWin_Convert : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_Convert)

protected:
	explicit CImGuiWin_Convert();
	explicit CImGuiWin_Convert(const CImGuiWin_Convert& rhs) = default;
	virtual ~CImGuiWin_Convert() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_Convert* Create();

private:
	virtual void	Free() override;

public:
	void SearchAllFBX(const std::wstring& folderPath);

	void ConvertToBinary();

	void Load_FBX(const wstring& strPath);
	void Load_Bones(const aiScene* pScene);
	void Load_Bone(const aiNode* pNode);
	void Load_Animations(const aiScene* pScene);
	void Load_Meshes(const aiScene* pScene);
	void Load_Materials(const aiScene* pScene);

private:
	string					m_strDirectory = { "" };

	vector<wstring>			m_FBX_Paths;

	const aiScene* m_pAiScene = { nullptr };

	_bool					m_bShowComplete = false;
};

END