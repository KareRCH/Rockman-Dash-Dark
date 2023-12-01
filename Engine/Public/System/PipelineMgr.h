#pragma once

#include "Base.h"
#include "System/Define/PipelineMgr_Define.h"

BEGIN(Engine)

/// <summary>
/// 카메라의 뷰, 행렬을 저장하고, 역행렬을 쉽게 얻을 수 있도록 하는 파이프라인 객체
/// </summary>
class CPipelineMgr : public CBase
{
	DERIVED_CLASS(CBase, CPipelineMgr)

private:
	explicit CPipelineMgr() = default;
	explicit CPipelineMgr(const CPipelineMgr& rhs) = delete;
	virtual ~CPipelineMgr() = default;

public:
	HRESULT				Initialize();
	void				Tick();

public:
	static CPipelineMgr*	Create();
private:
	virtual void		Free();

public:
	void	Active_Camera(ECamType eType, ECamNum eNum) { m_bActiveCams[ECast(eType)][ECast(eNum)] = true; }
	void	Deactive_Camera(ECamType eType, ECamNum eNum) { m_bActiveCams[ECast(eType)][ECast(eNum)] = false; }

public:
	const _float4x4		Get_ViewFloat4x4(ECamType eType, ECamNum eNum) const { return m_matViews[ECast(eType)][ECast(eNum)]; }
	const _matrix		Get_ViewMatrix(ECamType eType, ECamNum eNum) const { return XMLoadFloat4x4(&m_matViews[ECast(eType)][ECast(eNum)]); }
	
	const _float4x4		Get_ProjFloat4x4(ECamType eType, ECamNum eNum) const { return m_matProjs[ECast(eType)][ECast(eNum)]; }
	const _matrix		Get_ProjMatrix(ECamType eType, ECamNum eNum) const { return XMLoadFloat4x4(&m_matProjs[ECast(eType)][ECast(eNum)]); }
	
	const _float4x4		Get_ProjInvFloat4x4(ECamType eType, ECamNum eNum) const;
	const _matrix		Get_ProjInvMatrix(ECamType eType, ECamNum eNum) const;


public:
	void	Set_ViewMatrix(ECamType eType, ECamNum eNum, _float4x4 mat) { m_matViews[ECast(eType)][ECast(eNum)] = mat; }
	void	Set_ViewMatrix(ECamType eType, ECamNum eNum, _matrix mat) { XMStoreFloat4x4(&m_matViews[ECast(eType)][ECast(eNum)], mat); }

	void	Set_ProjMatrix(ECamType eType, ECamNum eNum, _float4x4 mat) { m_matProjs[ECast(eType)][ECast(eNum)] = mat; }
	void	Set_ProjMatrix(ECamType eType, ECamNum eNum, _matrix mat) { XMStoreFloat4x4(&m_matProjs[ECast(eType)][ECast(eNum)], mat); }


private:
	_bool			m_bActiveCams[ECast(ECamType::Size)][ECast(ECamNum::Size)];
	_float4x4		m_matViews[ECast(ECamType::Size)][ECast(ECamNum::Size)];
	_float4x4		m_matProjs[ECast(ECamType::Size)][ECast(ECamNum::Size)];

public:
	const D3D11_VIEWPORT Get_Viewport(EViewportNum eNum) { return m_Viewports[ECast(eNum)]; }

private:
	D3D11_VIEWPORT	m_Viewports[ECast(EViewportNum::Size)];

};

END