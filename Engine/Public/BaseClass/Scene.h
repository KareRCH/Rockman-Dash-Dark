#pragma once

#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene abstract : public CBase
{
	DERIVED_CLASS(CBase, CScene)
protected:
	explicit CScene(const DX11DEVICE_T tDevice);
	explicit CScene(const CScene& rhs) = delete;
	virtual ~CScene() = default;

public:
	virtual HRESULT		Initialize();
	virtual void		PriorityTick();
	virtual _int		Tick(const _float& fTimeDelta);
	virtual void		LateTick();
	virtual void		Render();

	virtual HRESULT		InitializeLate_Scene();

protected:
	virtual void		Free();

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

public:
	CPrimitiveComponent*	Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag);
	CGameObject*			Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
	void					Add_GameObject(const _tchar* pLayerTag, CGameObject* const pObj);
	void					Add_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag, CGameObject* pObj);

	CLayer*					Get_Layer(const _tchar* pLayerTag) { return m_mapLayer[pLayerTag]; }
	HRESULT					Add_Layer(const _tchar* pLayerTag, CLayer* pLayer);

	HRESULT					Delete_LayerAll();

protected:	// 레이어 관련
	// 모든 레이어가 준비되었을 때 작성하는 코드입니다.
	virtual HRESULT			Initialize_Layer_Completed() PURE;
	

protected:
	_unmap<wstring, CLayer*>		m_mapLayer;
	vector<CLayer*>					m_vecPriorityLayer;

};

END