#pragma once

#include "Client_Define.h"
#include "Engine_Define.h"

BEGIN(Enging)
class FSerialData;
END

BEGIN(Client)

class CGameObjectFactory abstract
{
public:
	static class CGameObject* Create(FSerialData& Data);
};

END