#pragma once

#include "BaseClass/Level.h"

BEGIN(Engine)

/// <summary>
/// ������ ���ӿ�����Ʈ�� ��Ƴ��� Ŭ����
/// ������ü�� �����Ͽ� ������ �纻��ü�� ���̾�� �����Ͽ� ��Ƴ��´�.
/// �����ϰ� �ִ� �纻��ü���� Tick���� �Լ��� �ݺ������� ȣ�����ش�.
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
//	// ������ ���̴� �Լ�
//	void					Add_Layer(const wstring& pLayerTag, CLayer* const pLayer);


};

END