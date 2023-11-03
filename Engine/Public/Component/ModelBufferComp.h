#pragma once

#include "VIBufferComp.h"

BEGIN(Engine)

/// <summary>
/// �𵨿� ���� ���۸� ����ϴ� ������Ʈ
/// �ִϸ��̼Ǳ��� ���������� �����غ�
/// </summary>
class ENGINE_DLL CModelBufferComp : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CModelBufferComp)
protected:
	explicit CModelBufferComp(ID3D11Device* pGraphicDev);
	explicit CModelBufferComp(const CModelBufferComp& rhs);
	virtual ~CModelBufferComp() = default;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Initialize(const string& strGroupKey, const string& strModelKey);
	virtual _int	Tick(const _float& fTimeDelta) override;
	virtual void	LateTick() override;
	virtual void	Render(ID3D11DeviceContext* pDeviceContext) override;

public:
	static CModelBufferComp* Create(ID3D11Device* pGraphicDev);
	virtual CPrimitiveComponent* Clone();

protected:
	virtual void	Free();
};

END