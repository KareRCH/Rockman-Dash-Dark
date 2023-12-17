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

#pragma region 카메라
public:
	void	Active_Camera(ECamType eType, ECamNum eNum) { m_bActiveCams[ECast(eType)][ECast(eNum)] = true; }
	void	Deactive_Camera(ECamType eType, ECamNum eNum) { m_bActiveCams[ECast(eType)][ECast(eNum)] = false; }

public:
	const _float4x4		Get_CamFloat4x4(ECamType eType, ECamMatrix eMatrix, ECamNum eNum) const
	{
		if (m_bActiveCams[ECast(eType)][ECast(eNum)])
			return m_CamMatrices[ECast(eType)][ECast(eMatrix)][ECast(eNum)];
		return _float4x4();
	}
	const _matrix		Get_CamMatrix(ECamType eType, ECamMatrix eMatrix, ECamNum eNum) const
	{
		if (m_bActiveCams[ECast(eType)][ECast(eNum)])
			return XMLoadFloat4x4(&m_CamMatrices[ECast(eType)][ECast(eMatrix)][ECast(eNum)]);
		return _matrix();
	}

	const _float4x4		Get_CamInvFloat4x4(ECamType eType, ECamMatrix eMatrix, ECamNum eNum) const
	{
		if (m_bActiveCams[ECast(eType)][ECast(eNum)])
			return m_CamInvMatrices[ECast(eType)][ECast(eMatrix)][ECast(eNum)];
		return _float4x4();
	}
	const _matrix		Get_CamInvMatrix(ECamType eType, ECamMatrix eMatrix, ECamNum eNum) const
	{
		if (m_bActiveCams[ECast(eType)][ECast(eNum)])
			return XMLoadFloat4x4(&m_CamInvMatrices[ECast(eType)][ECast(eMatrix)][ECast(eNum)]);
		return _matrix();
	}


public:
	void	Set_CamMatrix(ECamType eType, ECamMatrix eMatrix, ECamNum eNum, _float4x4 mat)
	{
		if (!m_bActiveCams[ECast(eType)][ECast(eNum)])
			return;
		m_CamMatrices[ECast(eType)][ECast(eMatrix)][ECast(eNum)] = mat;
	}
	void	Set_CamMatrix(ECamType eType, ECamMatrix eMatrix, ECamNum eNum, _fmatrix mat)
	{
		if (!m_bActiveCams[ECast(eType)][ECast(eNum)])
			return;

		XMStoreFloat4x4(&m_CamMatrices[ECast(eType)][ECast(eMatrix)][ECast(eNum)], mat);
	}

public:
	const _float4	Get_CamPositionFloat4(ECamType eType, ECamNum eNum)
	{
		if (!m_bActiveCams[ECast(eType)][ECast(eNum)])
			return _float4();

		return m_CamPositions[ECast(eType)][ECast(eNum)];
	}
	const _vector	Get_CamPositionVector(ECamType eType, ECamNum eNum)
	{
		if (!m_bActiveCams[ECast(eType)][ECast(eNum)])
			return _vector();

		return XMLoadFloat4(&m_CamPositions[ECast(eType)][ECast(eNum)]);
	}


private:
	_bool			m_bActiveCams[ECast(ECamType::Size)][ECast(ECamNum::Size)];
	_float4x4		m_CamMatrices[ECast(ECamType::Size)][ECast(ECamMatrix::Size)][ECast(ECamNum::Size)];
	_float4x4		m_CamInvMatrices[ECast(ECamType::Size)][ECast(ECamMatrix::Size)][ECast(ECamNum::Size)];
	_float4			m_CamPositions[ECast(ECamType::Size)][ECast(ECamNum::Size)];
#pragma endregion


public:
	const D3D11_VIEWPORT Get_Viewport(EViewportNum eNum) { return m_Viewports[ECast(eNum)]; }

private:
	D3D11_VIEWPORT	m_Viewports[ECast(EViewportNum::Size)];

};

END