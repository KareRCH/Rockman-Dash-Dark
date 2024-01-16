#pragma once

#include "Client_Define.h"
#include "Engine_Define.h"
#include "Utility/ClassID.h"

BEGIN(Engine)
class FSerialData;
END

BEGIN(Client)

enum class EObjectIDExt : _uint
{
	Player = ECast(EObjectID::End),
	Horokko,
	Fingerii,
	Balfura,
	StaticObject
};

template <_uint T>
struct TObjectExtTrait;

enum class EComponentIDExt : _uint
{
	Anithing = ECast(EComponentID::End),
};

template <_uint T>
struct TComponentExtTrait;

class CGameObjectFactory abstract
{
public:
	static class CGameObject* Create(FSerialData& Data);
};

END