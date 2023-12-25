#pragma once

#include "InternalComponent.h"
#include "System/Define/PipelineMgr_Define.h"


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
class ENGINE_DLL CPipelineComp : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CPipelineComp)

protected:
	explicit CPipelineComp() = default;
	explicit CPipelineComp(const CPipelineComp& rhs);
	virtual ~CPipelineComp() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static CPipelineComp* Create();
protected:
	// �ε�� �̹� ������� ���̴��� Ȱ���Ѵ�.
	// ���̴� �ڵ�� �̹� ������ �Ǿ� ������� �ִٴ� �����Ͽ� �ε��Ѵ�.
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free();



#pragma region ķ�� �Ŵ���
public:
	const _float4x4		Get_CamFloat4x4(ECamType eType, ECamMatrix eMatrix, ECamNum eNum) const;
	const _matrix		Get_CamMatrix(ECamType eType, ECamMatrix eMatrix, ECamNum eNum) const;

	const _float4x4		Get_CamInvFloat4x4(ECamType eType, ECamMatrix eMatrix, ECamNum eNum) const;
	const _matrix		Get_CamInvMatrix(ECamType eType, ECamMatrix eMatrix, ECamNum eNum) const;

public:
	void				Set_CamMatrix(ECamType eType, ECamMatrix eMatrix, ECamNum eNum, _float4x4 mat);
	void				Set_CamMatrix(ECamType eType, ECamMatrix eMatrix, ECamNum eNum, _fmatrix mat);

public:
	const _float4		Get_CamPositionFloat4(ECamType eType, ECamNum eNum) const;
	const _vector		Get_CamPositionVector(ECamType eType, ECamNum eNum) const;

public:
	const D3D11_VIEWPORT Get_Viewport(EViewportNum eNum);

private:
	class CPipelineMgr* m_pPipelineMgr = { nullptr };			// ķ�� �Ŵ��� ����  
#pragma endregion


};

END