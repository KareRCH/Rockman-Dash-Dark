#pragma once

#include "BaseClass/Scene.h"

BEGIN(Engine)

enum class EMANAGE_SCENE : _uint
{
	// 단일 씬 모드, 씬 전환시 이전 씬의 정보가 지워집니다. 일반적인 게임에서 씁니다.
	SINGLE, 
	// 씬이 꺼지지 않고 남아있는 모드, 툴에서 씁니다. 메모리를 더 씁니다.
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

	// 툴에서 쓰이는 함수
	void					Add_Layer(const _tchar* pLayerTag, CLayer* const pLayer);



public:
	// 씬 생성해서 바로 집어넣는 방식
	HRESULT			Set_Scene(CScene* pScene);
	// 생성된 씬을 돌려쓰는 방식
	HRESULT			Set_Scene(wstring strSceneName);
	// 씬 추가, 이름과 함께
	HRESULT			Add_Scene(CScene* pScene, wstring strSceneName);
	// 현재씬에 요소들만 삭제 (레이어, 오브젝트)
	HRESULT			Clear_CurrentScene();
	// 모든씬에 요소들만 삭제 (레이어, 오브젝트)
	HRESULT			Clear_SceneAll();
	// 현재 씬 삭제
	HRESULT			Delete_CurrentScene();
	// 씬 전체 삭제, 나중에 요소들 해제하는 것으로 변경
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