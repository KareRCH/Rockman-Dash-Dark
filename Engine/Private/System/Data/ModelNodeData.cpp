#include "System/Data/ModelNodeData.h"

void FModelNodeBaseData::Free()
{
	for (auto& item : vecChildren)
	{
		Safe_Release(item);
	}
	vecChildren.clear();
}


// ------------------------ FModelNodeData ----------------------------


FModelNodeData* FModelNodeData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FModelNodeData Create Failed");

		return nullptr;
	}

	return pInstance;
}

void FModelNodeData::Free()
{
	SUPER::Free();


}


// ------------------------ ModelNodeGroup ----------------------------


FModelNodeGroup* FModelNodeGroup::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FModelNodeData Create Failed");

		return nullptr;
	}

	return pInstance;
}

void FModelNodeGroup::Free()
{
	for (auto& Pair : mapModelNodeData)
	{
		Safe_Release(Pair.second);
	}
	mapModelNodeData.clear();
}