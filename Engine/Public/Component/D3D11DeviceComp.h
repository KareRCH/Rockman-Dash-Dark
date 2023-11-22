#pragma once

#include "InternalComponent.h"

/// <summary>
/// 다이렉트11 그래픽 장치를 저장하는 컴포넌트.
/// 프로토타입을 생성하고
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
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;		// 프로토타입 생성용
public:
	virtual HRESULT Initialize(void* Arg = nullptr) override;				// 일반 초기화, 클론용

public:
	static	CD3D11DeviceComp* Create();										// 프로토타입 생성용
	virtual CComponent* Clone(void* Arg = nullptr) override;				// 복제

protected:
	virtual void	Free() {}

public:
	GETSET_1(ID3D11Device*, m_pDevice.Get(), Device, GET_REF_C)
	GETSET_1(ID3D11DeviceContext*, m_pContext.Get(), Context, GET_REF_C)

private:
	ComPtr<ID3D11Device>			m_pDevice = { nullptr };		// 렌더 장치
	ComPtr<ID3D11DeviceContext>		m_pContext = { nullptr };		// 렌더 장치 컨텍스트
};

