#include "System/Data/AnimData.h"

FAnimNodeData* FAnimNodeData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FAnimNodeData Create Failed");

		return nullptr;
	}

	return pInstance;
}

void FAnimNodeData::Free()
{
	vecPositions.clear();
	vecRotations.clear();
	vecScales.clear();
}

_float4x4 FAnimNodeData::Interporated_Matrix(const _float& fCurTime)
{
	_uint iStartPivot, iPivot;
	_matrix matTransform = XMMatrixIdentity();
	_vec vCalcPos, vCalcRot, vCalcScale;
	if (!vecPositions.empty())
	{
		iPivot = Calculate_PivotPosition(fCurTime);

		// ������ ���ϸ�, �������� �ð��� ���� �ð��� �������� ����Ͽ� ���� �����Ѵ�.
		_float fStartTime = vecPositions[iPivot].fTime;	// �ð� ��ȭ��
		_float fEndTime = fStartTime;								// �������� �������� �ð�����
		_float fRatioTime = fStartTime;

		_vec vStartPos = XMLoadFloat3(&vecPositions[iPivot].vPos);	// �������κ��� ��ȭ��
		_vec vEndPos = vStartPos;
		_vec vFinalPos = vStartPos;

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

		_vec qtStartRot = XMLoadFloat4(&vecRotations[iPivot].qtRot);
		_vec qtEndRot = qtStartRot;
		_vec qtFinalRot = qtStartRot;

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
		_float fEndTime = fStartTime;								// �������� �������� �ð�����
		_float fRatioTime = fStartTime;

		_vec vStartScale = XMLoadFloat3(&vecScales[iPivot].vScale);	// �������κ��� ��ȭ��
		_vec vEndScale = vStartScale;
		_vec vFinalScale = vStartScale;

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

	_matrix matResult = XMMatrixAffineTransformation(vCalcScale, XMQuaternionIdentity(), vCalcRot, vCalcPos);
	_float4x4 matReturn;
	XMStoreFloat4x4(&matReturn, matResult);

	return matReturn;
}

_uint FAnimNodeData::Calculate_PivotPosition(const _float& fCurTime)
{
	_uint iPivot;

	// ������ ũ��� �ε����� �ð��� ���� ���̸� ����.
	_uint iSize = vecPositions.size();
	// ù���� ������ �ð����� ���ص�
	iPivot = Cast<_uint>(vecPositions.back().fTime - vecPositions.front().fTime);
	// ������ ���� ���� �ð��� ���� ���� �������� ���Ѵ�.
	iPivot = Cast<_uint>(fCurTime * (Cast<_float>(iSize) / Cast<_float>(iPivot)));

	while (true)
	{
		if (fCurTime >= vecPositions[iPivot].fTime)
		{
			// ���� ���� �ε����� ���� ���� �迭�� �����, ���� ��ġ�� ���� �����Ѵ�.
			if (iPivot + 1 >= iSize) { break; }
			// �ƴϸ� �ε����� ���� �������� ã�´�.
			else { --iPivot; }
		}
		else
			++iPivot;

		// �������� ����� �Ѿ�� �ε����� ������.
		if (iPivot >= iSize)
		{
			--iPivot;
			break;
		}
	}

	return iPivot;
}

_uint FAnimNodeData::Calculate_PivotRotation(const _float& fCurTime)
{
	_uint iPivot;

	// ������ ũ��� �ε����� �ð��� ���� ���̸� ����.
	_uint iSize = vecRotations.size();
	// ù���� ������ �ð����� ���ص�
	iPivot = Cast<_uint>(vecRotations.back().fTime - vecRotations.front().fTime);
	// ������ ���� ���� �ð��� ���� ���� �������� ���Ѵ�.
	iPivot = Cast<_uint>(fCurTime * (Cast<_float>(iSize) / Cast<_float>(iPivot)));

	while (true)
	{
		if (fCurTime >= vecRotations[iPivot].fTime)
		{
			// ���� ���� �ε����� ���� ���� �迭�� �����, ���� ��ġ�� ���� �����Ѵ�.
			if (iPivot + 1 >= iSize) { break; }
			// �ƴϸ� �ε����� ���� �������� ã�´�.
			else { --iPivot; }
		}
		else { ++iPivot; }

		// �������� ����� �Ѿ�� �ε����� ������.
		if (iPivot >= iSize)
		{
			--iPivot;
			break;
		}
	}

	return iPivot;
}

_uint FAnimNodeData::Calculate_PivotScale(const _float& fCurTime)
{
	_uint iPivot;

	// ������ ũ��� �ε����� �ð��� ���� ���̸� ����.
	_uint iSize = vecScales.size();
	// ù���� ������ �ð����� ���ص�
	iPivot = Cast<_uint>(vecScales.back().fTime - vecScales.front().fTime);
	// ������ ���� ���� �ð��� ���� ���� �������� ���Ѵ�.
	iPivot = Cast<_uint>(fCurTime * (Cast<_float>(iSize) / Cast<_float>(iPivot)));

	while (true)
	{
		if (fCurTime >= vecScales[iPivot].fTime)
		{
			// ���� ���� �ε����� ���� ���� �迭�� �����, ���� ��ġ�� ���� �����Ѵ�.
			if (iPivot + 1 >= iSize) { break; }
			// �ƴϸ� �ε����� ���� �������� ã�´�.
			else { --iPivot; }
		}
		else
			++iPivot;

		// �������� ����� �Ѿ�� �ε����� ������.
		if (iPivot >= iSize)
		{
			--iPivot;
			break;
		}
	}

	return iPivot;
}

FAnimData* FAnimData::Create()
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

void FAnimData::Free()
{
	for (auto& Pair : mapNodeAnim)
		Safe_Release(Pair.second);
	mapNodeAnim.clear();
}

const FAnimNodeData* const FAnimData::Get_AnimNodeData(const wstring& strNodeKey)
{
	auto iter = mapNodeAnim.find(strNodeKey);
	if (iter == mapNodeAnim.end())
		return nullptr;

	return (*iter).second;
}

void FAnimData::Add_AnimNodeData(const wstring& strNodeKey, FAnimNodeData* pAnimNodeData)
{
	auto iter = mapNodeAnim.find(strNodeKey);
	if (iter != mapNodeAnim.end())
	{
		Safe_Release(pAnimNodeData);
		return;
	}

	mapNodeAnim.emplace(strNodeKey, pAnimNodeData);
}

_float FAnimData::Calculate_Time(const _float& fTimeDelta, _float fCurTime, _bool bMod)
{
	_float fModedTIme = (bMod) ? fmodf(fCurTime, fDuration) : min(fCurTime, fDuration);		// ������ �ð� �ڷ� ���� �ʰ� �Ѵ�.
	_float fRatio = fTickPerSecond * fTimeDelta;		// �ִϸ��̼ǰ� �ý��� �ð���ȭ���� ����ȭ�Ѵ�.
	return fModedTIme * fRatio;
}


//---------------------------------- AnimGroup
FAnimGroup* FAnimGroup::Create()
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

void FAnimGroup::Free()
{
	for (auto& Pair : mapAnimData)
		Safe_Release(Pair.second);

	mapAnimData.clear();
}

const FAnimData* const FAnimGroup::Get_AnimData(const wstring& strAnimKey)
{
	auto iter = mapAnimData.find(strAnimKey);
	if (iter == mapAnimData.end())
		return nullptr;

	return (*iter).second;
}

void FAnimGroup::Add_AnimData(const wstring& strAnimKey, FAnimData* pAnimData)
{
	auto iter = mapAnimData.find(strAnimKey);
	if (iter != mapAnimData.end())
	{
		Safe_Release(pAnimData);
		return;
	}

	mapAnimData.emplace(strAnimKey, pAnimData);
}


