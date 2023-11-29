#pragma once

#include "InternalComponent.h"
#include "System/Define/CamViewMgr_Define.h"




BEGIN(Engine)

/// <summary>
/// CamViewMgr�� ���� �����ϵ��� ����� ������Ʈ
/// �ܺο����� CamViewMgr�� �����ϴ� �ϸ� �����ϴ�.
/// �� ������Ʈ������ CamViewMgr�� �Ϻ� ���, ī�޶� ��Ŀ� ���� �͸��� �ٷ��.
/// �� �ܿ��� GI�� ���ؼ� ���۵ȴ�.
/// ���� ������Ʈ���� �� ������Ʈ�� �ʼ������� Ȱ��Ǿ�� �Ѵ�...
/// 
/// [���� �ν��Ͻ� - ķ�� �Ŵ���] ����
/// </summary>
class ENGINE_DLL CCamViewComp : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CCamViewComp)

protected:
	explicit CCamViewComp() = default;
	explicit CCamViewComp(const CCamViewComp& rhs);
	virtual ~CCamViewComp() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static CCamViewComp* Create();
protected:
	// �ε�� �̹� ������� ���̴��� Ȱ���Ѵ�.
	// ���̴� �ڵ�� �̹� ������ �Ǿ� ������� �ִٴ� �����Ͽ� �ε��Ѵ�.
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free();



#pragma region ķ�� �Ŵ���
public:
	const _float4x4		Get_ViewFloat4x4(ECamType eType, ECamNum eNum) const;
	const _matrix		Get_ViewMatrix(ECamType eType, ECamNum eNum) const;

	const _float4x4		Get_ProjFloat4x4(ECamType eType, ECamNum eNum) const;
	const _matrix		Get_ProjMatrix(ECamType eType, ECamNum eNum) const;

	const _float4x4		Get_ProjInvFloat4x4(ECamType eType, ECamNum eNum) const;
	const _matrix		Get_ProjInvMatrix(ECamType eType, ECamNum eNum) const;

public:
	const D3D11_VIEWPORT Get_Viewport(EViewportNum eNum);

private:
	class CCamViewMgr* m_pCamViewMgr = { nullptr };			// ķ�� �Ŵ��� ����  
#pragma endregion


};

END