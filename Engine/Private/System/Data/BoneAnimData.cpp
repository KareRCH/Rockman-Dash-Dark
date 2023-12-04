#include "System/Data/BoneAnimData.h"

FBoneAnimNodeData* FBoneAnimNodeData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		MSG_BOX("FAnimNodeData Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void FBoneAnimNodeData::Free()
{
	vecPositions.clear();
	vecRotations.clear();
	vecScales.clear();
}

_float4x4 FBoneAnimNodeData::Interporated_Matrix(const _float& fCurTime) const
{
	_uint iPivot;
	_matrix matTransform = XMMatrixIdentity();
	_vector vCalcPos, vCalcRot, vCalcScale;
	if (!vecPositions.empty())
	{
		iPivot = Calculate_PivotPosition(fCurTime);

		// 지점을 구하면, 기준점의 시간과 현재 시간을 기준으로 계산하여 값을 보간한다.
		_float fStartTime = vecPositions[iPivot].fTime;	// 시간 변화율
		_float fEndTime = fStartTime;								// 기준점과 다음점의 시간차이
		_float fRatioTime = fStartTime;

		_vector vStartPos = XMLoadFloat3(&vecPositions[iPivot].vPos);	// 기준으로부터 변화율
		_vector vEndPos = vStartPos;
		_vector vFinalPos = vStartPos;

		// 다음 요소가 있을 때
		if (iPivot + 1 < vecRotations.size())
		{
			// 다음 요소에 대한 값을 저장
			fEndTime = vecPositions[iPivot + 1].fTime;
			vEndPos = XMLoadFloat3(&vecPositions[iPivot + 1].vPos);

			fRatioTime = (fCurTime - fStartTime) / (fEndTime - fStartTime);	// 기준점과 다음점의 시간차이와의 비율을 구함
			vFinalPos += (vEndPos - vStartPos) * fRatioTime;				// 기준점과 다음점의 위치 값의 차를 구해 시간 변화율을 곱함
		}

		vCalcPos = vFinalPos;
	}

	if (!vecRotations.empty())
	{
		iPivot = Calculate_PivotRotation(fCurTime);

		// 지점을 구하면, 기준점의 시간과 현재 시간을 기준으로 계산하여 값을 보간한다.
		_float fStartTime = vecRotations[iPivot].fTime;	// 시간 변화율
		_float fEndTime = fStartTime;								
		_float fRatioTime = 0.f;		// 기준점과 다음점의 시간차이

		_vector qtStartRot = XMLoadFloat4(&vecRotations[iPivot].qtRot);
		_vector qtEndRot = qtStartRot;
		_vector qtFinalRot = qtStartRot;

		// 다음 요소가 있을 때
		if (iPivot + 1 < vecRotations.size())
		{
			// 다음 요소에 대한 값을 저장
			fEndTime = vecRotations[iPivot + 1].fTime;
			qtEndRot = XMLoadFloat4(&vecRotations[iPivot + 1].qtRot);
			
			fRatioTime = (fCurTime - fStartTime) / (fEndTime - fStartTime);
			qtFinalRot = XMQuaternionSlerp(qtStartRot, qtEndRot, fRatioTime);
		}

		vCalcRot = qtFinalRot;
	}

	if (!vecScales.empty())
	{
		iPivot = Calculate_PivotScale(fCurTime);

		// 지점을 구하면, 기준점의 시간과 현재 시간을 기준으로 계산하여 값을 보간한다.
		_float fStartTime = vecScales[iPivot].fTime;	// 시간 변화율
		_float fEndTime = fStartTime;								// 기준점과 다음점의 시간차이
		_float fRatioTime = fStartTime;

		_vector vStartScale = XMLoadFloat3(&vecScales[iPivot].vScale);	// 기준으로부터 변화율
		_vector vEndScale = vStartScale;
		_vector vFinalScale = vStartScale;

		// 다음 요소가 있을 때
		if (iPivot + 1 < vecScales.size())
		{
			// 다음 요소에 대한 값을 저장
			fEndTime = vecScales[iPivot + 1].fTime;
			vEndScale = XMLoadFloat3(&vecScales[iPivot + 1].vScale);

			fRatioTime = (fCurTime - fStartTime) / (fEndTime - fStartTime);	// 기준점과 다음점의 시간차이와의 비율을 구함
			vFinalScale += (vEndScale - vStartScale) * fRatioTime;				// 기준점과 다음점의 위치 값의 차를 구해 시간 변화율을 곱함
		}

		vCalcScale = vFinalScale;
	}

	_matrix matResult = XMMatrixAffineTransformation(vCalcScale, XMQuaternionIdentity(), vCalcRot, vCalcPos);
	_float4x4 matReturn;
	XMStoreFloat4x4(&matReturn, matResult);

	return matReturn;
}

_uint FBoneAnimNodeData::Calculate_PivotPosition(const _float& fCurTime) const
{
	_uint iPivot;

	// 벡터의 크기는 인덱스의 시간과 끝의 차이를 뜻함.
	_uint iSize = Cast<_uint>(vecPositions.size());
	// 첫점과 끝점의 시간차를 구해둠
	iPivot = Cast<_uint>(vecPositions.back().fTime - vecPositions.front().fTime);
	// 비율을 구해 현재 시간을 구해 실제 기준점을 구한다.
	iPivot = Cast<_uint>(fCurTime * (Cast<_float>(iSize) / Cast<_float>(iPivot)));

	while (true)
	{
		if (fCurTime >= vecPositions[iPivot].fTime)
		{
			// 만약 현재 인덱스의 다음 값이 배열을 벗어나면, 현재 위치를 통해 보간한다.
			if (iPivot + 1 >= iSize) { break; }
			// 아니면 인덱스를 내려 기준점을 찾는다.
			else { --iPivot; }
		}
		else
			++iPivot;

		// 기준점이 사이즈를 넘어가면 인덱스가 정해짐.
		if (iPivot >= iSize)
		{
			--iPivot;
			break;
		}
	}

	return iPivot;
}

_uint FBoneAnimNodeData::Calculate_PivotRotation(const _float& fCurTime) const
{
	_uint iPivot;

	// 벡터의 크기는 인덱스의 시간과 끝의 차이를 뜻함.
	_uint iSize = Cast<_uint>(vecRotations.size());
	// 첫점과 끝점의 시간차를 구해둠
	iPivot = Cast<_uint>(vecRotations.back().fTime - vecRotations.front().fTime);
	// 비율을 구해 현재 시간을 구해 실제 기준점을 구한다.
	iPivot = Cast<_uint>(fCurTime * (Cast<_float>(iSize) / Cast<_float>(iPivot)));

	while (true)
	{
		if (fCurTime >= vecRotations[iPivot].fTime)
		{
			// 만약 현재 인덱스의 다음 값이 배열을 벗어나면, 현재 위치를 통해 보간한다.
			if (iPivot + 1 >= iSize) { break; }
			// 아니면 인덱스를 내려 기준점을 찾는다.
			else { --iPivot; }
		}
		else { ++iPivot; }

		// 기준점이 사이즈를 넘어가면 인덱스가 정해짐.
		if (iPivot >= iSize)
		{
			--iPivot;
			break;
		}
	}

	return iPivot;
}

_uint FBoneAnimNodeData::Calculate_PivotScale(const _float& fCurTime) const
{
	_uint iPivot;

	// 벡터의 크기는 인덱스의 시간과 끝의 차이를 뜻함.
	_uint iSize = Cast<_uint>(vecScales.size());
	// 첫점과 끝점의 시간차를 구해둠
	iPivot = Cast<_uint>(vecScales.back().fTime - vecScales.front().fTime);
	// 비율을 구해 현재 시간을 구해 실제 기준점을 구한다.
	iPivot = Cast<_uint>(fCurTime * (Cast<_float>(iSize) / Cast<_float>(iPivot)));

	while (true)
	{
		if (fCurTime >= vecScales[iPivot].fTime)
		{
			// 만약 현재 인덱스의 다음 값이 배열을 벗어나면, 현재 위치를 통해 보간한다.
			if (iPivot + 1 >= iSize) { break; }
			// 아니면 인덱스를 내려 기준점을 찾는다.
			else { --iPivot; }
		}
		else
			++iPivot;

		// 기준점이 사이즈를 넘어가면 인덱스가 정해짐.
		if (iPivot >= iSize)
		{
			--iPivot;
			break;
		}
	}

	return iPivot;
}

FBoneAnimData* FBoneAnimData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FAnimData Create Failed");

		return nullptr;
	}

	return pInstance;
}

void FBoneAnimData::Free()
{
	for (auto& Pair : mapNodeAnim)
		Safe_Release(Pair.second);
	mapNodeAnim.clear();
}

const FBoneAnimNodeData* const FBoneAnimData::Find_AnimNodeData(_uint iIndex) const
{
	if (iIndex < 0 || iIndex >= vecAnim_BoneIndex.size())
		return nullptr;

	return vecAnim_BoneIndex[iIndex];
}

const FBoneAnimNodeData* const FBoneAnimData::Find_AnimNodeData(const wstring& strNodeKey) const
{
	auto iter = mapNodeAnim.find(strNodeKey);
	if (iter == mapNodeAnim.end())
		return nullptr;

	return (*iter).second;
}

void FBoneAnimData::Add_AnimNodeData(const wstring& strNodeKey, FBoneAnimNodeData* pAnimNodeData)
{
	if (pAnimNodeData->iBoneID >= vecAnim_BoneIndex.size())
		vecAnim_BoneIndex.resize(Cast<_uint>(pAnimNodeData->iBoneID + 1), nullptr);

	if (vecAnim_BoneIndex[pAnimNodeData->iBoneID] != nullptr)
	{
		Safe_Release(pAnimNodeData);
		return;
	}

	vecAnim_BoneIndex[pAnimNodeData->iBoneID] = pAnimNodeData;

	auto iter = mapNodeAnim.find(strNodeKey);
	if (iter != mapNodeAnim.end())
	{
		Safe_Release(pAnimNodeData);
		return;
	}

	mapNodeAnim.emplace(strNodeKey, pAnimNodeData);
}

_float FBoneAnimData::Calculate_Time(const _float& fTimeDelta, _float fCurTime, _bool bMod) const
{
	_float fModedTIme = Cast<_float>((bMod) ? fmodf(fCurTime, Cast<_float>(dfDuration)) : min(fCurTime, Cast<_float>(dfDuration)));		// 정해진 시간 뒤로 가지 않게 한다.
	_float fRatio = Cast<_float>(dfTickPerSecond) * fTimeDelta;		// 애니메이션과 시스템 시간변화율을 동기화한다.
	return fModedTIme * fRatio;
}


//---------------------------------- AnimGroup
FBoneAnimGroup* FBoneAnimGroup::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FAnimGroup Create Failed");

		return nullptr;
	}

	return pInstance;
}

void FBoneAnimGroup::Free()
{
	for (auto& Pair : mapAnimData)
		Safe_Release(Pair.second);

	mapAnimData.clear();
}

const FBoneAnimData* const FBoneAnimGroup::Find_AnimData(const wstring& strAnimKey)
{
	auto iter = mapAnimData.find(strAnimKey);
	if (iter == mapAnimData.end())
		return nullptr;

	return (*iter).second;
}

void FBoneAnimGroup::Add_AnimData(const wstring& strAnimKey, FBoneAnimData* pAnimData)
{
	auto iter = mapAnimData.find(strAnimKey);
	if (iter != mapAnimData.end())
	{
		Safe_Release(pAnimData);
		return;
	}

	mapAnimData.emplace(strAnimKey, pAnimData);
}


