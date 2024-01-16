#include "GameObject/ComponentFactory.h"

#include "Component/PivotComponent.h"
#include "Component/ColliderComponent.h"
#include "Component/CommonModelComp.h"
#include "Component/BoxModelComp.h"
#include "Component/TerrainModelComp.h"
#include "Component/PlaneModelComp.h"
#include "Component/CylinderModelComp.h"
#include "Component/NavigationComponent.h"
#include "Component/CloudStationComp.h"
#include "Component/TeamAgentComp.h"

CComponent* CComponentFactory::Create(FSerialData& Data)
{
	_uint iCompID = 0;
	if (FAILED(Data.Get_Data("ComponentID", iCompID)))
		return nullptr;

	CComponent* pComp = { nullptr };

	EComponentID eID = Cast<EComponentID>(iCompID);
	switch (eID)
	{
	case EComponentID::Pivot:
		pComp = TComponentTrait<ECast(EComponentID::Pivot)>::Class::Create(Data);
		break;
	case EComponentID::Collider:
		pComp = TComponentTrait<ECast(EComponentID::Collider)>::Class::Create(Data);
		break;
	case EComponentID::CommonModel:
		pComp = TComponentTrait<ECast(EComponentID::CommonModel)>::Class::Create(Data);
		break;
	case EComponentID::BoxModel:
		pComp = TComponentTrait<ECast(EComponentID::BoxModel)>::Class::Create(Data);
		break;
	case EComponentID::TerrainModel:
		pComp = TComponentTrait<ECast(EComponentID::TerrainModel)>::Class::Create(Data);
		break;
	case EComponentID::PlaneModel:
		pComp = TComponentTrait<ECast(EComponentID::PlaneModel)>::Class::Create(Data);
		break;
	case EComponentID::CylinderModel:
		pComp = TComponentTrait<ECast(EComponentID::CylinderModel)>::Class::Create(Data);
		break;
	case EComponentID::Navigation:
		pComp = TComponentTrait<ECast(EComponentID::Navigation)>::Class::Create(Data);
		break;
	case EComponentID::CloudStation:
		pComp = TComponentTrait<ECast(EComponentID::CloudStation)>::Class::Create(Data);
		break;
	case EComponentID::TeamAgent:
		pComp = TComponentTrait<ECast(EComponentID::TeamAgent)>::Class::Create(Data);
		break;
	}

	return pComp;
}
