#include "Physics/PhysicsWorld3D.h"

#include "Physics/Collide.h"
#include "Physics/Contact.h"

CPhysicsWorld3D::CPhysicsWorld3D()
{
}

CPhysicsWorld3D::~CPhysicsWorld3D()
{
	Free();
}

CPhysicsWorld3D* CPhysicsWorld3D::Create(_uint iMaxContacts, _uint iIterations)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Ready_Physics(iMaxContacts, iIterations)))
	{
		MSG_BOX("PhysicsWorld3D Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPhysicsWorld3D::Free()
{
}

HRESULT CPhysicsWorld3D::Ready_Physics(_uint iMaxContacts, _uint iIterations)
{
	m_ContactResolver.Set_Iterations(iMaxContacts, iIterations);

	return S_OK;
}

void CPhysicsWorld3D::StartFrame_Physics()
{
	if (m_bIsPaused)
		return;

	// �� ����, �Ϸ�
	/*for (auto iter = m_listBody.begin(); iter != m_listBody.end(); ++iter)
	{
		(*iter)->Clear_Accumulators();
		(*iter)->CalculateDerivedData();
	}*/
}

_int CPhysicsWorld3D::Update_Physics(const _float& fTimeDelta)
{
	if (m_bIsPaused)
		return 0;

	// �� ���ϱ�, ��ü ��ġ ������� �浹ü ��ġ ����
	for (auto iter = m_listBody.begin(); iter != m_listBody.end(); ++iter)
	{
		(*iter)->Integrate(fTimeDelta);

		FCollisionPrimitive* pCol = static_cast<FCollisionPrimitive*>((*iter)->Get_Owner());
		pCol->Calculate_Transform();
		pCol->Set_Position(pCol->Get_PositionFloat3());
		pCol->Calculate_Shape();
	}

	// ���� �߻���
	_uint iUsedContacts = Generate_Contacts();


	//// �� �߻���� ���� �߻���� ���� ����� �浹�� �ذ�
	//if (m_bCalculateIterations)
	//	m_ContactResolver.Set_Iterations(iUsedContacts * 4);
	//m_ContactResolver.ResolveContacts(m_pContacts, iUsedContacts, fTimeDelta);

	return 0;
}

_uint CPhysicsWorld3D::Generate_Contacts()
{
	_uint iLimit = m_iMaxContacts;
	FContact* pNextContact = m_pContacts;

	// �浹�߻�
	//for (auto iter = m_ConGenList.begin(); iter != m_ConGenList.end(); ++iter)
	//{
	//	_uint iUsed = (*iter)->Add_Contact(pNextContact, iLimit);
	//	iLimit -= iUsed;
	//	pNextContact += iUsed;

	//	// ����Ʈ�� �ʰ��ϸ� �� �̻� ����ó���� ���� �ʴ´�.
	//	if (iLimit <= 0) break;
	//}

	// D-SAP
	list<pair<FRigidBody*, FRigidBody*>> listPairCollide;	// �浹 ��� ���

	// �Ÿ� �����, Square �Ÿ��� ����Ѵ�.
	// O(n)
	for (auto iter = m_listBroadBody.begin(); iter != m_listBroadBody.end(); iter++)
	{
		// �����̴� ��ü�� ���ؼ��� ��ġ�� ������Ʈ �Ѵ�.
		if ((*iter).pBody->Get_BodyType() == ERIGID_BODY_TYPE::STATIC && (*iter).bIsInit)
			continue;

		FDistPoint& DistPoint = (*iter);

		_float3 vPos = DistPoint.pBody->Get_Position();
		_vector vSimPos = XMLoadFloat3(&vPos);
		DistPoint.fDist = XMVectorGetX(XMVector3Length(vSimPos));

		FCollisionPrimitive* pShape = Cast<FCollisionPrimitive*>((*iter).pBody->Get_Owner());
		_float fHalfScale = XMVectorGetX(XMVector3Length(pShape->Get_ScaleVector() * 0.5f));
		DistPoint.fStart = DistPoint.fDist - fHalfScale;
		DistPoint.fEnd = DistPoint.fDist + fHalfScale;

		(*iter).bIsInit = true;
	}
	// ���� ��ġ�� ����� ����
	m_listBroadBody.sort();

	for (auto iter = m_listBroadBody.begin(); iter != m_listBroadBody.end(); ++iter)
	{
		FDistPoint& SrcData = (*iter);

		for (auto iterDst = (++iter)--; iterDst != m_listBroadBody.end(); ++iterDst)
		{
			FDistPoint& DstData = (*iterDst);

			// ��ŸƮ ����Ʈ�� �ִٸ� �浹 ���� �����Ѵ�.
			if (SrcData.fStart < DstData.fEnd && DstData.fStart <= SrcData.fEnd)
			{
				// ���̾� ����ũ�� ��ĥ �� ���� �����.
				FCollisionPrimitive* pColSrc = Cast<FCollisionPrimitive*>(SrcData.pBody->Get_Owner());
				FCollisionPrimitive* pColDst = Cast<FCollisionPrimitive*>(DstData.pBody->Get_Owner());

				// ����ũ�� �浹�Ǵ��� Ȯ��
				if (pColSrc->Get_CollisionMask() & pColDst->Get_CollisionLayer()
					|| pColDst->Get_CollisionMask() & pColSrc->Get_CollisionLayer())
				{
					// �ٿ�� �ڽ��� �浹�Ǵ��� Ȯ��
					if (pColSrc->BoundingBox.vMax.x >= pColDst->BoundingBox.vMin.x
						&& pColSrc->BoundingBox.vMax.y >= pColDst->BoundingBox.vMin.y
						&& pColSrc->BoundingBox.vMax.z >= pColDst->BoundingBox.vMin.z
						&& pColDst->BoundingBox.vMax.x >= pColSrc->BoundingBox.vMin.x
						&& pColDst->BoundingBox.vMax.y >= pColSrc->BoundingBox.vMin.y
						&& pColDst->BoundingBox.vMax.z >= pColSrc->BoundingBox.vMin.z)
					{
						listPairCollide.push_back({ SrcData.pBody, DstData.pBody });
					}
				}
			}
			else
				break;
		}
	}


	// �浹 ����ȭ, ���� �߰� ����
	// ��ȹ�� �浹��ü�� Ʈ���� ����� �ε��� �� ���� ��ü�� ���� ó���ϴ� ��.
	// ���⼭ �߻���Ų �浹�� ���� ���� �������� �߻��ϴ� 
	_int iDebugCount = 0;
	_int iDebug_BodySrc = 0;
	for (auto iter = listPairCollide.begin(); iter != listPairCollide.end(); ++iter)
	{
		bool bCollide = false;
		if (m_bIsPaused)
			return 0;

		FCollisionPrimitive* pColSrc = static_cast<FCollisionPrimitive*>((*iter).first->Get_Owner());
		FCollisionPrimitive* pColDst = static_cast<FCollisionPrimitive*>((*iter).second->Get_Owner());

		FCollisionData tColData;
		tColData.iContactsLeft = 1;	// �۵� ��ų��� �־����.

		// �ϳ��� �浹�� üũ�� �ϴ� ��쿡�� ����Ѵ�.
		if (pColSrc->Get_CollisionMask() & pColDst->Get_CollisionLayer()
			|| pColDst->Get_CollisionMask() & pColSrc->Get_CollisionLayer())
			bCollide = FCollisionDetector::CollsionPrimitive(pColSrc, pColDst, &tColData);
		else
			bCollide = false;

		if (bCollide)
		{
			if (pColSrc->Get_CollisionMask() & pColDst->Get_CollisionLayer())
				pColSrc->Handle_CollsionEvent(pColDst->Get_Owner(), &tColData.tContacts);
			tColData.tContacts.Reverse_BodyData();
			if (pColDst->Get_CollisionMask() & pColSrc->Get_CollisionLayer())
				pColDst->Handle_CollsionEvent(pColSrc->Get_Owner(), &tColData.tContacts);
		}
		++iDebugCount;
	}

	//FPotentialContact pContacts;
	//_uint iNumContacts = m_pBVHRootNode->Get_PotentialContacts(&pContacts, 300);

	//for (auto iter = pContacts.listBodies.begin(); iter != pContacts.listBodies.end(); ++iter)
	//{
	//	bool bCollide = false;
	//	if (m_bIsPaused)
	//		return 0;

	//	FCollisionPrimitive* pColSrc = Cast<FCollisionPrimitive*>((*iter).first->Get_Owner());
	//	FCollisionPrimitive* pColDst = Cast<FCollisionPrimitive*>((*iter).second->Get_Owner());

	//	FCollisionData tColData;
	//	tColData.iContactsLeft = 1;	// �۵� ��ų��� �־����.

	//	// �ϳ��� �浹�� üũ�� �ϴ� ��쿡�� ����Ѵ�.
	//	if (pColSrc->Get_CollisionMask() & pColDst->Get_CollisionLayer()
	//		|| pColDst->Get_CollisionMask() & pColSrc->Get_CollisionLayer())
	//	{
	//		/*if (pColSrc->BoundingBox.Overlaps(&pColDst->BoundingBox))
	//		{*/
	//			bCollide = FCollisionDetector::CollsionPrimitive(pColSrc, pColDst, &tColData);
	//			++iDebugCount;
	//		//}
	//	}
	//	else
	//		bCollide = false;

	//	if (bCollide)
	//	{
	//		if (pColSrc->Get_CollisionMask() & pColDst->Get_CollisionLayer())
	//			pColSrc->Handle_CollsionEvent(pColDst->Get_Owner(), &tColData.tContacts);
	//		tColData.tContacts.Reverse_BodyData();
	//		if (pColDst->Get_CollisionMask() & pColSrc->Get_CollisionLayer())
	//			pColDst->Handle_CollsionEvent(pColSrc->Get_Owner(), &tColData.tContacts);
	//	}
	//}

	/*wstringstream ss;
	wstring str;
	ss << iDebugCount;
	str = L"Physics CheckCount : " + ss.str() + L"\n";
	OutputDebugString(str.c_str());*/
	//cout << "�˻� ī��Ʈ : " << iDebugCount << endl;
	/*cout << "Src ī��Ʈ : " << iDebug_BodySrc << endl;
	cout << "Dst ī��Ʈ : " << ((iDebug_BodySrc) ? (iDebugCount / iDebug_BodySrc) : 0) << endl;*/
	//cout << "Body ī��Ʈ : " << m_listBody.size() << endl;

	// ���� ���� ���� ��ȯ
	return m_iMaxContacts - iLimit;
}

FCollisionPrimitive* CPhysicsWorld3D::Test_Contact(FCollisionPrimitive* const pCollision)
{
	// ���� �浹ü�� ����Ʈ�ٵ��� �浹ü��� ���ؼ� �ϳ��� �浹�ϸ� ��ȯ�Ѵ�.
	for (auto iter = m_listBody.begin(); iter != m_listBody.end(); ++iter)
	{
		FCollisionPrimitive* pCol = static_cast<FCollisionPrimitive*>((*iter)->Get_Owner());
		if (FCollisionDetector::CollsionPrimitive(pCollision, pCol, nullptr))
			return pCol;
	}

	return nullptr;
}

list_collide_test CPhysicsWorld3D::Test_Contacts(FCollisionPrimitive* const pCollision)
{
	list_collide_test listCollision;

	_vector vSimPos = pCollision->Get_PositionVector();
	_float fDist = XMVectorGetX(XMVector3Length(vSimPos));

	_vector vMin = XMLoadFloat3(&pCollision->BoundingBox.vMin);
	_vector vMax = XMLoadFloat3(&pCollision->BoundingBox.vMax);

	_float fHalfScale = XMVectorGetX(XMVector3Length((vMax - vMin) * 0.5f));
	_float fStart = fDist - fHalfScale;
	_float fEnd = fDist + fHalfScale;
	
	auto iterFind = lower_bound(m_listBroadBody.begin(), m_listBroadBody.end(), fStart,
		[](const FDistPoint& DistPoint, _float _fStart) {
			return (DistPoint.fStart < _fStart);
		});
	if (iterFind == m_listBroadBody.end())
		return listCollision;

	// ���� �浹ü�� ����Ʈ�ٵ��� �浹ü��� ���ؼ� �ϳ��� �浹�ϸ� ��ȯ�Ѵ�.
	for (auto iter = iterFind; iter != m_listBroadBody.end(); ++iter)
	{
		FDistPoint& DistPoint = (*iter);

		if (fStart <= DistPoint.fEnd && DistPoint.fStart <= fEnd)
		{
			FCollisionPrimitive* pColDst = Cast<FCollisionPrimitive*>(DistPoint.pBody->Get_Owner());

			_bool bCollide = false;
			FCollisionData tColData;
			tColData.iContactsLeft = 1;	// �۵� ��ų��� �־����.

			if (pCollision->Get_CollisionMask() & pColDst->Get_CollisionLayer())
			{
				if (pCollision->BoundingBox.vMax.x >= pColDst->BoundingBox.vMin.x
					&& pCollision->BoundingBox.vMax.y >= pColDst->BoundingBox.vMin.y
					&& pCollision->BoundingBox.vMax.z >= pColDst->BoundingBox.vMin.z
					&& pColDst->BoundingBox.vMax.x >= pCollision->BoundingBox.vMin.x
					&& pColDst->BoundingBox.vMax.y >= pCollision->BoundingBox.vMin.y
					&& pColDst->BoundingBox.vMax.z >= pCollision->BoundingBox.vMin.z)
					bCollide = FCollisionDetector::CollsionPrimitive(pCollision, pColDst, &tColData);
			}
			else
				continue;

			if (bCollide)
			{
				listCollision.push_back(pair<FCollisionPrimitive*, FContact>(pColDst, tColData.tContacts));
			}
		}
		else
			break;
	}

	return listCollision;
}

void CPhysicsWorld3D::Add_RigidBody(FRigidBody* pBody)
{
	auto iter = find_if(m_listBody.begin(), m_listBody.end(),
		[&pBody](FRigidBody*& pDstBody) {
			return pDstBody == pBody;
		});
	if (iter != m_listBody.end())
		return;
	
	m_listBody.push_back(pBody);
	m_listBroadBody.push_back({ pBody, -1.f, -1.f, -1.f });
	/*FCollisionPrimitive* pCol = ReCast<FCollisionPrimitive*>(pBody->Get_Owner());
	pCol->Calculate_Transform();
	pCol->Calculate_Shape();
	m_pBVHRootNode->Insert(pBody, pCol->BoundingBox);*/
}

void CPhysicsWorld3D::Delete_RigidBody(FRigidBody* pBody)
{
	if (m_listBody.empty())
		return;

	auto iter = find_if(m_listBody.begin(), m_listBody.end(),
		[&pBody](FRigidBody*& pDstBody) {
			return pDstBody == pBody;
		});
	if (iter != m_listBody.end())
		m_listBody.erase(iter);

	auto iterBroad = find_if(m_listBroadBody.begin(), m_listBroadBody.end(),
		[&pBody](FDistPoint& pDstBody) {
			return pDstBody.pBody == pBody;
		});
	if (iterBroad != m_listBroadBody.end())
		m_listBroadBody.erase(iterBroad);
}
