#pragma once

#include "Client_Define.h"
#include "Engine_Define.h"

BEGIN(Engine)
class FSerialData;
END

BEGIN(Client)

enum class EObjectClassID : _uint
{
	Player,
	Horokko,
};

template <EObjectClassID T>
struct TObjectClassTrait;

class CGameObjectFactory abstract
{
public:
	static class CGameObject* Create(FSerialData& Data);
};

END