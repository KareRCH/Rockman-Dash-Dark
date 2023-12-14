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

		// ������ ���ϸ�, �������� �ð��� ���� �ð��� �������� ����Ͽ� ���� �����Ѵ�.
		_float fStartTime = vecPositions[iPivot].fTime;	// �ð� ��ȭ��
		_float fEndTime = fStartTime;								// �������� �������� �ð�����
		_float fRatioTime = fStartTime;

		_vector vStartPos = XMLoadFloat3(&vecPositions[iPivot].vPos);	// �������κ��� ��ȭ��
		_vector vEndPos = vStartPos;
		_vector vFinalPos = vStartPos;

		// ���� ��Ұ� ���� ��
		if (iPivot + 1 < vecRotations.size())
		{
			// ���� ��ҿ� ���� ���� ����
			fEndTime = vecPositions[iPivot + 1].fTime;
			vEndPos = XMLoadFloat3(&vecPositions[iPivot + 1].vPos);

			fRatioTime = (fCurTime - fStartTime) / (fEndTime - fStartTime);	// �������� �������� �ð����̿��� ������ ����
			vFinalPos += (vEndPos - vStartPos) * fRatioTime;				// �������� �������� ��ġ ���� ���� ���� �ð� ��ȭ���� ����
		}

		vCalcPos = vFinalPos;
	}

	if (!vecRotations.empty())
	{
		iPivot = Calculate_PivotRotation(fCurTime);

		// ������ ���ϸ�, �������� �ð��� ���� �ð��� �������� ����Ͽ� ���� �����Ѵ�.
		_float fStartTime = vecRotations[iPivot].fTime;	// �ð� ��ȭ��
		_float fEndTime = fStartTime;								
		_float fRatioTime = 0.f;		// �������� �������� �ð�����

		_vector qtStartRot = XMLoadFloat4(&vecRotations[iPivot].qtRot);
		_vector qtEndRot = qtStartRot;
		_vector qtFinalRot = qtStartRot;

		// ���� ��Ұ� ���� ��
		if (iPivot + 1 < vecRotations.size())
		{
			// ���� ��ҿ� ���� ���� ����
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

		// ������ ���ϸ�, �������� �ð��� ���� �ð��� �������� ����Ͽ� ���� �����Ѵ�.
		_float fStartTime = vecScales[iPivot].fTime;	// �ð� ��ȭ��
		_float fEndTime = fStartTime;					// �������� �������� �ð�����
		_float fRatioTime = fStartTime;

		_vector vStartScale = XMLoadFloat3(&vecScales[iPivot].vScale);	// �������κ��� ��ȭ��
		_vector vEndScale = vStartScale;
		_vector vFinalScale = vStartScale;

		// ���� ��Ұ� ���� ��
		if (iPivot + 1 < vecScales.size())
		{
			// ���� ��ҿ� ���� ���� ����
			fEndTime = vecScales[iPivot + 1].fTime;
			vEndScale = XMLoadFloat3(&vecScales[iPivot + 1].vScale);

			fRatioTime = (fCurTime - fStartTime) / (fEndTime - fStartTime);	// �������� �������� �ð����̿��� ������ ����
			vFinalScale += (vEndScale - vStartScale) * fRatioTime;				// �������� �������� ��ġ ���� ���� ���� �ð� ��ȭ���� ����
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

	// ������ ũ��� �ε����� �ð��� ���� ���̸� ����.
	_uint iSize = Cast<_uint>(vecPositions.size());
	// ù���� ������ �ð����� ���ص�
	iPivot = Cast<_uint>(vecPositions.back().fTime - vecPositions.front().fTime);
	// ������ ���� ���� �ð��� ���� ���� �������� ���Ѵ�.
	iPivot = Cast<_uint>(fCurTime * (Cast<_float>(iSize) / Cast<_float>(iPivot)));

	if (iPivot - 1 == UINT_MAX)
		iPivot = 0;
	if (iPivot >= iSize)
		iPivot = iSize - 1;
	
	while (true)
	{
		// ���� �������� �ش��ϴ� �ð� ���� �ش�ȴٸ�, �� ���� �ε������� �� �Ŀ� �����Ѵ�.
		// ���� �ð� ���� �񱳰��� ���� ��, �ִ��� ���� �ε����� ���Ͽ� �����.
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
			// ������ �ð� ���� ���� ���̸� �������� ������.
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

	// ������ ũ��� �ε����� �ð��� ���� ���̸� ����.
	_uint iSize = Cast<_uint>(vecRotations.size());
	// ù���� ������ �ð����� ���ص�
	iPivot = Cast<_uint>(vecRotations.back().fTime - vecRotations.front().fTime);
	// ������ ���� ���� �ð��� ���� ���� �������� ���Ѵ�.
	iPivot = Cast<_uint>(fCurTime * (Cast<_float>(iSize) / Cast<_float>(iPivot)));

	if (iPivot - 1 == UINT_MAX)
		iPivot = 0;
	if (iPivot >= iSize)
		iPivot = iSize - 1;

	while (true)
	{
		// ���� �������� �ش��ϴ� �ð� ���� �ش�ȴٸ�, �� ���� �ε������� �� �Ŀ� �����Ѵ�.
		// ���� �ð� ���� �񱳰��� ���� ��, �ִ��� ���� �ε����� ���Ͽ� �����.
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
			// ������ �ð� ���� ���� ���̸� �������� ������.
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

	// ������ ũ��� �ε����� �ð��� ���� ���̸� ����.
	_uint iSize = Cast<_uint>(vecScales.size());
	// ù���� ������ �ð����� ���ص�
	iPivot = Cast<_uint>(vecScales.back().fTime - vecScales.front().fTime);
	// ������ ���� ���� �ð��� ���� ���� �������� ���Ѵ�.
	iPivot = Cast<_uint>(fCurTime * (Cast<_float>(iSize) / Cast<_float>(iPivot)));

	if (iPivot - 1 == UINT_MAX)
		iPivot = 0;
	if (iPivot >= iSize)
		iPivot = iSize - 1;

	while (true)
	{
		// ���� �������� �ش��ϴ� �ð� ���� �ش�ȴٸ�, �� ���� �ε������� �� �Ŀ� �����Ѵ�.
		// ���� �ð� ���� �񱳰��� ���� ��, �ִ��� ���� �ε����� ���Ͽ� �����.
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
			// ������ �ð� ���� ���� ���̸� �������� ������.
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

	_float fModedTIme = Cast<_float>((bMod) ? fmodf(fConvCurTime, fDuration) : min(fConvCurTime, fDuration));		// ������ �ð� �ڷ� ���� �ʰ� �Ѵ�.
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


