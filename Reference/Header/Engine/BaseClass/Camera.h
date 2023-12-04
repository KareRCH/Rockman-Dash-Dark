#pragma once

#include "GameObject.h"
#include "System/Define/PipelineMgr_Define.h"

BEGIN(Engine)

/// <summary>
/// ī�޶� ���̽� Ŭ����
/// </summary>
class ENGINE_DLL CCamera abstract : public CGameObject
{
	DERIVED_CLASS(CGameObject, CCamera)

public:
	struct FCameraDesc
	{
		_float3		vEye, vAt, vUp;
		_float		fFov, fAspect, fNear, fFar;
	};

protected:
	explicit CCamera();
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize();
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual void	Render();

protected:
	virtual void	Free();

public:
	void Apply_ViewProjMatrix();

protected:
	ECamType	m_eCamType;			// ��� ī�޶� ���� (����, ����)
	ECamNum		m_eCamID;			// ��� ī�޶� �ѹ� (0~7, ����Ʈ�� ����ȭ��)

	_float3		m_vAt;				// �ٶ󺸴� ����
	_float		m_fFov;				// �þ߰�
	_float		m_fAspect;			// ����
	_float		m_fNear;			// ī�޶� �ּҰŸ�
	_float		m_fFar;				// ī�޶� �ִ�Ÿ�

	_float4x4	m_matView;			// ����� �� ���
	_float4x4	m_matProj;			// ����� ���� ���
};

END