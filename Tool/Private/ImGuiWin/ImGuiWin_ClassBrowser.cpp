#include "ImGuiWin/ImGuiWin_ClassBrowser.h"

#include "GameObject/StaticObject.h"
#include "ImGuiWin/ImGuiWin_PrototypeProperty.h"

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

		if (ImGui::Selectable(u8"Player", (m_iSelected_Object == 0), eFlag))
		{
			m_iSelected_Object = 0;
		}
        if (ImGui::Selectable(u8"StaticObject", (m_iSelected_Object == 1), eFlag))
        {
            m_iSelected_Object = 1;
        }

		ImGui::EndListBox();
	}

    if (ImGui::Button(u8"프로토타입 생성"))
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

        break;
    case 1:
    {
        GI()->Add_GameObject(m_pGameObject = CStaticObject::Create());
        Safe_AddRef(m_pGameObject);
        break;
    }
    }

    if (nullptr != m_pGameObject)
    {
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
