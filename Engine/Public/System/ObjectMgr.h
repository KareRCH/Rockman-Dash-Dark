#pragma once

#include "BaseClass/Level.h"

BEGIN(Engine)

/// <summary>
/// 생성한 게임오브젝트를 모아놓는 클래스
/// 원형객체를 복제하여 생성한 사본객체를 레이어로 구분하여 모아놓는다.
/// 보관하고 있는 사본객체들의 Tick관련 함수를 반복적으로 호출해준다.
/// </summary>
class CObjectMgr final : public CBase
{
	DERIVED_CLASS(CBase, CObjectMgr)

private:
	explicit CObjectMgr();
	virtual ~CObjectMgr() = default;

public:
	HRESULT				Initialize(_uint iNumLevels);
	void				Priority_Tick(const _float& fTimeDelta);
	_int				Tick(const _float& fTimeDelta);
	void				Late_Tick(const _float& fTimeDelta);

public:
	static CObjectMgr* Create(_uint iNumLevels);
private:
	virtual void		Free();

public:
	HRESULT Add_Prototype(const wstring& strPrototypeKey, class CGameObject* pPrototype);
	HRESULT Add_CloneObject(_uint iLevelIndex, const wstring& strLayerKey, const wstring& strPrototypeKey, void* pArg);
	class CGameObject* Find_Prototype(const wstring& strPrototypeKey);
	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& strLayerKey);

private:
	_uint				m_iNumLevels = { 0 };

private:
	_unmap<wstring, class CGameObject*>			m_mapPrototypes;
	_unmap<wstring, class CLayer*>*				m_pLayers = { nullptr };
	using map_layer = _unmap<wstring, class CLayer*>;

//public:
//	CPrimitiveComponent*	Get_Component(COMPONENTID eID, const wstring& pLayerTag, const wstring& pObjTag, const wstring& pComponentTag);
//	CGameObject*			Get_GameObject(const wstring& strLayerTag, const wstring& pObjTag);
//	void					Add_GameObject(const wstring& strLayerTag, CGameObject* const pObj);
//	void					Add_GameObject(const wstring& strLayerTag, const wstring& pObjTag, CGameObject* const pObj);
//
//	// 툴에서 쓰이는 함수
//	void					Add_Layer(const wstring& pLayerTag, CLayer* const pLayer);


};

END