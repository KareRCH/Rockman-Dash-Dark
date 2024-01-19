#include "GameObject/GameObjectFactory.h"

#include "BaseClass/GameObject.h"
#include "Utility/RapidJsonSerial.h"

#include "GameObject/Player.h"
#include "GameObject/ReaverBot_Horokko.h"
#include "GameObject/ReaverBot_Balfura.h"
#include "GameObject/ReaverBot_Fingerii.h"
#include "GameObject/ReaverBot_HanmuruDoll.h"
#include "GameObject/StaticObject.h"

#include "BaseClass/Terrain.h"
#include "BaseClass/Navigation.h"


CGameObject* CGameObjectFactory::Create(FSerialData& Data)
{
	_uint iClassID = 0;
	if (FAILED(Data.Get_Data("ClassID", iClassID)))
		return nullptr;

	CGameObject* pObj = { nullptr };

	EObjectID eID = Cast<EObjectID>(iClassID);
	EObjectIDExt eExtID = Cast<EObjectIDExt>(iClassID);
	switch (eID)
	{
	case EObjectID::Terrain:
		pObj = TObjectTrait<ECast(EObjectID::Terrain)>::Class::Create(Data);
		break;
	case EObjectID::Navigation:
		//pObj = TObjectTrait<ECast(EObjectID::Navigation)>::Class::Create(Data);
		break;
	default:
		switch (eExtID)
		{
		case EObjectIDExt::Player:
			pObj = TObjectExtTrait<ECast(EObjectIDExt::Player)>::Class::Create(Data);
			break;
		case EObjectIDExt::Horokko:
			pObj = TObjectExtTrait< ECast(EObjectIDExt::Horokko)>::Class::Create(Data);
			break;
		case EObjectIDExt::Fingerii:
			pObj = TObjectExtTrait< ECast(EObjectIDExt::Fingerii)>::Class::Create(Data);
			break;
		case EObjectIDExt::Balfura:
			pObj = TObjectExtTrait< ECast(EObjectIDExt::Balfura)>::Class::Create(Data);
			break;
		case EObjectIDExt::StaticObject:
			pObj = TObjectExtTrait< ECast(EObjectIDExt::StaticObject)>::Class::Create(Data);
			break;
		case EObjectIDExt::HanmuruDoll:
			pObj = TObjectExtTrait< ECast(EObjectIDExt::HanmuruDoll)>::Class::Create(Data);
			break;
		}
		break;
	}
	

	return pObj;
}
