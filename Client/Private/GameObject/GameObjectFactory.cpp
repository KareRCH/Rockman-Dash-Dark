#include "GameObject/GameObjectFactory.h"

#include "BaseClass/GameObject.h"
#include "Utility/RapidJsonSerial.h"

#include "GameObject/Player.h"
#include "GameObject/ReaverBot_Horokko.h"


CGameObject* CGameObjectFactory::Create(FSerialData& Data)
{
	_uint iClassID = 0;
	if (FAILED(Data.Get_Data("ClassID", iClassID)))
		return nullptr;

	CGameObject* pObj = { nullptr };

	EObjectClassID eID = Cast<EObjectClassID>(iClassID);
	switch (eID)
	{
	case EObjectClassID::Player:
		pObj = TObjectClassTrait<EObjectClassID::Player>::Class::Create(Data);
		break;
	case EObjectClassID::Horokko:
		//pObj = CREATE_WITH_CLASSID(CReaverbot_Horokko, Data);
		break;
	}

	return pObj;
}
