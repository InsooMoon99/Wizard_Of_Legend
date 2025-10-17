#pragma once

#include "CActor.h"

class CActor;

static float DistanceToTarget(CActor* _Owner, CActor* _Target)
{
	Vec2 ownerPos = _Owner->Get_Collision()->GetWorldPos();
	Vec2 targetPos = _Target->Get_Collision()->GetWorldPos();

	Vector2 dis = targetPos - ownerPos;
	float fLength = dis.Length();

	return fLength;
}