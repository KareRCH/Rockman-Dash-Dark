#pragma once

#include "Base.h"
#include "Component/PipelineComp.h"

BEGIN(Engine)

/// <summary>
/// ÀýµÎÃ¼
/// </summary>
class CFrustum : public CBase
{
	DERIVED_CLASS(CBase, CFrustum)

private:
	CFrustum();
	virtual ~CFrustum() = default;

public:
	HRESULT Initialize();
	void	Tick();

public:
	static CFrustum* Create();
	virtual void Free() override;

public:
	void	Transform_ToLocalSpace(_fmatrix WorldMatrix);
	_bool	IsIn_WorldPlanes(_fvector vPoint, _float fRadius);
	_bool	IsIn_LocalPlanes(_fvector vPoint, _float fRadius);

private:
	class CGameInstance* m_pGI = { nullptr };

#pragma region Ä·ºä ÄÄÆ÷³ÍÆ®
protected:
	CPipelineComp& PipelineComp() { return (*m_pPipelineComp); }

private:
	CPipelineComp* m_pPipelineComp = { nullptr };
#pragma endregion

private:
	_float3			m_vOriginalPoints[8];
	_float4			m_WorldPlanes[6];
	_float4			m_LocalPlanes[6];

private:
	void Make_Planes(const _float3* pPoints, _Out_ _float4* pPlanes);


};

END