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
	virtual void		Priority_Tick(const _float& fTimeDelta);
	virtual _int		Tick(const _float& fTimeDelta);
	virtual void		Late_Tick(const _float& fTimeDelta);
	virtual void		Render();

	virtual HRESULT		InitializeLate_Scene();

protected:
	virtual void		Free();

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

public:
	CPrimitiveComponent*	Get_Component(COMPONENTID eID, const wstring& pLayerTag, const wstring& pObjTag, const wstring& pComponentTag);
	CGameObject*			Get_GameObject(const wstring&, const wstring& pObjTag);
	void					Add_GameObject(const wstring&, CGameObject* const pObj);
	void					Add_GameObject(const wstring&, const wstring& pObjTag, CGameObject* pObj);

	CLayer*					Get_Layer(const wstring& pLayerTag) { return m_mapLayer[pLayerTag]; }
	HRESULT					Add_Layer(const wstring& pLayerTag, CLayer* pLayer);

	HRESULT					Delete_LayerAll();

protected:	// 레이어 관련
	// 모든 레이어가 준비되었을 때 작성하는 코드입니다.
	virtual HRESULT			Initialize_Layer_Completed() PURE;
	

protected:
	_unmap<wstring, CLayer*>		m_mapLayer;
	vector<CLayer*>					m_vecPriorityLayer;

};

END