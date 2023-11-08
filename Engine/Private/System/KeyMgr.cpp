#include "System/KeyMgr.h"

#include "System/GameInstance.h"

CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

HRESULT CKeyMgr::Initialize()
{
	//Add_InputKey(new INPUT_KEY_INFO(L"Right"));
	////m_vInputKeys.back()->Add_Keys(VK_RIGHT, 'D');
	//Add_InputKey(new INPUT_KEY_INFO(L"Left"));
	////m_vInputKeys.back()->Add_Keys(VK_LEFT, 'A');
	//Add_InputKey(new INPUT_KEY_INFO(L"Up"));
	////m_vInputKeys.back()->Add_Key(VK_UP);
	//Add_InputKey(new INPUT_KEY_INFO(L"Down"));
	////m_vInputKeys.back()->Add_Key(VK_DOWN);

	//Add_InputKey(new INPUT_KEY_INFO(L"UI_Accept"));
	////m_vInputKeys.back()->Add_Key(VK_ACCEPT);
	//Add_InputKey(new INPUT_KEY_INFO(L"UI_Cancel"));
	////m_vInputKeys.back()->Add_Key(VK_ESCAPE);
	//Add_InputKey(new INPUT_KEY_INFO(L"UI_Menu"));
	////m_vInputKeys.back()->Add_Key(VK_ACCEPT);

	return S_OK;
}

void CKeyMgr::Tick()
{
	//Engine::Update_InputDev();		// ���̷�Ʈ ��ǲ ������Ʈ�� Ű �Ŵ������� ���־� �ϰ����� �����Ѵ�.
}

void CKeyMgr::Late_Tick()
{
	//for (int i = 0; i < m_vInputKeys.size(); ++i)
	//{
	//	if (nullptr == m_vInputKeys[i])
	//		continue;

	//	for (int j = 0; j < m_vInputKeys[i]->vKeys.size(); ++j)
	//	{
	//		// �Է��� �޾��� ��
	//		if (GetAsyncKeyState(get<0>(m_vInputKeys[i]->vKeys[j])) & 0x8000)
	//		{
	//			if (EINPUT_KEY_STATE::NUETRAL >= get<1>(m_vInputKeys[i]->vKeys[j]))
	//				get<1>(m_vInputKeys[i]->vKeys[j]) = EINPUT_KEY_STATE::PRESSED;
	//			else
	//				get<1>(m_vInputKeys[i]->vKeys[j]) = EINPUT_KEY_STATE::PRESSING;
	//		}
	//		// �Է��� ���� ��
	//		else
	//		{
	//			if (EINPUT_KEY_STATE::PRESSING <= get<1>(m_vInputKeys[i]->vKeys[j]))
	//				get<1>(m_vInputKeys[i]->vKeys[j]) = EINPUT_KEY_STATE::RELEASED;
	//			else
	//				get<1>(m_vInputKeys[i]->vKeys[j]) = EINPUT_KEY_STATE::NUETRAL;
	//		}
	//	}
	//}

	// m_bKeyState�� ���� Ű ���¸� ������Ʈ �մϴ�.
	for (int i = 0; i < MAX_DINPUT_KEY; ++i)
	{
		if (m_bKeyState[i] && !(GameInstance()->Get_DIKeyState(i)))
			m_bKeyState[i] = !m_bKeyState[i];
		else if (!m_bKeyState[i] && (GameInstance()->Get_DIKeyState(i)))
			m_bKeyState[i] = !m_bKeyState[i];
	}

	for (int i = 0; i < MAX_DINPUT_MOUSE; ++i)
	{
		if (i <= 2)
		{
			if (m_bMouseState[i] && !(GameInstance()->Get_DIMouseState((Engine::MOUSEKEYSTATE)i)))
				m_bMouseState[i] = !m_bMouseState[i];
			else if (!m_bMouseState[i] && (GameInstance()->Get_DIMouseState((Engine::MOUSEKEYSTATE)i)))
				m_bMouseState[i] = !m_bMouseState[i];
		}
		else
		{
			switch((Engine::MOUSEKEYSTATE)i)
			{
			case Engine::DIM_MWU:
			{
				if (m_bMouseState[i] && !(GameInstance()->Get_DIMouseMove(DIMS_Z) > 0L))
					m_bMouseState[i] = !m_bMouseState[i];
				else if (!m_bMouseState[i] && (GameInstance()->Get_DIMouseMove(DIMS_Z) > 0L))
					m_bMouseState[i] = !m_bMouseState[i];
				break;
			}
			case Engine::DIM_MWD:
			{
				if (m_bMouseState[i] && !(GameInstance()->Get_DIMouseMove(DIMS_Z) < 0L))
					m_bMouseState[i] = !m_bMouseState[i];
				else if (!m_bMouseState[i] && (GameInstance()->Get_DIMouseMove(DIMS_Z) < 0L))
					m_bMouseState[i] = !m_bMouseState[i];
				break;
			}
			}
		}
	}
}

CKeyMgr* CKeyMgr::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("KeyMgr Create Failed");
		Engine::Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CKeyMgr::Free()
{
	if (m_mapInputKeys.empty())
		return;

	/*for_each(m_mapInputKeys.begin(), m_mapInputKeys.end(), [](pair<const _tchar*, vector<INPUT_KEY_INFO*>>& vecInputKeyInfo) {
		for_each(vecInputKeyInfo.second.begin(), vecInputKeyInfo.second.end(), ()[INPUT_KEY_INFO*& pInputKey] {
			Safe_Delete(pInputKey);
			})
		});*/
}

void CKeyMgr::Add_InputKey(INPUT_KEY_INFO* pInputKey)
{
}

void CKeyMgr::Remove_InputKey(INPUT_KEY_INFO* pInputKey)
{
}
