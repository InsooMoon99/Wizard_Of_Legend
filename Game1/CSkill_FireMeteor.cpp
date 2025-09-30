#include "stdafx.h"
#include "CSkill_FireMeteor.h"
#include "CFire_Ball.h"
#include "CCharacter.h"
#include "CFire_Meteor.h"
#include "CFire_MiddleMeteor.h"
#include "CPlayer.h"
#include "CStateComponent.h"

CSkill_FireMeteor::CSkill_FireMeteor()
{

}

CSkill_FireMeteor::~CSkill_FireMeteor()
{

}

void CSkill_FireMeteor::BeginPlay()
{
	CSkill::BeginPlay();

	m_Id = "Meteor";

	if (auto data = SKILL_DATA->GetById(m_Id))
	{
		m_DataDamage = data->Damage;
		m_DataCoolDown = data->CoolDown - 0.1f;
	}
	else
	{
		m_DataDamage = 10.f;
		m_DataCoolDown = 5.f - 0.1f;
	}

}

void CSkill_FireMeteor::Tick()
{
	CSkill::Tick();

	for (auto iter = m_FireMeteor.begin(); iter != m_FireMeteor.end(); )
	{
		auto meteor = *iter;

		meteor->Tick();

		if (meteor->IsDestroy())
		{
			m_CastingCircle->visible = false;
			iter = m_FireMeteor.erase(iter);
		}

		else
			++iter;
	}

	for (auto iter = m_FireMiddleMeteor.begin(); iter != m_FireMiddleMeteor.end(); )
	{
		auto meteor = *iter;

		meteor->Tick();

		if (meteor->IsDestroy())
			iter = m_FireMiddleMeteor.erase(iter);

		else
			++iter;
	}

	if (m_CastingCircle)
	{
		// 캐스팅 애니 끝나갈 때 소환 시작
		if (m_CastingCircle->frame.x >= m_CastingCircle->maxFrame.x - 20 && !bMiddleCheck)
		{
			bMiddleCheck = true;
			bMiddleSpawning = true;    
			m_MiddleCount = 0;
			m_SpawnTimer = 0.f;
			m_NextSpawnTime = CHelper::RandF(0.05f, 0.1f);
		}

		// 소환중일 때 랜덤 타이밍 체크
		if (bMiddleSpawning && m_MiddleCount < m_MiddleMax)
		{
			m_SpawnTimer += DELTA;

			if (m_SpawnTimer >= m_NextSpawnTime)
			{
				m_SpawnTimer = 0.f;
				m_NextSpawnTime = CHelper::RandF(0.05f, 0.1f); // 다음 발까지 대기

				// ★ MiddleMeteor 하나 생성
				SpawnOneMiddleMeteor(m_MiddleCount);

				m_MiddleCount++;
				if (m_MiddleCount >= m_MiddleMax)
					bMiddleSpawning = false; // 다 만들었으면 종료
			}
		}

		// 메인 메테오(중앙)는 따로, 기존대로
		if (m_CastingCircle->frame.x >= m_CastingCircle->maxFrame.x - 9 && !bMeteorCheck)
		{
			bMeteorCheck = true;
			auto meteor = make_shared<CFire_Meteor>(m_Skill_Info);
			meteor->BeginPlay();
			m_FireMeteor.push_back(meteor);
		}

		m_CastingCircle->Update();
	}

}

void CSkill_FireMeteor::LateUpdate()
{
	CSkill::LateUpdate();

}

void CSkill_FireMeteor::Release()
{
	CSkill::Release();

}

void CSkill_FireMeteor::Render()
{
	CSkill::Render();

	for (auto& meteor : m_FireMeteor) 
	{
		if (!meteor->IsDestroy())
			meteor->Render();
	}

	for (auto& meteor : m_FireMiddleMeteor)
	{
		if (!meteor->IsDestroy())
			meteor->Render();
	}

	if (m_CastingCircle)
		m_CastingCircle->Render();

}

bool CSkill_FireMeteor::CanActivate(const FSkillInfo& _Info)
{
	//if (!CSkill::CanActivate(_Info))
	//	return false;
	//
	//// 부모쪽 CSkill에서 CoolRemain으로 처리
	//
	//return true;

	if (!CSkill::CanActivate(_Info))

		// TODO : 나중에 스킬 쿨타임에 따라서 판단

		return true;
}

void CSkill_FireMeteor::Activate(CActor* _Owner)
{
	if (!CanActivate(m_Skill_Info)) return;
	CSkill::Activate(_Owner);

	CPlayer* player = dynamic_cast<CPlayer*>(_Owner);
	if (!player) return;

	auto state = CHelper::GetComponent<CStateComponent>(player);
	if (!state) return;

	bMiddleCheck = false;
	bMeteorCheck = false;

	state->Change_State(ECharacterState::Attack3);

	Vec2 ownerDir = CHelper::DirToVec(state->Get_Dir());
	Vec2 spawnPos = Vec2(_Owner->Get_WorldPos().x, _Owner->Get_WorldPos().y + 350.f);

	m_Skill_Info.Instigator = _Owner;
	m_Skill_Info.SpawnPos = spawnPos;
	m_Skill_Info.Damage = m_DataDamage;
	m_Skill_Info.Amount = 500.f; 

	StartCoolDown();

	bMeteorCheck = false;

	m_PointPos = Vec2(_Owner->Get_WorldPos().x, _Owner->Get_WorldPos().y) + ownerDir * 200.f;

	m_CastingCircle = RESOURCE->CreateSprite(L"Particle\\CastingCircle.png", Vec2(3780.f / 27.f, 140.f) * 2.5f, Int2(27, 1), true);
	m_CastingCircle->ChangeAnim(ANIMSTATE::ONCE, 0.05f);
	m_CastingCircle->color.A(0.3f);
	m_CastingCircle->SetWorldPos(m_PointPos);

	Vec2 toTarget = m_PointPos - spawnPos;
	float distance = toTarget.Length();

	if (distance < 1e-6f)
		toTarget = Vec2(0.f, -1.f); 
	else
		toTarget.Normalize();

	m_Skill_Info.Aim_Dir = toTarget;
	m_Skill_Info.Angle = Utility::DirToRadian(-toTarget);

	m_Skill_Info.TravelMax = distance;
}

void CSkill_FireMeteor::SpawnOneMiddleMeteor(int index)
{
	const Vec2 center = m_PointPos;
	const float radius = 100.f;
	const float H = 200.f;
	const float angleStep = (2.f * PI) / m_MiddleMax;

	// 메인 메테오 진행 방향(이미 Activate에서 세팅됨)
	Vec2 mainDir = m_Skill_Info.Aim_Dir;
	if (mainDir.LengthSquared() < 1e-6f) mainDir = Vec2(0.f, -1.f); // 안전장치
	else                                  mainDir.Normalize();

	// 5각형 꼭짓점 각도
	Vec2 ownerDir = CHelper::DirToVec(
		CHelper::GetComponent<CStateComponent>(dynamic_cast<CPlayer*>(m_Skill_Info.Instigator))->Get_Dir());
	float baseAngle = atan2f(ownerDir.y, ownerDir.x);

	float ang = baseAngle + index * angleStep;
	Vec2  offset = Vec2(cosf(ang), sinf(ang)) * radius;
	Vec2  targetPos = center + offset;

	// ★ 세계 좌표 고정 (H,H) 대신, '메인 진행 방향 기준으로' 뒤쪽에서 스폰
	Vec2  spawnPos = targetPos - mainDir * H;

	// 직선 이동(사실상 mainDir와 동일)
	Vec2  toTarget = targetPos - spawnPos;
	float dist = toTarget.Length();
	if (dist > 1e-6f) toTarget.Normalize(); else toTarget = mainDir;

	FSkillInfo info = m_Skill_Info;
	info.SpawnPos = spawnPos;
	info.Aim_Dir = toTarget;                      // 모든 방향에서 일관
	info.Angle = Utility::DirToRadian(-toTarget);
	info.TravelMax = dist;

	auto meteor = make_shared<CFire_MiddleMeteor>(info);
	meteor->BeginPlay();
	m_FireMiddleMeteor.push_back(meteor);
}


