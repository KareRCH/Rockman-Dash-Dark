#pragma once

#include "BaseClass/Level.h"

BEGIN(Engine)


enum class EGameObject : _uint
{

};

/// <summary>
/// 생성한 게임오브젝트를 모아놓는 클래스
/// 원형객체를 복제하여 생성한 사본객체를 레이어로 구분하여 모아놓는다.
/// 보관하고 있는 사본객체들의 Tick관련 함수를 반복적으로 호출해준다.
/// 월드의 영역을 관리해주며, 이 관리되는 영역을 
/// </summary>
class CObjectMgr final : public CBase
{
	DERIVED_CLASS(CBase, CObjectMgr)

private:
	explicit CObjectMgr() = default;
	virtual ~CObjectMgr() = default;

public:
	HRESULT				Initialize(_uint iNumLevels);
	void				Priority_Tick(const _float& fTimeDelta);
	_int				Tick(const _float& fTimeDelta);
	void				Late_Tick(const _float& fTimeDelta);

public:
	static CObjectMgr*	Create(_uint iNumLevels);
private:
	virtual void		Free();

#pragma region 프로토타입 관리
public:
	HRESULT Add_Prototype(const wstring& strPrototypeKey, class CGameObject* pPrototype);
	HRESULT Add_CloneObject(const wstring& strPrototypeKey, void* pArg);
	class CGameObject* Find_Prototype(const wstring& strPrototypeKey);
	void	Clear_Prototypes(const wstring& strContainTag);

private:
	_unmap<wstring, class CGameObject*>			m_mapPrototypes;			// 오브젝트의 프로토타입  
#pragma endregion



	
#pragma region 오브젝트 관리
public:
	HRESULT				Add_GameObject(class CGameObject* pObj);
	class CGameObject*	Find_GameObject(_uint iFindID);
	void				Clear_GameObject(const wstring& strLayerTag);

	// 벡터 정리 함수, 빈공간을 채워준다. 0(n)의 시간복잡도를 지닌다. 주로 업데이트가 끝날 때 사용된다.
private:
	void				Straighten_GameObjects();

private:
	_uint							m_iGiveObjectID = 0;		// 부여되는 게임 오브젝트의 ID
	vector<class CGameObject*>		m_vecGameObjects;			// 게임 오브젝트 저장공간  
#pragma endregion




#pragma region 루프 오브젝트 관리
public:
	void				RegistToTick_GameObjects();

private:
	list<class CGameObject*>		m_listTickObjects;			// 루프용 오브젝트 리스트  
#pragma endregion



#pragma region 영구 저장 오브젝트
public:


private:

#pragma endregion

};

END