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

class CObjectMgr final : public CBase
{
	DERIVED_CLASS(CBase, CObjectMgr)

private:
	explicit CObjectMgr(const DX11DEVICE_T tDevice);
	virtual ~CObjectMgr() = default;

public:
	HRESULT				Initialize(const EMANAGE_SCENE eType);
	void				Priority_Tick(const _float& fTimeDelta);
	_int				Tick(const _float& fTimeDelta);
	void				Late_Tick(const _float& fTimeDelta);
	void				Render();

public:
	static CObjectMgr* Create(const DX11DEVICE_T tDevice, const EMANAGE_SCENE eType);

private:
	virtual void		Free();

public:
	CPrimitiveComponent*	Get_Component(COMPONENTID eID, const wstring& pLayerTag, const wstring& pObjTag, const wstring& pComponentTag);
	CGameObject*			Get_GameObject(const wstring& strLayerTag, const wstring& pObjTag);
	void					Add_GameObject(const wstring& strLayerTag, CGameObject* const pObj);
	void					Add_GameObject(const wstring& strLayerTag, const wstring& pObjTag, CGameObject* const pObj);

	// ������ ���̴� �Լ�
	void					Add_Layer(const wstring& pLayerTag, CLayer* const pLayer);

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
	ID3D11Device*						m_pDevice = { nullptr };
	ID3D11DeviceContext*				m_pDeviceContext = { nullptr };

private:
	CScene*								m_pScene_Reserve;
	CScene*								m_pScene_Current;
	_unmap<wstring, CScene*>			m_mapScene;
	EMANAGE_SCENE						m_eSceneProcess;


};

END