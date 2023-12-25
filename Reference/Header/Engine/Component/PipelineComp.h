#pragma once

#include "InternalComponent.h"
#include "System/Define/PipelineMgr_Define.h"


BEGIN(Engine)

/// <summary>
/// CamViewMgr에 직접 접근하도록 설계된 컴포넌트
/// 외부에서는 CamViewMgr를 조작하는 일만 가능하다.
/// 이 컴포넌트에서는 CamViewMgr의 일부 기능, 카메라 행렬에 대한 것만을 다룬다.
/// 그 외에는 GI를 통해서 조작된다.
/// 게임 오브젝트에는 이 컴포넌트가 필수적으로 활용되어야 한다...
/// 
/// [게임 인스턴스 - 캠뷰 매니저] 결합
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
	// 로드시 이미 만들어진 셰이더를 활용한다.
	// 셰이더 코드는 이미 컴파일 되어 만들어져 있다는 가정하에 로드한다.
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free();



#pragma region 캠뷰 매니저
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
	class CPipelineMgr* m_pPipelineMgr = { nullptr };			// 캠뷰 매니저 접근  
#pragma endregion


};

END