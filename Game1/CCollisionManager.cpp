#include "stdafx.h"
#include "CCollisionManager.h"

CCollisionManager* CCollisionManager::Instance = nullptr;

void CCollisionManager::Init()
{

}

void CCollisionManager::Update()
{
	vector<GameObject*>& colliders = m_Colliders;

	for (int i = 0; i < colliders.size(); i++)
	{
		for (int j = i + 1; j < colliders.size(); j++)
		{
			GameObject* src = colliders[i];
			GameObject* dst = colliders[j];

			if (src->Intersect(dst))
			{
				if (src->Get_CollisionMap().contains(dst) == false)
				{
					// 자기 자신 충돌 X
					if (src->Get_Owner() == dst->Get_Owner())continue;
					// 공격 객체 같으면 충돌 X
					if (src->Get_Instigator() == dst->Get_Instigator()) continue;

					if (src->Get_Instigator() && src->Get_Instigator() == dst->Get_Owner()) continue;
					if (dst->Get_Instigator() && dst->Get_Instigator() == src->Get_Owner()) continue;

					// 죽으면 무시
					if (src->Get_Owner()->IsDestroy()) continue;
					if (dst->Get_Owner()->IsDestroy()) continue;

					src->Get_Owner()->OnComponentBeginOverlap(src, dst);
					dst->Get_Owner()->OnComponentBeginOverlap(dst, src);
					src->Get_CollisionMap().insert(dst);
					dst->Get_CollisionMap().insert(src);
				}
			}

			else
			{
				if (src->Get_CollisionMap().contains(dst))
				{
					src->Get_Owner()->OnComponentEndOverlap(src, dst);
					dst->Get_Owner()->OnComponentEndOverlap(dst, src);
					src->Get_CollisionMap().erase(dst);
					dst->Get_CollisionMap().erase(src);
				}
			}
		}
	}

}

void CCollisionManager::AddCollider(GameObject* collider)
{
	m_Colliders.push_back(collider);

}

void CCollisionManager::RemoveCollider(GameObject* collider)
{
	auto iter = remove(m_Colliders.begin(), m_Colliders.end(), collider);
	m_Colliders.erase(iter, m_Colliders.end());
}

