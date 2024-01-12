#include "GameObject/GameObjectFactory.h"

#include "BaseClass/GameObject.h"
#include "Utility/RapidJsonSerial.h"

#include "GameObject/Player.h"
#include "GameObject/ReaverBot_Horokko.h"
#include "GameObject/ReaverBot_Balfura.h"
#include "GameObject/ReaverBot_Fingerii.h"


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
		pObj = TObjectClassTrait<EObjectClassID::Horokko>::Class::Create(Data);
		break;
	case EObjectClassID::Fingerii:
		pObj = TObjectClassTrait<EObjectClassID::Fingerii>::Class::Create(Data);
		break;
	case EObjectClassID::Balfura:
		//pObj = TObjectClassTrait<EObjectClassID::Balfura>::Class::Create(Data);
		break;
	}

	return pObj;
}
