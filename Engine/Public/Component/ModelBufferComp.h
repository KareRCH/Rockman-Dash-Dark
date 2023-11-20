#pragma once

#include "VIBufferComp.h"

#include "System/Define/ModelMgr_Define.h"

BEGIN(Engine)

/// <summary>
/// 모델에 대한 버퍼를 기록하는 컴포넌트
/// 애니메이션까지 적용할지는 생각해봄
/// </summary>
class ENGINE_DLL CModelBufferComp : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CModelBufferComp)
protected:
	explicit CModelBufferComp(const DX11DEVICE_T tDevice);
	explicit CModelBufferComp(const CModelBufferComp& rhs);
	virtual ~CModelBufferComp() = default;

public:
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey, const wstring& strModelKey);
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual _int	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual void	Render() override;

public:
	static CModelBufferComp* Create(const DX11DEVICE_T tDevice);
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free();
};

END