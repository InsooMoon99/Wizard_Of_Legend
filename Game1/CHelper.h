#pragma once
#include "CActor.h"
#include "CCharacter.h"
#include "CComponent.h"

class CHelper
{
public:
	static const char* ToString_PlayerState(ECharacterState _State)
	{
		switch (_State)
		{
		case ECharacterState::Idle:		return "State_Idle";
		case ECharacterState::Walk:		return "State_Walk";
		case ECharacterState::Dash:		return "State_Dash";
		case ECharacterState::Attack1:		return "State_Attack1";
		case ECharacterState::Attack2:		return "State_Attack2";

		default:						return "None";
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

	template<typename T>
	static T* CreateComponent(CActor* _Owner)
	{
		if (_Owner == nullptr)
			return nullptr;

		CComponent* component = new T();
		component->BeginPlay();
		component->SetOwner(_Owner);

		auto componentType = dynamic_cast<T*>(component);

		if (componentType)
			_Owner->AddComponent(componentType);

		return componentType;
	}

	template<typename T>
	static T* GetComponent(CActor* _Owner)
	{
		if (_Owner == nullptr)
			return nullptr;

		const auto& components = _Owner->GetAllComponents();

		for (CComponent* component : components)
		{
			T* foundComponent = dynamic_cast<T*>(component);
			if (foundComponent)
			{
				return foundComponent;
			}
		}

		return nullptr;
	}

};
