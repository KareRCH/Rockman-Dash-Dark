#include "System/Data/ModelNodeData.h"

void FModelNodeBaseData::Free()
{
	for (auto& item : vecChildren)
	{
		Safe_Release(item);
	}
	vecChildren.clear();
}

// ---------------------------- FModelRootNodeData --------------------------

FModelRootNodeData* FModelRootNodeData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FModelRootNodeData Create Failed");

		return nullptr;
	}

	return pInstance;
}

void FModelRootNodeData::Free()
{
	SUPER::Free();


}


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