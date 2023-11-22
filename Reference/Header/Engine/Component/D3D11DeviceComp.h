#pragma once

#include "InternalComponent.h"

/// <summary>
/// ���̷�Ʈ11 �׷��� ��ġ�� �����ϴ� ������Ʈ.
/// ������Ÿ���� �����ϰ�
/// </summary>
class CD3D11DeviceComp : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CD3D11DeviceComp)

	struct FInitData
	{
		ID3D11Device* pDevice;
		ID3D11DeviceContext* pContext;

		FInitData& operator=(const FInitData& tData)
		{
			this->pDevice = tData.pDevice;
			this->pContext = tData.pContext;

			return (*this);
		}
	};
protected:
	explicit CD3D11DeviceComp() = default;
	explicit CD3D11DeviceComp(const CD3D11DeviceComp& rhs);
	virtual ~CD3D11DeviceComp() = default;

protected:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;		// ������Ÿ�� ������
public:
	virtual HRESULT Initialize(void* Arg = nullptr) override;				// �Ϲ� �ʱ�ȭ, Ŭ�п�

public:
	static	CD3D11DeviceComp* Create();										// ������Ÿ�� ������
	virtual CComponent* Clone(void* Arg = nullptr) override;				// ����

protected:
	virtual void	Free() {}

public:
	GETSET_1(ID3D11Device*, m_pDevice.Get(), Device, GET_REF_C)
	GETSET_1(ID3D11DeviceContext*, m_pContext.Get(), Context, GET_REF_C)

private:
	ComPtr<ID3D11Device>			m_pDevice = { nullptr };		// ���� ��ġ
	ComPtr<ID3D11DeviceContext>		m_pContext = { nullptr };		// ���� ��ġ ���ؽ�Ʈ
};

