#include "ImGuiWin/ImGuiWin_ClassBrowser.h"

#include "GameObject/StaticObject.h"
#include "ImGuiWin/ImGuiWin_PrototypeProperty.h"

#include "Utility/ClassID.h"
#include "GameObject/GameObjectFactory.h"
#include "GameObject/Player.h"
#include "GameObject/ItemChest.h"
#include "GameObject/Item_Deflector.h"
#include "GameObject/ReaverBot_Balfura.h"
#include "GameObject/ReaverBot_Fingerii.h"
#include "GameObject/ReaverBot_Horokko.h"
#include "GameObject/DynamicCamera.h"
#include "GameObject/ReaverBot_HanmuruDoll.h"
#include "GameObject/Door_Common.h"
#include "GameObject/Trigger.h"

HRESULT CImGuiWin_ClassBrowser::Initialize()
{
    m_bOpen = true;

    return S_OK;
}

void CImGuiWin_ClassBrowser::Tick(const _float& fTimeDelta)
{
    if (!m_bOpen)
        return;

    ImGui::Begin(u8"클래스##ObjectTool");

    Layout_Browser(fTimeDelta);

    SUPER::Tick(fTimeDelta);

    ImGui::End();
}

HRESULT CImGuiWin_ClassBrowser::Render()
{
    return S_OK;
}

CImGuiWin_ClassBrowser* CImGuiWin_ClassBrowser::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("ImGuiWin_ObjectTool Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CImGuiWin_ClassBrowser::Free()
{
    SUPER::Free();

    Safe_Release(m_pGameObject);
}

void CImGuiWin_ClassBrowser::Layout_Browser(const _float& fTimeDelta)
{
	ImGui::Text(u8"오브젝트 리스트");

	if (ImGui::BeginListBox(u8"##오브젝트"))
	{
		ImGuiSelectableFlags eFlag = ImGuiSelectableFlags_None | ImGuiSelectableFlags_AllowDoubleClick;

        _uint iIndex = UINT_MAX;
		if (ImGui::Selectable(u8"0. Player", (m_iSelected_Object == ++iIndex), eFlag))
		{
            m_iSelected_Object = iIndex;
		}
        if (ImGui::Selectable(u8"1. StaticObject", (m_iSelected_Object == ++iIndex), eFlag))
        {
            m_iSelected_Object = iIndex;
        }
        if (ImGui::Selectable(u8"2. Horokko", (m_iSelected_Object == ++iIndex), eFlag))
        {
            m_iSelected_Object = iIndex;
        }
        if (ImGui::Selectable(u8"3. Fingerii", (m_iSelected_Object == ++iIndex), eFlag))
        {
            m_iSelected_Object = iIndex;
        }
        if (ImGui::Selectable(u8"4. Balfura", (m_iSelected_Object == ++iIndex), eFlag))
        {
            m_iSelected_Object = iIndex;
        }
        if (ImGui::Selectable(u8"5. Item_Deflector", (m_iSelected_Object == ++iIndex), eFlag))
        {
            m_iSelected_Object = iIndex;
        }
        if (ImGui::Selectable(u8"6. Chest", (m_iSelected_Object == ++iIndex), eFlag))
        {
            m_iSelected_Object = iIndex;
        }
        if (ImGui::Selectable(u8"7. DynamicCamera", (m_iSelected_Object == ++iIndex), eFlag))
        {
            m_iSelected_Object = iIndex;
        }
        if (ImGui::Selectable(u8"8. HanmuruDoll", (m_iSelected_Object == ++iIndex), eFlag))
        {
            m_iSelected_Object = iIndex;
        }
        if (ImGui::Selectable(u8"9. Door_Common", (m_iSelected_Object == ++iIndex), eFlag))
        {
            m_iSelected_Object = iIndex;
        }
        if (ImGui::Selectable(u8"10. Trigger", (m_iSelected_Object == ++iIndex), eFlag))
        {
            m_iSelected_Object = iIndex;
        }

		ImGui::EndListBox();
	}

    if (ImGui::Button(u8"프로토타입 생성")
        || ImGui::IsKeyDown(ImGuiKey_LeftShift) && ImGui::IsKeyPressed(ImGuiKey_A))
    {
        if (m_iSelected_Object != -1)
        {
            Create_Object();
        }
    }
}

void CImGuiWin_ClassBrowser::Create_Object()
{
    GI()->Set_LevelTag(TEXT("ObjectTool"));

    if (nullptr != m_pGameObject)
    {
        m_pGameObject->Set_Dead();
        Safe_Release(m_pGameObject);
    }

    switch (m_iSelected_Object)
    {
    case 0:
        GI()->Add_GameObject(m_pGameObject = CPlayer::Create());
        break;
    case 1:
        GI()->Add_GameObject(m_pGameObject = CStaticObject::Create());
        break;
    case 2:
        GI()->Add_GameObject(m_pGameObject = CReaverBot_Horokko::Create());
        break;
    case 3:
        GI()->Add_GameObject(m_pGameObject = CReaverBot_Fingerii::Create());
        break;
    case 4:
        GI()->Add_GameObject(m_pGameObject = CReaverBot_Balfura::Create());
        break;
    case 5:
        GI()->Add_GameObject(m_pGameObject = CItem_Deflector::Create());
        break;
    case 6:
        GI()->Add_GameObject(m_pGameObject = CItemChest::Create());
        break;
    case 7:
        GI()->Add_GameObject(m_pGameObject = CDynamicCamera::Create());
        break;
    case 8:
        GI()->Add_GameObject(m_pGameObject = CReaverBot_HanmuruDoll::Create());
        break;
    case 9:
        GI()->Add_GameObject(m_pGameObject = CDoor_Common::Create());
        break;
    case 10:
        GI()->Add_GameObject(m_pGameObject = CTrigger::Create());
        break;
    }

    if (nullptr != m_pGameObject)
    {
        m_pGameObject->TurnOff_State(EGObjectState::Tick);
        Safe_AddRef(m_pGameObject);
        if (m_pParentWin)
        {
            CImGuiWin_PrototypeProperty* pWin = { nullptr };
            if (SUCCEEDED(m_pParentWin->Find_Child<CImGuiWin_PrototypeProperty>(&pWin)))
            {
                pWin->Set_GameObject(m_pGameObject);
            }
        }
    }
}

void CImGuiWin_ClassBrowser::Save_Prototype(const wstring& strFileName)
{
    if (nullptr == m_pGameObject)
        return;

    FSerialData Data = m_pGameObject->SerializeData_Prototype();
    Data.Save_Data(strFileName);
}

void CImGuiWin_ClassBrowser::Set_GameObject(CGameObject* pObj)
{
    if (nullptr != m_pGameObject)
        m_pGameObject->Set_Dead();

    Safe_Release(m_pGameObject);
    m_pGameObject = pObj;
    Safe_AddRef(m_pGameObject);
}
