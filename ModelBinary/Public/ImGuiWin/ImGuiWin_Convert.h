#pragma once

#include "ImGuiWin.h"

// Assimp 라이브러리
#define AI_LMW_MAX_WEIGHTS 4
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "ModelLoad.h"

#include <future>
#include <chrono>

BEGIN(ModelBinary)

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

	HRESULT Load_Bones(const aiScene* pScene, class CModelLoad* pModel);
	HRESULT Load_Bone(const aiNode* pNode, class CModelLoad* pModel, _uint& iID, const TBone& ParentBone);
	HRESULT Load_Animations(const aiScene* pScene, class CModelLoad* pModel);
	HRESULT Load_Meshes(const aiScene* pScene, class CModelLoad* pModel);
	HRESULT Load_Materials(const aiScene* pScene, class CModelLoad* pModel, const string& strModelFilePath);

	// 바이너리 저장 함수
	HRESULT Save_Binary(const wstring& strPath, class CModelLoad* pModel);
	// 테스트용 바이너리 로드 함수
	HRESULT Load_Binary(const wstring& strPath, class CModelLoad* pModel);

private:
	string					m_strDirectory = { "" };

	vector<wstring>			m_FBX_Paths;
	mutex					m_Mutex;
	_uint					m_iLimit_Async;

	const aiScene*			m_pAiScene = { nullptr };

	_bool					m_bShowComplete = false;
	_bool					m_bIsOnlyAnims = { false };


};

END