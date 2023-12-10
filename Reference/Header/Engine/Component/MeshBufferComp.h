#pragma once

#include "VIBufferComp.h"

BEGIN(Engine)

/// <summary>
/// CommonModelComp�� �� �� �ֵ��� ������� ���� ������Ʈ
/// </summary>
class CMeshBufferComp : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CMeshBufferComp)

protected:
	explicit CMeshBufferComp() = default;
	explicit CMeshBufferComp(const CMeshBufferComp& rhs);
	virtual ~CMeshBufferComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual HRESULT	Render();

public:
	static CMeshBufferComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;

public:
	// ����Ʈ�� �� ��� ���� (��� ���� ����)
	HRESULT	Bind_EffectBoneMatrices(class CEffectComponent* pEffect, const _char* pConstantName);

private:
	class CMeshData* m_pMeshData = { nullptr };

};

END