#pragma once
#include "CActor.h"
#include "CCharacter.h"
#include "CComponent.h"
#include "CFloatingText.h"
#include "CMonsterBase.h"
#include "CPlayer.h"
#include "CStatusComponent.h"

static mt19937 g_RandomEngine(static_cast<unsigned int>(time(nullptr)));

class CHelper
{
public:
	static const char* ToString_PlayerState(ECharacterState _State)
	{
		switch (_State)
		{
		case ECharacterState::Idle:					return "State_Idle";
		case ECharacterState::Walk:					return "State_Walk";
		case ECharacterState::Dash:					return "State_Dash";
		case ECharacterState::Attack1:				return "State_Attack1";
		case ECharacterState::Attack2:				return "State_Attack2";
		case ECharacterState::Attack3:				return "State_Attack3";
		case ECharacterState::Hit:					return "Hit";

		case ECharacterState::Boss_Start:			return "Boss_Start";
		case ECharacterState::Boss_Start2:			return "Boss_Start2";
		case ECharacterState::Boss_Ready:			return "Boss_Ready";
		case ECharacterState::Boss_Attack1_Ready:	return "Boss_Attack1_Ready";
		case ECharacterState::Boss_Attack1:			return "Boss_Attack1";
		case ECharacterState::Boss_Attack2:			return "Boss_Attack2";
		case ECharacterState::Boss_Attack3:			return "Boss_Attack3";
		case ECharacterState::Air:					return "Air";

		default:									return "None";
		}
	}

	static const char* ToString_PlayerDir(EPlayerDir _Dir)
	{
		switch (_Dir)
		{
		case EPlayerDir::Front:			return "Front";
		case EPlayerDir::Back:			return "Back";
		case EPlayerDir::Left:			return "Left";
		case EPlayerDir::Right:			return "Right";
		case EPlayerDir::Front_Left:	return "Front_Left";
		case EPlayerDir::Front_Right:	return "Front_Right";
		case EPlayerDir::Back_Left:		return "Back_Left";
		case EPlayerDir::Back_Right:	return "Back_Right";
			//case EPlayerDir::Stop:	return "Stop";

		case EPlayerDir::PD_END:		return "PD_End";
		default: return "None";
		}
	}

	static const char* SkillIdFromType(ESkillType type)
	{
		switch (type)
		{
		case ESkillType::Fire_Ball:				return "FireBall";
		case ESkillType::Fire_Dragon:			return "FireDragon";
		case ESkillType::Fire_Meteor:           return "Meteor";
		case ESkillType::Wind_Slash:			return "WindSlash";
		case ESkillType::Wind_Dash:				return "WindDash";
		case ESkillType::Earth_Shield:          return "EarthShield";

		default:                                return "None";
		}
	}

	static Vec2 DirToVec(EPlayerDir _dir)
	{
		switch (_dir)
		{
		case EPlayerDir::Front:
			return Vec2(0.0f, -1.0f);
		case EPlayerDir::Back:
			return Vec2(0.0f, 1.0f);
		case EPlayerDir::Left:
			return Vec2(-1.0f, 0.0f);
		case EPlayerDir::Right:
			return Vec2(1.0f, 0.0f);

		case EPlayerDir::Front_Left:
		{
			{
				Vector2 frontLeft = Vec2(-1.0f, -1.0f);
				frontLeft.Normalize();
				return frontLeft;
			}
		}

		case EPlayerDir::Front_Right:
		{
			{
				Vector2 frontRight = Vec2(1.0f, -1.0f);
				frontRight.Normalize();
				return frontRight;
			}
		}
		case EPlayerDir::Back_Left:
		{
			{
				Vector2 backLeft = Vector2(-1.0f, 1.0f);
				backLeft.Normalize();
				return backLeft;
			}
		}

		case EPlayerDir::Back_Right:
		{
			{
				Vector2 backRight = Vec2(1.0f, 1.0f);
				backRight.Normalize();
				return backRight;
			}
		}
		default:
			return Vec2(1.0f, 0.0f); // 기본값 오른쪽으로
		}
	}

	static void HitColor(ObImage* _Img)
	{
		if (_Img == nullptr) return;

		Color hit_Color = Color(1.f, 1.f, 1.f, 0.5f);
		_Img->color = hit_Color;
	}

	static void OrignColor(ObImage* _Img)
	{
		if (_Img == nullptr) return;

		Color origin_Color = Color(0.5f, 0.5f, 0.5f, 0.5f);
		_Img->color = origin_Color;
	}

	static bool IsLastFrame(shared_ptr<ObImage> _Img)
	{
		return _Img->frame.x >= _Img->maxFrame.x - 1;
	}

	static bool IsSameTeam(CCharacter* _Src, CCharacter* _Dst)
	{
		if (!_Src || !_Dst)
			return false;

		return _Src->GetTeamID() == _Dst->GetTeamID();
	}

	static RECT DrawLTRect(const Vec2& leftTop, const Vec2& size)
	{
		return RECT{(LONG)leftTop.x,(LONG)leftTop.y,(LONG)(leftTop.x + size.x),(LONG)(leftTop.y + size.y)};
	}

	static RECT DrawCenterRect(const Vec2& center, const Vec2& size)
	{
		float halfW = size.x * 0.5f;
		float halfH = size.y * 0.5f;

		return RECT { (LONG)(center.x - halfW), (LONG)(center.y - halfH),(LONG)(center.x + halfW), (LONG)(center.y + halfH) };
	}

	static void OutlineText(const wstring& text, const RECT& rect, float fontSize, const wstring& fontName, const Color& textColor, const Color& outlineColor = Color(0, 0, 0, 1),int outlineThickness = 1)
	{
		for (int dx = -outlineThickness; dx <= outlineThickness; ++dx)
		{
			for (int dy = -outlineThickness; dy <= outlineThickness; ++dy)
			{
				if (dx == 0 && dy == 0) continue;

				RECT offsetRect = rect;
				offsetRect.left += dx;
				offsetRect.right += dx;
				offsetRect.top += dy;
				offsetRect.bottom += dy;

				DWRITE->RenderText(text, offsetRect, fontSize, fontName, outlineColor);
			}
		}

		DWRITE->RenderText(text, rect, fontSize, fontName, textColor);
	}

	static wstring ToW(const string& str)
	{
		return wstring(str.begin(), str.end());
	}

	static float AttackMoveFactorByFrame(const ObImage* img)
	{
		int f = img->frame.x;
		int last = max(img->maxFrame.x - 1, 1);
		float t = (float)f / (float)last; 

		if (t < 0.2f)  return 0.35f;

		if (t < 0.4f)  return 0.15f;

		if (t < 0.8f)  return 0.0f;

		return 0.20f;
	}

	static float RandF(float min, float max)
	{
		if (min > max)
		{
			swap(min, max);
		}

		uniform_real_distribution<float> dist(min, max);
		return dist(g_RandomEngine);
	}

	static void DrawDamageFont(int damage, Vec2 pos, Color color = Color(1.f, 1.f, 1.f, 1.f))
	{
		float jitterX = 18.f, jitterY = 10.f;

		float jx = RANDOM->Float(-jitterX, jitterX);
		float jy = RANDOM->Float(0.f, jitterY); 

		Vec2 start = pos + Vec2(jx, jy);

		FFloatingTextDesc text;
		text.text = to_wstring(damage);
		text.startPos = start;         
		text.duration = 0.9f;
		text.rise = 70.f;
		text.fontSize = 30.f;
		text.color = color;
		text.outline = true;

		OBJECT->SpawnActor(make_shared<CFloatingText>(text));
	}

	static float CaculateDamage(float attack, float defense, int randMin = 1, int randMax = 5)
	{
		int random = RANDOM->Int(randMin, randMax);
		return max(1.f, (attack - defense)) + random;
	}

	static void ApplyDamage(shared_ptr<CStatusComponent> status, float damage, Vec2 pos)
	{
		status->AddHp(-damage);

		CActor* owner = status->GetOwner();
		if (dynamic_cast<CPlayer*>(owner))
		{
			DrawDamageFont(damage, pos, Color(0.8f, 0.f, 0.f, 1.0f)); 
		}
		else
		{
			DrawDamageFont(damage, pos); 
		}
	}

	static RECT MakeScreenRectFromImage(ObImage* img, Vec2 offset = Vec2(0.f, 0.f))
	{
		Vec2 world = img->GetWorldPos();

		Vec2 screen = WorldToScreen(world); 

		Vec2 size = img->scale;         
		Vec2 half = size * 0.5f;

		RECT rc;
		rc.left = (LONG)floor(screen.x - half.x + offset.x);
		rc.top = (LONG)floor(screen.y - half.y + offset.y);
		rc.right = (LONG)ceil(screen.x + half.x + offset.x);
		rc.bottom = (LONG)ceil(screen.y + half.y + offset.y);
		return rc;
	}

	static Vec2 WorldToScreen(const Vec2& world)
	{
		Vec2 cam = CAM->GetCameraPos();
		Vec2 halfScreen(app.GetWidth() * 0.5f, app.GetHeight() * 0.5f);

		// X: 동일, Y: 부호 반전 (cam 기준으로 위가 +인 월드 → 아래가 +인 스크린)
		float sx = (world.x - cam.x) + halfScreen.x;
		float sy = (cam.y - world.y) + halfScreen.y; // ★ 여기!

		return Vec2(sx, sy);
	}

	template<typename T>
	static T* CreateComponent(CActor* _Owner)
	{
		if (_Owner == nullptr)
			return nullptr;

		CComponent* component = new T();
		//component->BeginPlay();
		component->SetOwner(_Owner);

		auto componentType = dynamic_cast<T*>(component);

		if (componentType)
			_Owner->AddComponent(componentType);

		return componentType;
	}

	template<typename T>
	static shared_ptr<T> CreateSharedComponent(CActor* _Owner)
	{
		if (_Owner == nullptr)
			return nullptr;

		auto component = make_shared<T>();
		//component->BeginPlay();
		component->SetOwner(_Owner);

		_Owner->AddComponent(component);

		return component;
	}

	template<typename T>
	static shared_ptr<T> GetComponent(CActor* _Owner)
	{
		if (_Owner == nullptr)
			return nullptr;

		const auto& components = _Owner->GetAllComponents();

		for (const auto& component : components)
		{
			shared_ptr<T> foundComponent = dynamic_pointer_cast<T>(component);
			if (foundComponent)
			{
				return foundComponent;
			}
		}

		return nullptr;
	}

	
};
