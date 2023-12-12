#pragma once

#include "Base.h"
#include "Component/D3D11DeviceComp.h"

BEGIN(Engine)

/// <summary>
/// 네비게이션의 각 삼각형에 대한 정보를 저장하는 클래스
/// 
/// </summary>
class ENGINE_DLL CNaviCell : public CBase, public ID3D11DeviceComp
{
	DERIVED_CLASS(CBase, CNaviCell)
protected:
	explicit CNaviCell() = default;
	explicit CNaviCell(const CNaviCell& rhs);
	virtual ~CNaviCell() = default;

public:
	virtual HRESULT Initialize(const _float3* pPoints);


#ifdef _DEBUG
	HRESULT Render(class CEffectComponent* pEffectComp);
#endif // _DEBUG


public:
	static CNaviCell* Create(const _float3* pPoints);

protected:
	virtual void		Free() override;

#pragma region 디바이스 컴포넌트
private:
	virtual ID3D11Device* const D3D11Device() const { return m_pDeviceComp->Get_Device(); }
	virtual ID3D11DeviceContext* const D3D11Context() const { return m_pDeviceComp->Get_Context(); }

private:
	CD3D11DeviceComp* m_pDeviceComp = { nullptr };
#pragma endregion

private:
	_float3		m_vPoints[3] = {};

#ifdef _DEBUG
private:
	class CNaviCellBufferComp* m_pVIBuffer = { nullptr };
#endif // _DEBUG


};

END