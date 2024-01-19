#include "Component/CylinderModelComp.h"

#include "Component/CylinderBufferComp.h"
#include "Component/EffectComponent.h"
#include "Component/TextureComponent.h"

CCylinderModelComp::CCylinderModelComp()
{
	// InternalComponent�� ������ �ڵ� ���� �ϵ��� ����ȴ�.
	// �̵��� �ݵ�� �����Ǿ�� �Ѵٴ� ��Ģ�� ������.
	// �̵鿡 ���� ������ reference�� �̷������.
	NULL_CHECK(m_pVIBufferComp = CCylinderBufferComp::Create());
	NULL_CHECK(m_pEffectComp = CEffectComponent::Create());
	NULL_CHECK(m_pTextureComp = CTextureComponent::Create());
}

CCylinderModelComp::CCylinderModelComp(const CCylinderModelComp& rhs)
{
	// InternalComponent �� ���� ����, Ȥ�� ���� ���簡 �ʿ��� ��� ���⼭ ó���ϵ��� �Ѵ�.
	// ���� ����� �ַ� Device, Pipeline�� ���� �ý��� ���� ������Ʈ�� �ش�ȴ�.
	// �������� ���� ����� ����ȴ�. (����, ���̴�, �ؽ�ó�� ���� �����Ǵ� ���ҽ��� ���� ������Ʈ����
	// ���������� ���� ���簡 �Ͼ��.
	NULL_CHECK(m_pVIBufferComp = Cast<CCylinderBufferComp*>(rhs.m_pVIBufferComp->Clone()));
	NULL_CHECK(m_pEffectComp = Cast<CEffectComponent*>(rhs.m_pEffectComp->Clone()));
	NULL_CHECK(m_pTextureComp = Cast<CTextureComponent*>(rhs.m_pTextureComp->Clone()));
}

HRESULT CCylinderModelComp::Initialize_Prototype(void* Arg)
{
	if (FAILED(__super::Initialize_Prototype(Arg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCylinderModelComp::Initialize_Prototype(FSerialData& InputData)
{
	if (FAILED(__super::Initialize_Prototype(InputData)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCylinderModelComp::Initialize(void* Arg)
{
	return S_OK;
}

HRESULT CCylinderModelComp::Initialize(FSerialData& InputData)
{
	if (FAILED(__super::Initialize(InputData)))
		return E_FAIL;

	return S_OK;
}

void CCylinderModelComp::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CCylinderModelComp::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

}

void CCylinderModelComp::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

}

HRESULT CCylinderModelComp::Render()
{
	Bind_ShaderResources();

	for (_uint i = 0; i < m_iNumActivePasses; i++)
	{
		m_pEffectComp->Begin(m_vecActivePasses[i]);

		m_pVIBufferComp->Bind_Buffer();

		m_pVIBufferComp->Render_Buffer();
	}

	return S_OK;
}

FSerialData CCylinderModelComp::SerializeData_Prototype()
{
	FSerialData Data = SUPER::SerializeData_Prototype();

	return Data;
}

FSerialData CCylinderModelComp::SerializeData()
{
	FSerialData Data = SUPER::SerializeData();

	return Data;
}

CCylinderModelComp* CCylinderModelComp::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CCylinderModelComp Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCylinderModelComp* CCylinderModelComp::Create(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(InputData)))
	{
		MSG_BOX("CCylinderModelComp Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCylinderModelComp::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CCylinderModelComp Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CComponent*>(pInstance);
}

CComponent* CCylinderModelComp::Clone(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(InputData)))
	{
		MSG_BOX("CCylinderModelComp Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CComponent*>(pInstance);
}

void CCylinderModelComp::Free()
{
	SUPER::Free();

	Safe_Release(m_pVIBufferComp);
	Safe_Release(m_pEffectComp);
	Safe_Release(m_pTextureComp);
}


HRESULT CCylinderModelComp::Bind_ShaderResources()
{
	_float4x4 matTemp = {};

	_float4x4 matParent = Calculate_TransformFloat4x4FromParent();
	XMStoreFloat4x4(&matTemp, Transform().Get_TransformMatrix() * XMLoadFloat4x4(&matParent));

	if (FAILED(m_pEffectComp->Bind_Matrix("g_WorldMatrix", &matTemp)))
		return E_FAIL;
	if (FAILED(m_pEffectComp->Bind_Matrix("g_ViewMatrix", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::View, ECamNum::One)))))
		return E_FAIL;
	if (FAILED(m_pEffectComp->Bind_Matrix("g_ProjMatrix", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::Proj, ECamNum::One)))))
		return E_FAIL;
	if (FAILED(m_pTextureComp->Bind_SRVToEffect(m_pEffectComp, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}