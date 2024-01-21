#pragma once

#include "Tool_Define.h"
#include "ImGuiWin.h"


BEGIN(Engine)
class CGameObject;
class CGameObjectComp;
class CSceneComponent;
class CCommonModelComp;
class CColliderComponent;
END

BEGIN(Tool)

class CImGuiWin_PrototypeProperty : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_PrototypeProperty)

protected:
	explicit CImGuiWin_PrototypeProperty() = default;
	explicit CImGuiWin_PrototypeProperty(const CImGuiWin_PrototypeProperty& rhs) = default;
	virtual ~CImGuiWin_PrototypeProperty() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_PrototypeProperty* Create();

private:
	virtual void	Free() override;

#pragma region Ä·ºä ÄÄÆ÷³ÍÆ®
protected:
	CPipelineComp& PipelineComp() { return (*m_pPipelineComp); }

private:
	CPipelineComp* m_pPipelineComp = { nullptr };
#pragma endregion

private:
	void Layout_Property();
	void Layout_GameObjectProperty();
	void Layout_GameObjectCompProperty(CGameObjectComp* pComp);
	void Layout_SceneCompProperty(CSceneComponent* pComp);
	void Layout_CommonModelCompProperty(CCommonModelComp* pComp);
	void Layout_ColliderCompProperty(CColliderComponent* pComp);


public:
	void Set_GameObject(CGameObject* pGameObject);

private:
	_bool			m_bChanged_GameObject = { false };
	CGameObject*	m_pGameObject = { nullptr };

};

END