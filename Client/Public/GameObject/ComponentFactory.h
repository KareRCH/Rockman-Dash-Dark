#pragma once

#include "Client_Define.h"
#include "Utility/ClassID.h"
#include "Utility/RapidJsonSerial.h"

BEGIN(Client)

class CComponentFactory abstract
{
public:
	static CComponent* Create(FSerialData& Data);
};

END