#include "System/Data/BoneAnimData.h"

FBoneAnimChannelData* FBoneAnimChannelData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		MSG_BOX("FAnimNodeData Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void FBoneAnimChannelData::Free()
{

}

_float4x4 FBoneAnimChannelData::Interpolated_Matrix(const _float& fCurTrackPos) const
{
	if (vecKeyFrames.empty())
		return _float4x4();

	// �� �Լ��� ȣ���ϱ����� ������ �ƴϳķ� �ð����� �������� ���´�.
	_uint iPivot = Calculate_Pivot(fCurTrackPos);
	_matrix matResult = XMMatrixIdentity();
	_vector vCalcPos, vCalcRot, vCalcScale;


	_vector vStartValue, vEndValue, vFinalValue;
	_float fStartTime, fEndTime, fRatioTime;
	fStartTime = vecKeyFrames[iPivot].fTrackPosition;	// �ð� ��ȭ��
	fEndTime = fStartTime;
	fRatioTime = 0.f;

	_bool bIsExistsNextKeyFrame = iPivot + 1 < m_iNumKeyFrames;
	// ���� Ű�������� �ִٸ� ���ð��� ���Ѵ�.
	if (bIsExistsNextKeyFrame)
	{
		fEndTime = vecKeyFrames[iPivot + 1].fTrackPosition;
		fRatioTime = (fCurTrackPos - fStartTime) / (fEndTime - fStartTime);	// �������� �������� �ð����̿��� ������ ����
	}

	// ������ ���ϸ�, �������� �ð��� ���� �ð��� �������� ����Ͽ� ���� �����Ѵ�.
	{
		vStartValue = vFinalValue = XMLoadFloat3(&vecKeyFrames[iPivot].vPos);	// �������κ��� ��ȭ��

		// ���� ��Ұ� ���� ��
		if (bIsExistsNextKeyFrame)
		{
			// ���� ��ҿ� ���� ���� ����
			vEndValue = XMLoadFloat3(&vecKeyFrames[iPivot + 1].vPos);
			vFinalValue += (vEndValue - vStartValue) * fRatioTime;				// �������� �������� ��ġ ���� ���� ���� �ð� ��ȭ���� ����
		}

		vCalcPos = vFinalValue;
	}




	// ������ ���ϸ�, �������� �ð��� ���� �ð��� �������� ����Ͽ� ���� �����Ѵ�.
	{
		vStartValue = vFinalValue = XMLoadFloat4(&vecKeyFrames[iPivot].qtRot);

		// ���� ��Ұ� ���� ��
		if (bIsExistsNextKeyFrame)
		{
			// ���� ��ҿ� ���� ���� ����
			vEndValue = XMLoadFloat4(&vecKeyFrames[iPivot + 1].qtRot);
			vFinalValue = XMQuaternionSlerp(vStartValue, vEndValue, fRatioTime);
		}

		vCalcRot = vFinalValue;
	}





	// ������ ���ϸ�, �������� �ð��� ���� �ð��� �������� ����Ͽ� ���� �����Ѵ�.
	{
		vStartValue = vFinalValue = XMLoadFloat3(&vecKeyFrames[iPivot].vScale);	// �������κ��� ��ȭ��

		// ���� ��Ұ� ���� ��
		if (bIsExistsNextKeyFrame)
		{
			// ���� ��ҿ� ���� ���� ����
			vEndValue = XMLoadFloat3(&vecKeyFrames[iPivot + 1].vScale);
			vFinalValue += (vEndValue - vStartValue) * fRatioTime;				// �������� �������� ��ġ ���� ���� ���� �ð� ��ȭ���� ����
		}

		vCalcScale = vFinalValue;
	}

	matResult = XMMatrixAffineTransformation(vCalcScale, XMQuaternionIdentity(), vCalcRot, vCalcPos);
	_float4x4 matReturn = {};
	XMStoreFloat4x4(&matReturn, matResult);

	return matReturn;
}

FKeyFrame FBoneAnimChannelData::Interpolated_KeyFrame(const _float& fCurTrackPos) const
{
	if (vecKeyFrames.empty())
		return FKeyFrame();

	// �� �Լ��� ȣ���ϱ����� ������ �ƴϳķ� �ð����� �������� ���´�.
	_uint iPivot = Calculate_Pivot(fCurTrackPos);
	_vector vCalcPos, vCalcRot, vCalcScale;


	_vector vStartValue, vEndValue, vFinalValue;
	_float fStartTime, fEndTime, fRatioTime;
	fStartTime = vecKeyFrames[iPivot].fTrackPosition;	// �ð� ��ȭ��
	fEndTime = fStartTime;
	fRatioTime = 0.f;

	_bool bIsExistsNextKeyFrame = iPivot + 1 < m_iNumKeyFrames;
	// ���� Ű�������� �ִٸ� ���ð��� ���Ѵ�.
	if (bIsExistsNextKeyFrame)
	{
		fEndTime = vecKeyFrames[iPivot + 1].fTrackPosition;
		fRatioTime = (fCurTrackPos - fStartTime) / (fEndTime - fStartTime);	// �������� �������� �ð����̿��� ������ ����
	}

	// ������ ���ϸ�, �������� �ð��� ���� �ð��� �������� ����Ͽ� ���� �����Ѵ�.
	{
		vStartValue = vFinalValue = XMLoadFloat3(&vecKeyFrames[iPivot].vPos);	// �������κ��� ��ȭ��

		// ���� ��Ұ� ���� ��
		if (bIsExistsNextKeyFrame)
		{
			// ���� ��ҿ� ���� ���� ����
			vEndValue = XMLoadFloat3(&vecKeyFrames[iPivot + 1].vPos);
			vFinalValue = XMVectorLerp(vStartValue, vEndValue, fRatioTime);		// �������� �������� ��ġ ���� ���� ���� �ð� ��ȭ���� ����

			//if (m_iNumKeyFrames == 21)
				//cout << iPivot << "[" << fCurTrackPos << ", " << fStartTime << ", " << fEndTime << ", " << fRatioTime << "] : " << XMVectorGetX(vFinalValue) << " " << XMVectorGetY(vFinalValue) << " " << XMVectorGetZ(vFinalValue) << endl;
		}

		vCalcPos = vFinalValue;
	}




	// ������ ���ϸ�, �������� �ð��� ���� �ð��� �������� ����Ͽ� ���� �����Ѵ�.
	{
		vStartValue = vFinalValue = XMLoadFloat4(&vecKeyFrames[iPivot].qtRot);

		// ���� ��Ұ� ���� ��
		if (bIsExistsNextKeyFrame)
		{
			// ���� ��ҿ� ���� ���� ����
			vEndValue = XMLoadFloat4(&vecKeyFrames[iPivot + 1].qtRot);
			vFinalValue = XMQuaternionSlerp(vStartValue, vEndValue, fRatioTime);
		}

		vCalcRot = vFinalValue;
	}





	// ������ ���ϸ�, �������� �ð��� ���� �ð��� �������� ����Ͽ� ���� �����Ѵ�.
	{
		vStartValue = vFinalValue = XMLoadFloat3(&vecKeyFrames[iPivot].vScale);	// �������κ��� ��ȭ��

		// ���� ��Ұ� ���� ��
		if (bIsExistsNextKeyFrame)
		{
			// ���� ��ҿ� ���� ���� ����
			vEndValue = XMLoadFloat3(&vecKeyFrames[iPivot + 1].vScale);
			vFinalValue = XMVectorLerp(vStartValue, vEndValue, fRatioTime);		// �������� �������� ��ġ ���� ���� ���� �ð� ��ȭ���� ����
		}

		vCalcScale = vFinalValue;
	}

	FKeyFrame KeyFrame = {};
	XMStoreFloat3(&KeyFrame.vPos, vCalcPos);
	XMStoreFloat4(&KeyFrame.qtRot, vCalcRot);
	XMStoreFloat3(&KeyFrame.vScale, vCalcScale);

	return KeyFrame;
}

_uint FBoneAnimChannelData::Calculate_Pivot(const _float& fCurTrackPos) const
{
	// ������ ũ��� �ε����� �ð��� ���� ���̸� ����.
	if (m_iNumKeyFrames == 0)
		return 0;

	_uint iPivot;
	// ù���� ������ �ð����� ���ص�
	iPivot = Cast<_uint>(vecKeyFrames.back().fTrackPosition - vecKeyFrames.front().fTrackPosition);
	// ������ ���� ���� �ð��� ���� ���� �������� ���Ѵ�.
	iPivot = Cast<_uint>(fCurTrackPos * (Cast<_float>(m_iNumKeyFrames) / Cast<_float>(iPivot)));

	if (iPivot - 1 == UINT_MAX)
		iPivot = 0;
	if (iPivot >= m_iNumKeyFrames)
		iPivot = m_iNumKeyFrames - 1;
	
	while (true)
	{
		/*if (iSize == 21 && iPivot == 9 || iPivot == 10)
			cout << iPivot << endl;*/
		// ���� �������� �ش��ϴ� �ð� ���� �ش�ȴٸ�, �� ���� �ε������� �� �Ŀ� �����Ѵ�.
		// ���� �ð� ���� �񱳰��� ���� ��, �ִ��� ���� �ε����� ���Ͽ� �����.
		// �Ѱ����� ���� Ű�� ���ؾ��Ѵ�.
		if (fCurTrackPos >= vecKeyFrames[iPivot].fTrackPosition)
		{
			// ���� ������ ����
			if (iPivot + 1 < m_iNumKeyFrames)
			{
				// ���̰���
				if (fCurTrackPos < vecKeyFrames[iPivot + 1].fTrackPosition)
				{ break; }
				// ���� ������ ���̰� ã��
				else { ++iPivot; continue; }
			}
			// ���� �������� ����. ���簪�� �����Ѵ�.
			else { break; }
			// ���� �������� ����. ���� ������ �˻��Ѵ�.
			if (iPivot - 1 != UINT_MAX)
			{
				// ���� �������� ���� ���ؼ� ������ �Űܾ��ϴ��� �Ǻ��Ѵ�.
				if (fCurTrackPos >= vecKeyFrames[iPivot - 1].fTrackPosition)
				{
					--iPivot;
					continue;
				}
				else
					break;
			}
			
		}
		// ���̰��� ���ϱ� ���� �Ǻ� ��ġ�� �����ؾ��Ѵ�.
		else
		{
			// ���� ������ ����
			if (iPivot + 1 < m_iNumKeyFrames)
			{
				// ���������� ���ϱ�
				if (fCurTrackPos >= vecKeyFrames[iPivot + 1].fTrackPosition)
				{ ++iPivot; continue; }
			}
			// ���� �������� ����. ���� ������ �˻��Ѵ�.
			if (iPivot - 1 != UINT_MAX)
			{
				if (m_iNumKeyFrames == 21 && fCurTrackPos >= 15.f)
				{
					int t = 0;
				}
				// ���� �������� ���� ���ؼ� ������ �Űܾ��ϴ��� �Ǻ��Ѵ�.
				if (fCurTrackPos >= vecKeyFrames[iPivot - 1].fTrackPosition
					|| fCurTrackPos < vecKeyFrames[iPivot - 1].fTrackPosition)
				{ --iPivot; continue; }
			}
			break;
		}
	}

	/*if (m_iNumKeyFrames == 21)
		cout << iPivot << endl;*/

	return iPivot;
}

FBoneAnimData* FBoneAnimData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		MSG_BOX("FAnimData Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void FBoneAnimData::Free()
{
	for (auto& Pair : mapAnimChannels)
		Safe_Release(Pair.second);
	mapAnimChannels.clear();
}

const FBoneAnimChannelData* const FBoneAnimData::Find_AnimChannelData(_uint iIndex) const
{
	if (iIndex < 0 || iIndex >= vecAnimChannels.size())
		return nullptr;

	return vecAnimChannels[iIndex];
}

const FBoneAnimChannelData* const FBoneAnimData::Find_AnimChannelData(const wstring& strNodeKey) const
{
	auto iter = mapAnimChannels.find(strNodeKey);
	if (iter == mapAnimChannels.end())
		return nullptr;

	return (*iter).second;
}

void FBoneAnimData::Add_AnimChannelData(const wstring& strNodeKey, FBoneAnimChannelData* pAnimNodeData)
{
	if (pAnimNodeData->iBoneID >= vecAnimChannels.size())
		vecAnimChannels.resize(Cast<_uint>(pAnimNodeData->iBoneID + 1), nullptr);

	if (vecAnimChannels[pAnimNodeData->iBoneID] != nullptr)
	{
		Safe_Release(pAnimNodeData);
		return;
	}

	vecAnimChannels[pAnimNodeData->iBoneID] = pAnimNodeData;

	auto iter = mapAnimChannels.find(strNodeKey);
	if (iter != mapAnimChannels.end())
	{
		Safe_Release(pAnimNodeData);
		return;
	}

	mapAnimChannels.emplace(strNodeKey, pAnimNodeData);
}

_float FBoneAnimData::Calculate_Time(_float fCurTime, _bool bMod) const
{
	_float fConvCurTime = fCurTime * fTickPerSecond;

	_float fModedTIme = Cast<_float>((bMod) ? fmodf(fConvCurTime, fDuration) : min(fConvCurTime, fDuration));		// ������ �ð� �ڷ� ���� �ʰ� �Ѵ�.
	return fModedTIme;
}



//---------------------------------- AnimGroup
CBoneAnimGroup* CBoneAnimGroup::Create()
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

void CBoneAnimGroup::Free()
{
	for (auto& Pair : mapAnimDatas)
		Safe_Release(Pair.second);

	mapAnimDatas.clear();
}

FBoneAnimData* const CBoneAnimGroup::Find_BoneAnim(const _uint iIndex)
{
	if (iIndex < 0 && iIndex >= vecAnimDatas.size())
		return nullptr;

	return vecAnimDatas[iIndex];
}

FBoneAnimData* const CBoneAnimGroup::Find_BoneAnim(const wstring& strAnimKey)
{
	auto iter = mapAnimDatas.find(strAnimKey);
	if (iter == mapAnimDatas.end())
		return nullptr;

	return (*iter).second;
}

void CBoneAnimGroup::Add_BoneAnim(const wstring& strAnimKey, FBoneAnimData* pAnimData)
{
	auto iter = mapAnimDatas.find(strAnimKey);
	if (iter != mapAnimDatas.end())
	{ 
		Safe_Release(pAnimData);
		return;
	}

	mapAnimDatas.emplace(strAnimKey, pAnimData);
	vecAnimDatas.push_back(pAnimData);
	++iNumAnims;
}

FKeyFrame CBoneAnimGroup::Interpolated_Anim(const _uint iAnimIndex, const _uint iChannelIndex, const _float& fCurTrackPos) const
{
	if (iAnimIndex < 0 || iAnimIndex >= vecAnimDatas.size())
		return FKeyFrame();

	auto pChannel = vecAnimDatas[iAnimIndex]->Find_AnimChannelData(iChannelIndex);
	if (pChannel == nullptr)
		return FKeyFrame();

	return pChannel->Interpolated_KeyFrame(fCurTrackPos);
}

void CBoneAnimGroup::Interpolated_Anims(FKeyFrame* pKeyFrames, size_t iNumKeyFrames, FAnimInterpolate* pArrInterpolateData, size_t iNumInterpolates)
{
	if (iNumInterpolates < 0)
		return;

	// ����ġ ����� �� ����ġ�� ���� ȥ�ո���̴�.
	// ������� �ܺο��� ���־�� �Ѵ�.
	// �ش� �Լ������� �ܺηκ��� ���� �����ͷκ��� �ִϸ��̼��� ȥ���� ���� �̾Ƴ��µ��� ���ȴ�.
	vector<_vector> vecPos, vecRot, vecScale;
	vecPos.resize(iNumKeyFrames, {});
	vecRot.resize(iNumKeyFrames, XMQuaternionIdentity());
	vecScale.resize(iNumKeyFrames, XMVectorSet(1.f, 1.f, 1.f, 0.f));

	// ����ġ�� �ܺο��� ���� 1�̵ȴٴ� �����Ͽ� �����Ѵ�.
	// �ִϸ��̼ǰ� ����, �ִϸ��̼� ����ŷ ��� ���� 1�� ���·� �����Ѵ�.
	_float fWeight = 0.f, fWeightRatio;
	for (_uint i = 0; i < iNumInterpolates; i++)
	{
		FAnimInterpolate* pData = &pArrInterpolateData[i];
		FBoneAnimData* pBoneAnim = Find_BoneAnim(pData->iAnimID);
		_uint iNumChannel = Cast<_uint>(pData->vecChannelIDs.size());
		

		if (pBoneAnim == nullptr)
			continue;

		fWeight += pData->fWeight;
		if (fWeight != 0.f)
			fWeightRatio = (pData->fWeight / fWeight);
		else
			fWeightRatio = 0.f;
		
		for (_uint j = 0; j < iNumChannel; j++)
		{
			FBoneAnimChannelData* pChannel = ConCast<FBoneAnimChannelData*>(pBoneAnim->Find_AnimChannelData(pData->vecChannelIDs[j]));
			if (pChannel == nullptr)
				continue;

			// ���� ����, ä�ο� �����ؼ� ä���� �������� �����´�.
			FKeyFrame InterKeyFrame = pChannel->Interpolated_KeyFrame(pData->fTrackPos);

			vecPos[j] = XMVectorLerp(vecPos[j], XMLoadFloat3(&InterKeyFrame.vPos), fWeightRatio);
			vecRot[j] = XMQuaternionSlerp(vecRot[j], XMLoadFloat4(&InterKeyFrame.qtRot), fWeightRatio);
			vecScale[j] = XMVectorLerp(vecScale[j], XMLoadFloat3(&InterKeyFrame.vScale), fWeightRatio);
		}
	}

	// ��� �� ��� ����
	for (_uint i = 0; i < iNumKeyFrames; i++)
	{
		XMStoreFloat3(&pKeyFrames[i].vPos, vecPos[i]);
		XMStoreFloat4(&pKeyFrames[i].qtRot, vecRot[i]);
		XMStoreFloat3(&pKeyFrames[i].vScale, vecScale[i]);
	}
}

void CBoneAnimGroup::Interpolated_KeyFrames(FKeyFrame* pKeyFrames, size_t iNumKeyFrames, FKeyFrameInterpolate* pArrInterpolate, size_t iNumInterpolates)
{
	if (iNumKeyFrames < 0)
		return;

	// ����ġ ����� �� ����ġ�� ���� ȥ�ո���̴�.
	// ������� �ܺο��� ���־�� �Ѵ�.
	// �ش� �Լ������� �ܺηκ��� ���� �����ͷκ��� �ִϸ��̼��� ȥ���� ���� �̾Ƴ��µ��� ���ȴ�.
	vector<_vector> vecPos, vecRot, vecScale;
	vecPos.resize(iNumKeyFrames, {});
	vecRot.resize(iNumKeyFrames, XMQuaternionIdentity());
	vecScale.resize(iNumKeyFrames, XMVectorSet(1.f, 1.f, 1.f, 0.f));

	// ����ġ�� �ܺο��� ���� 1�̵ȴٴ� �����Ͽ� �����Ѵ�.
	// �ִϸ��̼ǰ� ����, �ִϸ��̼� ����ŷ ��� ���� 1�� ���·� �����Ѵ�.
	_float fWeight = 0.f, fWeightRatio;
	for (_uint i = 0; i < iNumInterpolates; i++)
	{
		FKeyFrameInterpolate* pData = &pArrInterpolate[i];

		fWeight += pData->fWeight;
		if (fWeight != 0.f)
			fWeightRatio = (pData->fWeight / fWeight);
		else
			fWeightRatio = 0.f;

		for (_uint j = 0; j < iNumKeyFrames; j++)
		{
			FKeyFrame& InterKeyFrame = pData->KeyFrames[j];

			vecPos[j] = XMVectorLerp(vecPos[j], XMLoadFloat3(&InterKeyFrame.vPos), fWeightRatio);
			vecRot[j] = XMQuaternionSlerp(vecRot[j], XMLoadFloat4(&InterKeyFrame.qtRot), fWeightRatio);
			vecScale[j] = XMVectorLerp(vecScale[j], XMLoadFloat3(&InterKeyFrame.vScale), fWeightRatio);
		}
	}

	// ��� �� ��� ����
	for (_uint i = 0; i < iNumKeyFrames; i++)
	{
		XMStoreFloat3(&pKeyFrames[i].vPos, vecPos[i]);
		XMStoreFloat4(&pKeyFrames[i].qtRot, vecRot[i]);
		XMStoreFloat3(&pKeyFrames[i].vScale, vecScale[i]);
	}
}


