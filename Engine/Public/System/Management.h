#pragma once

#include "BaseClass/Scene.h"

BEGIN(Engine)

enum class EMANAGE_SCENE : _uint
{
	// ���� �� ���, �� ��ȯ�� ���� ���� ������ �������ϴ�. �Ϲ����� ���ӿ��� ���ϴ�.
	SINGLE, 
	// ���� ������ �ʰ� �����ִ� ���, ������ ���ϴ�. �޸𸮸� �� ���ϴ�.
	MULTI
};

class CManagement final : public CBase
{
	DERIVED_CLASS(CBase, CManagement)

private:
	explicit CManagement(const DX11DEVICE_T tDevice);
	virtual ~CManagement() = default;

public:
	HRESULT				Initialize(const EMANAGE_SCENE eType);
	void				PriorityTick();
	_int				Tick(const _float& fTimeDelta);
	void				LateTick();
	void				Render();

public:
	static CManagement* Create(const DX11DEVICE_T tDevice, const EMANAGE_SCENE eType);

private:
	virtual void		Free();

public:
	CPrimitiveComponent*	Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag);
	CGameObject*			Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
	void					Add_GameObject(const _tchar* pLayerTag, CGameObject* const pObj);
	void					Add_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag, CGameObject* const pObj);

	// ������ ���̴� �Լ�
	void					Add_Layer(const _tchar* pLayerTag, CLayer* const pLayer);



public:
	// �� �����ؼ� �ٷ� ����ִ� ���
	HRESULT			Set_Scene(CScene* pScene);
	// ������ ���� �������� ���
	HRESULT			Set_Scene(wstring strSceneName);
	// �� �߰�, �̸��� �Բ�
	HRESULT			Add_Scene(CScene* pScene, wstring strSceneName);
	// ������� ��ҵ鸸 ���� (���̾�, ������Ʈ)
	HRESULT			Clear_CurrentScene();
	// ������ ��ҵ鸸 ���� (���̾�, ������Ʈ)
	HRESULT			Clear_SceneAll();
	// ���� �� ����
	HRESULT			Delete_CurrentScene();
	// �� ��ü ����, ���߿� ��ҵ� �����ϴ� ������ ����
	HRESULT			Delete_SceneAll();
	
private:
	ID3D11Device*						m_pDevice = nullptr;
	ID3D11DeviceContext*				m_pDeviceContext = nullptr;

private:
	CScene*								m_pScene_Reserve;
	CScene*								m_pScene_Current;
	_unmap<wstring, CScene*>			m_mapScene;
	EMANAGE_SCENE						m_eSceneProcess;


};

END