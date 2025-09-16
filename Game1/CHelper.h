#pragma once
#include "CActor.h"
#include "CCharacter.h"
#include "CComponent.h"

class CHelper
{
public:
	static const char* ToString_PlayerState(EPlayerState _State)
	{
		switch (_State)
		{
		case EPlayerState::Idle:		return "State_Idle";
		case EPlayerState::Walk:		return "State_Walk";
		case EPlayerState::Dash:		return "State_Dash";
		case EPlayerState::Attack1:		return "State_Attack1";
		case EPlayerState::Attack2:		return "State_Attack2";

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
