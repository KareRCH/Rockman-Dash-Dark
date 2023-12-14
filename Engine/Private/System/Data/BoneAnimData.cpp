#include "System/Data/BoneAnimData.h"

FBoneAnimChannelData* FBoneAnimChannelData::Create()
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

void FBoneAnimChannelData::Free()
{
	vecPositions.clear();
	vecRotations.clear();
	vecScales.clear();
}

_float4x4 FBoneAnimChannelData::Interporated_Matrix(const _float& fCurTime) const
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
		_float fEndTime = fStartTime;					// 기준점과 다음점의 시간차이
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

	vCalcPos.m128_f32[3] = 1.f;
	_matrix matResult = XMMatrixAffineTransformation(vCalcScale, XMQuaternionIdentity(), vCalcRot, vCalcPos);
	_float4x4 matReturn;
	XMStoreFloat4x4(&matReturn, matResult);

	return matReturn;
}

_uint FBoneAnimChannelData::Calculate_PivotPosition(const _float& fCurTime) const
{
	_uint iPivot;

	// 벡터의 크기는 인덱스의 시간과 끝의 차이를 뜻함.
	_uint iSize = Cast<_uint>(vecPositions.size());
	// 첫점과 끝점의 시간차를 구해둠
	iPivot = Cast<_uint>(vecPositions.back().fTime - vecPositions.front().fTime);
	// 비율을 구해 현재 시간을 구해 실제 기준점을 구한다.
	iPivot = Cast<_uint>(fCurTime * (Cast<_float>(iSize) / Cast<_float>(iPivot)));

	if (iPivot - 1 == UINT_MAX)
		iPivot = 0;
	if (iPivot >= iSize)
		iPivot = iSize - 1;
	
	while (true)
	{
		// 현재 기준점에 해당하는 시간 값이 해당된다면, 더 작은 인덱스와의 비교 후에 결정한다.
		// 현재 시간 값이 비교값을 넘을 때, 최대한 하위 인덱스와 비교하여 맞춘다.
		if (fCurTime >= vecPositions[iPivot].fTime)
		{
			if (iPivot + 1 < iSize
			&& fCurTime >= vecPositions[iPivot + 1].fTime)
			{
				++iPivot;
			}
			else { break; }
		}
		else
		{
			// 여전히 시간 값이 작은 값이면 기준점을 내린다.
			if (iPivot - 1 != UINT_MAX
				&& fCurTime >= vecPositions[iPivot - 1].fTime)
			{
				--iPivot;
			}
			else { break; }
		}
	}

	return iPivot;
}

_uint FBoneAnimChannelData::Calculate_PivotRotation(const _float& fCurTime) const
{
	_uint iPivot;

	// 벡터의 크기는 인덱스의 시간과 끝의 차이를 뜻함.
	_uint iSize = Cast<_uint>(vecRotations.size());
	// 첫점과 끝점의 시간차를 구해둠
	iPivot = Cast<_uint>(vecRotations.back().fTime - vecRotations.front().fTime);
	// 비율을 구해 현재 시간을 구해 실제 기준점을 구한다.
	iPivot = Cast<_uint>(fCurTime * (Cast<_float>(iSize) / Cast<_float>(iPivot)));

	if (iPivot - 1 == UINT_MAX)
		iPivot = 0;
	if (iPivot >= iSize)
		iPivot = iSize - 1;

	while (true)
	{
		// 현재 기준점에 해당하는 시간 값이 해당된다면, 더 작은 인덱스와의 비교 후에 결정한다.
		// 현재 시간 값이 비교값을 넘을 때, 최대한 하위 인덱스와 비교하여 맞춘다.
		if (fCurTime >= vecRotations[iPivot].fTime)
		{
			if (iPivot + 1 < iSize
				&& fCurTime >= vecRotations[iPivot + 1].fTime)
			{
				++iPivot;
			}
			else { break; }
		}
		else
		{
			// 여전히 시간 값이 작은 값이면 기준점을 내린다.
			if (iPivot - 1 != UINT_MAX
				&& fCurTime >= vecRotations[iPivot - 1].fTime)
			{
				--iPivot;
			}
			else { break; }
		}
	}

	return iPivot;
}

_uint FBoneAnimChannelData::Calculate_PivotScale(const _float& fCurTime) const
{
	_uint iPivot;

	// 벡터의 크기는 인덱스의 시간과 끝의 차이를 뜻함.
	_uint iSize = Cast<_uint>(vecScales.size());
	// 첫점과 끝점의 시간차를 구해둠
	iPivot = Cast<_uint>(vecScales.back().fTime - vecScales.front().fTime);
	// 비율을 구해 현재 시간을 구해 실제 기준점을 구한다.
	iPivot = Cast<_uint>(fCurTime * (Cast<_float>(iSize) / Cast<_float>(iPivot)));

	if (iPivot - 1 == UINT_MAX)
		iPivot = 0;
	if (iPivot >= iSize)
		iPivot = iSize - 1;

	while (true)
	{
		// 현재 기준점에 해당하는 시간 값이 해당된다면, 더 작은 인덱스와의 비교 후에 결정한다.
		// 현재 시간 값이 비교값을 넘을 때, 최대한 하위 인덱스와 비교하여 맞춘다.
		if (fCurTime >= vecScales[iPivot].fTime)
		{
			if (iPivot + 1 < iSize
				&& fCurTime >= vecScales[iPivot + 1].fTime)
			{
				++iPivot;
			}
			else { break; }
		}
		else
		{
			// 여전히 시간 값이 작은 값이면 기준점을 내린다.
			if (iPivot - 1 != UINT_MAX
				&& fCurTime >= vecScales[iPivot - 1].fTime)
			{
				--iPivot;
			}
			else { break; }
		}
	}

	return iPivot;
}

FBoneAnimData* FBoneAnimData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		MSG_BOX("FAnimData Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void FBoneAnimData::Free()
{
	for (auto& Pair : mapAnimNodes)
		Safe_Release(Pair.second);
	mapAnimNodes.clear();
}

const FBoneAnimChannelData* const FBoneAnimData::Find_AnimNodeData(_uint iIndex) const
{
	if (iIndex < 0 || iIndex >= vecAnimNodes.size())
		return nullptr;

	return vecAnimNodes[iIndex];
}

const FBoneAnimChannelData* const FBoneAnimData::Find_AnimNodeData(const wstring& strNodeKey) const
{
	auto iter = mapAnimNodes.find(strNodeKey);
	if (iter == mapAnimNodes.end())
		return nullptr;

	return (*iter).second;
}

void FBoneAnimData::Add_AnimNodeData(const wstring& strNodeKey, FBoneAnimChannelData* pAnimNodeData)
{
	if (pAnimNodeData->iBoneID >= vecAnimNodes.size())
		vecAnimNodes.resize(Cast<_uint>(pAnimNodeData->iBoneID + 1), nullptr);

	if (vecAnimNodes[pAnimNodeData->iBoneID] != nullptr)
	{
		Safe_Release(pAnimNodeData);
		return;
	}

	vecAnimNodes[pAnimNodeData->iBoneID] = pAnimNodeData;

	auto iter = mapAnimNodes.find(strNodeKey);
	if (iter != mapAnimNodes.end())
	{
		Safe_Release(pAnimNodeData);
		return;
	}

	mapAnimNodes.emplace(strNodeKey, pAnimNodeData);
}

_float FBoneAnimData::Calculate_Time(_float fCurTime, _bool bMod) const
{
	_float fDuration = Cast<_float>(fDuration);
	_float fTickPerSecond = Cast<_float>(fTickPerSecond);
	_float fConvCurTime = fCurTime * fTickPerSecond;

	_float fModedTIme = Cast<_float>((bMod) ? fmodf(fConvCurTime, fDuration) : min(fConvCurTime, fDuration));		// 정해진 시간 뒤로 가지 않게 한다.
	return fModedTIme;
}


//---------------------------------- AnimGroup
FBoneAnimGroup* FBoneAnimGroup::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		MSG_BOX("FAnimGroup Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void FBoneAnimGroup::Free()
{
	for (auto& Pair : mapAnimDatas)
		Safe_Release(Pair.second);

	mapAnimDatas.clear();
}

FBoneAnimData* const FBoneAnimGroup::Find_BoneAnim(const _uint iIndex)
{
	if (iIndex < 0 && iIndex >= vecAnimDatas.size())
		return nullptr;

	return vecAnimDatas[iIndex];
}

FBoneAnimData* const FBoneAnimGroup::Find_BoneAnim(const wstring& strAnimKey)
{
	auto iter = mapAnimDatas.find(strAnimKey);
	if (iter == mapAnimDatas.end())
		return nullptr;

	return (*iter).second;
}

void FBoneAnimGroup::Add_BoneAnim(const wstring& strAnimKey, FBoneAnimData* pAnimData)
{
	auto iter = mapAnimDatas.find(strAnimKey);
	if (iter != mapAnimDatas.end())
	{
		Safe_Release(pAnimData);
		return;
	}

	mapAnimDatas.emplace(strAnimKey, pAnimData);
}


