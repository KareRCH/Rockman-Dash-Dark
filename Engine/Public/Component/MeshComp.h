#pragma once

#include "Component/InternalComponent.h"

BEGIN(Engine)

/// <summary>
/// CommonModelComp에 쓸 수 있도록 만들어진 버퍼 컴포넌트
/// </summary>
class ENGINE_DLL CMeshComp : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CMeshComp)

protected:
	explicit CMeshComp() = default;
	explicit CMeshComp(const CMeshComp& rhs);
	virtual ~CMeshComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static CMeshComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;

public:
	_bool	IsMeshLoaded() const { return (m_pMeshData != nullptr); }
	_uint	Get_MeshMaterialIndex() const;
	HRESULT Load_Mesh(EModelGroupIndex eGroupIndex, const wstring& strModelFilePath, _uint iIndex);
	// 이펙트에 뼈 행렬 전달 (상수 버퍼 버전)
	HRESULT	Bind_BoneMatricesToEffect(class CEffectComponent* pEffect, const _char* pConstantName, const CBoneGroup& BoneGroup);
	HRESULT Bind_Buffer();
	HRESULT Render_Buffer();

private:
	class FMeshData* m_pMeshData = { nullptr };

};

END