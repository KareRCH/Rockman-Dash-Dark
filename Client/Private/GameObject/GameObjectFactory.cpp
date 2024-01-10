#include "GameObject/GameObjectFactory.h"

#include "BaseClass/GameObject.h"
#include "Utility/RapidJsonSerial.h"

#include "GameObject/Player.h"


CGameObject* CGameObjectFactory::Create(FSerialData& Data)
{
	_uint iClassID = 0;
	if (FAILED(Data.Get_Data("ClassID", iClassID)))
		return nullptr;

	CGameObject* pObj = { nullptr };

	if (iClassID == GET_CLASSID(CPlayer))
	{
		pObj = CREATE_WITH_CLASSID(CPlayer, Data);
	}

	return pObj;
}
