// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcDungeon/Public/Characters/PD_CharacterMovementComponent.h"
#include "ProcDungeon/Public/Characters/PD_BaseCharacter.h"
#include "ProcDungeon/Public/GAS/PD_AbilitySystemComponent.h"

//---------------------------------------------------------------------------------------------------------------------
float UPD_CharacterMovementComponent::GetMaxSpeed() const
{
	APD_BaseCharacter* Owner = Cast<APD_BaseCharacter>(GetOwner());

	if (!Owner)
	{
		return Super::GetMaxSpeed();
	}

	if (!Owner->IsCharacterAlive())
	{
		return 0.f;
	}

	if (Owner->GetAbilitySystemComponent()->HasMatchingGameplayTag(
		FGameplayTag::RequestGameplayTag(FName("PD.Status.Stun"))
	))
	{
		return 0.f;
	}

	return Owner->GetMoveSpeed();
}
