#include "ImGuiWin/ImGuiWin_Property.h"

#include "GameObject/StaticObject.h"
#include "GameObject/ReaverBot_Horokko.h"
#include "GameObject/ReaverBot_Balfura.h"
#include "GameObject/ReaverBot_Fingerii.h"
#include "GameObject/Player.h"

#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"
#include "ImGuiFileDialog.h"

HRESULT CImGuiWin_Property::Initialize()
{
	m_bOpen = true;

	return S_OK;
}

void CImGuiWin_Property::Tick(const _float& fTimeDelta)
{
	if (!m_bOpen)
		return;

	ImGui::Begin(u8"속성");

    Layout_Property();

	SUPER::Tick(fTimeDelta);

	ImGui::End();
}

HRESULT CImGuiWin_Property::Render()
{
	return S_OK;
}

CImGuiWin_Property* CImGuiWin_Property::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CImGuiWin_Viewer Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGuiWin_Property::Free()
{
	SUPER::Free();

    Safe_Release(m_pGameObject);
}

void CImGuiWin_Property::Layout_Property()
{
	if (nullptr == m_pGameObject)
		return;

	Layout_GameObjectProperty();

	_uint iNumComponents = m_pGameObject->Get_NumComponents();
	for (_uint i = 0; i < iNumComponents; i++)
	{
		// 상위 컴포넌트 설정
		auto pGameObjectComp = m_pGameObject->Get_Component<CGameObjectComp>(i);
		if (nullptr != pGameObjectComp)
		{
			ImGui::Separator();
            ImGui::Separator();
            ImGui::Separator();
			Layout_GameObjectCompProperty(pGameObjectComp);
		}
	}
}

void CImGuiWin_Property::Layout_GameObjectProperty()
{
    _float3 vPos = {}, vRot = {}, vScale = {};

    vPos = m_pGameObject->Transform().Get_PositionFloat3();
    vRot = m_pGameObject->Transform().Get_RotationFixedFloat3();
    vRot.x = XMConvertToDegrees(vRot.x);
    vRot.y = XMConvertToDegrees(vRot.y);
    vRot.z = XMConvertToDegrees(vRot.z);
    vScale = m_pGameObject->Transform().Get_ScaleFloat3();


    string strName = ConvertToString(m_pGameObject->Get_Name());
    _char szName[MAX_PATH];
    strcpy_s(szName, ConvertToString(m_pGameObject->Get_Name()).c_str());
    ImGui::Text(szName);

    ImGui::Button(u8"이름");
    ImGui::SameLine();
    strcpy_s(szName, strName.c_str());
    ImGui::Text(szName);


    ImGui::Button(u8"프로토 이름");
    ImGui::SameLine();
    _char szProtoName[MAX_PATH];
    strcpy_s(szProtoName, ConvertToString(m_pGameObject->Get_ProtoName()).c_str());
    string strProp = "";
    strProp = "##ProtoName" + strName;
    strcpy_s(szName, strProp.c_str());
    ImGui::SetNextItemWidth(150.f);
    if (ImGui::InputText(szName, szProtoName, sizeof szProtoName, ImGuiInputTextFlags_EnterReturnsTrue)
        || ImGui::IsItemDeactivatedAfterEdit())
    {
        m_pGameObject->Set_ProtoName(ConvertToWstring(szProtoName));
    }



    ImGui::Text(u8"위치");
    ImGui::Button("X##ProtoPosX");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat("##ProtoPosX", &vPos.x, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)
        || ImGui::IsItemDeactivated())
    {
        m_pGameObject->Transform().Set_PositionX(vPos.x);
    }
    ImGui::SameLine();

    ImGui::Button("Y##ProtoPosY");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat("##ProtoPosY", &vPos.y, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)
        || ImGui::IsItemDeactivated())
    {
        m_pGameObject->Transform().Set_PositionY(vPos.y);
    }
    ImGui::SameLine();

    ImGui::Button("Z##ProtoPosZ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat("##ProtoPosZ", &vPos.z, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)
        || ImGui::IsItemDeactivated())
    {
        m_pGameObject->Transform().Set_PositionZ(vPos.z);
    }






    ImGui::Text(u8"회전");
    ImGui::Button("X##ProtoRotX");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat("##ProtoRotX", &vRot.x, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)
        || ImGui::IsItemDeactivated())
    {
        m_pGameObject->Transform().Set_RotationFixedX(XMConvertToRadians(vRot.x));
    }
    ImGui::SameLine();

    ImGui::Button("Y##ProtoRotY");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat("##ProtoRotY", &vRot.y, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)
        || ImGui::IsItemDeactivated())
    {
        m_pGameObject->Transform().Set_RotationFixedY(XMConvertToRadians(vRot.y));
    }
    ImGui::SameLine();

    ImGui::Button("Z##ProtoRotZ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat("##ProtoRotZ", &vRot.z, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)
        || ImGui::IsItemDeactivated())
    {
        m_pGameObject->Transform().Set_RotationFixedZ(XMConvertToRadians(vRot.z));
    }





    ImGui::Text(u8"크기");
    ImGui::Button("X##ProtoScaleX");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat("##ProtoScaleX", &vScale.x, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)
        || ImGui::IsItemDeactivated())
    {
        m_pGameObject->Transform().Set_ScaleX(vScale.x);
    }
    ImGui::SameLine();

    ImGui::Button("Y##ProtoScaleY");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat("##ProtoScaleY", &vScale.y, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)
        || ImGui::IsItemDeactivated())
    {
        m_pGameObject->Transform().Set_ScaleY(vScale.y);
    }
    ImGui::SameLine();

    ImGui::Button("Z##ProtoScaleZ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat("##ProtoScaleZ", &vScale.z, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue)
        || !ImGui::IsItemDeactivated())
    {
        m_pGameObject->Transform().Set_ScaleZ(vScale.z);
    }


    _float fPriority = m_pGameObject->Get_Priority(ECast(EGObjTickPriority::Tick));
    ImGui::Button("TickPriority##ProtoPriorityTick");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat("##ProtoPriorityTick",
        &fPriority, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
    {
        m_pGameObject->Set_Priority(ECast(EGObjTickPriority::Tick), fPriority);
    }

    fPriority = m_pGameObject->Get_Priority(ECast(EGObjTickPriority::Render));
    ImGui::Button("RenderPriority##ProtoPriorityRender");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat("##ProtoPriorityRender",
        &fPriority, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
    {
        m_pGameObject->Set_Priority(ECast(EGObjTickPriority::Render), fPriority);
    }
}

void CImGuiWin_Property::Layout_GameObjectCompProperty(CGameObjectComp* pComp)
{
    string strName = ConvertToString(pComp->Get_Name());
    string strProp = "";
    _char szName[MAX_PATH];

    ImGui::Button(u8"이름");
    ImGui::SameLine();
    strcpy_s(szName, strName.c_str());
    ImGui::Text(szName);


    ImGui::Button(u8"프로토 이름");
    ImGui::SameLine();
    _char szProtoName[MAX_PATH];
    strcpy_s(szProtoName, ConvertToString(pComp->Get_ProtoName()).c_str());
    strProp = "##ProtoName" + strName;
    strcpy_s(szName, strProp.c_str());
    ImGui::SetNextItemWidth(150.f);
    if (ImGui::InputText(szName, szProtoName, sizeof szProtoName, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        pComp->Set_ProtoName(ConvertToWstring(szProtoName));
    }

    auto pSceneComp = DynCast<CSceneComponent*>(pComp);
    if (nullptr != pSceneComp)
    {
        ImGui::Separator();
        Layout_SceneCompProperty(pSceneComp);
    }
}

void CImGuiWin_Property::Layout_SceneCompProperty(CSceneComponent* pComp)
{
    _float3 vPos = {}, vRot = {}, vScale = {};

    vPos = pComp->Transform().Get_PositionFloat3();
    vRot = pComp->Transform().Get_RotationFixedFloat3();
    vRot.x = XMConvertToDegrees(vRot.x);
    vRot.y = XMConvertToDegrees(vRot.y);
    vRot.z = XMConvertToDegrees(vRot.z);
    vScale = pComp->Transform().Get_ScaleFloat3();
    string strName = ConvertToString(pComp->Get_Name());
    string strProp = "";
    _char szName[MAX_PATH];

    ImGui::Text(u8"위치");
    ImGui::Button("X##ProtoPosX");
    ImGui::SameLine();
    strProp = "##ProtoPosX" + strName;
    strcpy_s(szName, strProp.c_str());
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat(szName, &vPos.x, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
    {
        pComp->Transform().Set_PositionX(vPos.x);
    }
    if (ImGui::IsItemActive())
        ImGui::SetKeyboardFocusHere(-1);
    ImGui::SameLine();

    ImGui::Button("Y##ProtoPosY");
    ImGui::SameLine();
    strProp = "##ProtoPosY" + strName;
    strcpy_s(szName, strProp.c_str());
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat(szName, &vPos.y, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
    {
        pComp->Transform().Set_PositionY(vPos.y);
    }
    if (ImGui::IsItemActive())
        ImGui::SetKeyboardFocusHere(-1);
    ImGui::SameLine();

    ImGui::Button("Z##ProtoPosZ");
    ImGui::SameLine();
    strProp = "##ProtoPosZ" + strName;
    strcpy_s(szName, strProp.c_str());
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat(szName, &vPos.z, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
    {
        pComp->Transform().Set_PositionZ(vPos.z);
    }
    if (ImGui::IsItemActive())
        ImGui::SetKeyboardFocusHere(-1);






    ImGui::Text(u8"회전");
    ImGui::Button("X##ProtoRotX");
    ImGui::SameLine();
    strProp = "##ProtoRotX" + strName;
    strcpy_s(szName, strProp.c_str());
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat(szName, &vRot.x, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
    {
        pComp->Transform().Set_RotationFixedX(XMConvertToRadians(vRot.x));
    }
    if (ImGui::IsItemActive())
        ImGui::SetKeyboardFocusHere(-1);
    ImGui::SameLine();

    ImGui::Button("Y##ProtoRotY");
    ImGui::SameLine();
    strProp = "##ProtoRotY" + strName;
    strcpy_s(szName, strProp.c_str());
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat(szName, &vRot.y, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
    {
        pComp->Transform().Set_RotationFixedY(XMConvertToRadians(vRot.y));
    }
    if (ImGui::IsItemActive())
        ImGui::SetKeyboardFocusHere(-1);
    ImGui::SameLine();

    ImGui::Button("Z##ProtoRotZ");
    ImGui::SameLine();
    strProp = "##ProtoRotZ" + strName;
    strcpy_s(szName, strProp.c_str());
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat(szName, &vRot.z, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
    {
        pComp->Transform().Set_RotationFixedZ(XMConvertToRadians(vRot.z));
    }
    if (ImGui::IsItemActive())
        ImGui::SetKeyboardFocusHere(-1);





    ImGui::Text(u8"크기");
    ImGui::Button("X##ProtoScaleX");
    ImGui::SameLine();
    strProp = "##ProtoScaleX" + strName;
    strcpy_s(szName, strProp.c_str());
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat(szName, &vScale.x, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
    {
        pComp->Transform().Set_ScaleX(vScale.x);
    }
    if (ImGui::IsItemActive())
        ImGui::SetKeyboardFocusHere(-1);
    ImGui::SameLine();

    ImGui::Button("Y##ProtoScaleY");
    ImGui::SameLine();
    strProp = "##ProtoScaleY" + strName;
    strcpy_s(szName, strProp.c_str());
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat(szName, &vScale.y, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
    {
        pComp->Transform().Set_ScaleY(vScale.y);
    }
    if (ImGui::IsItemActive())
        ImGui::SetKeyboardFocusHere(-1);
    ImGui::SameLine();

    ImGui::Button("Z##ProtoScaleZ");
    ImGui::SameLine();
    strProp = "##ProtoScaleZ" + strName;
    strcpy_s(szName, strProp.c_str());
    ImGui::SetNextItemWidth(50.f);
    if (ImGui::InputFloat(szName, &vScale.z, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
    {
        pComp->Transform().Set_ScaleZ(vScale.z);
    }
    if (ImGui::IsItemActive())
        ImGui::SetKeyboardFocusHere(-1);


    auto pCommonModelComp = DynCast<CCommonModelComp*>(pComp);
    if (nullptr != pCommonModelComp)
    {
        ImGui::Separator();
        Layout_CommonModelCompProperty(pCommonModelComp);
    }

    auto pColliderComp = DynCast<CColliderComponent*>(pComp);
    if (nullptr != pColliderComp)
    {
        ImGui::Separator();
        Layout_ColliderCompProperty(pColliderComp);
    }
}

void CImGuiWin_Property::Layout_CommonModelCompProperty(CCommonModelComp* pComp)
{
}

void CImGuiWin_Property::Layout_ColliderCompProperty(CColliderComponent* pComp)
{
}

void CImGuiWin_Property::Set_GameObject(CGameObject* pGameObject)
{
    if (nullptr == pGameObject)
        return;

    Safe_Release(m_pGameObject);
    m_pGameObject = pGameObject;
    Safe_AddRef(m_pGameObject);

    m_bChanged_GameObject = true;
}
