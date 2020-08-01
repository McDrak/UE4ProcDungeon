// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcDungeon/Public/GAS/PD_TargettingAbility.h"
#include "ProcDungeon/Public/GAS/PD_AbilitySystemComponent.h"
#include "ProcDungeon/Public/Characters/ProcDungeonCharacter.h"
#include "ProcDungeon/Public/Characters/PD_BaseCharacter.h"

//---------------------------------------------------------------------------------------------------------------------
UPD_TargettingAbility::UPD_TargettingAbility()
{
	CurrentTargetCount = 0;
	bCanAutoTarget = true;
}

//---------------------------------------------------------------------------------------------------------------------
void UPD_TargettingAbility::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate);

	// Empty the queue from previous calls
	TargetCharacters.Empty();
	CurrentTargetCount = 0;

	// Add the ability owner to the queue
	if(bCanAutoTarget)
	{
		APD_BaseCharacter* AvatarCharacter = Cast<APD_BaseCharacter>(GetActorInfo().OwnerActor);
		TargetCharacters.Enqueue(AvatarCharacter);
		CurrentTargetCount++;
	}
}

//---------------------------------------------------------------------------------------------------------------------
bool UPD_TargettingAbility::EnqueCharacter(APD_BaseCharacter* TargetCharacter)
{
	bool EnqueFlag = false;

	if (TargettingGameplayTag.IsValid() && IsValid(TargetCharacter))
	{
		FGameplayTagContainer TargetEffectsContainer;
		TargetEffectsContainer.AddTag(TargettingGameplayTag);
		if (!TargetCharacter->GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(TargetEffectsContainer))
		{
			if (CurrentTargetCount >= MaxTargetCount)
			{
				APD_BaseCharacter* HeadCharacter;
				TargetCharacters.Dequeue(HeadCharacter);
				HeadCharacter->GetAbilitySystemComponent()->RemoveActiveEffectsWithTags(TargetEffectsContainer);
				HeadCharacter->BP_TargetTrigger(AbilityID, false);
			}

			TargetCharacters.Enqueue(TargetCharacter);
			CurrentTargetCount++;

			EnqueFlag = true;
		}
	}

	return EnqueFlag;
}

//---------------------------------------------------------------------------------------------------------------------
void UPD_TargettingAbility::ApplyEffectToQueue()
{
	FGameplayTagContainer TargetEffectsContainer;
	TargetEffectsContainer.AddTag(TargettingGameplayTag);
	while (!TargetCharacters.IsEmpty())
	{
		APD_BaseCharacter* HeadCharacter;
		TargetCharacters.Dequeue(HeadCharacter);

		if (
			IsValid(HeadCharacter) &&
			HeadCharacter->GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(TargetEffectsContainer)
		)
		{
			BP_ApplyTargetEffectOnCharacter(HeadCharacter);
			HeadCharacter->GetAbilitySystemComponent()->RemoveActiveEffectsWithTags(TargetEffectsContainer);
			HeadCharacter->BP_TargetTrigger(AbilityID, false);
		}
	}
}

//----------------------------------------------------------------------------------.-----------------------------------
void UPD_TargettingAbility::ClearQueueAndEffects()
{
	FGameplayTagContainer TargetEffectsContainer;
	TargetEffectsContainer.AddTag(TargettingGameplayTag);
	while (!TargetCharacters.IsEmpty())
	{
		APD_BaseCharacter* HeadCharacter;
		TargetCharacters.Dequeue(HeadCharacter);

		
		if (HeadCharacter)
		{
			HeadCharacter->GetAbilitySystemComponent()->RemoveActiveEffectsWithTags(TargetEffectsContainer);
			HeadCharacter->BP_TargetTrigger(AbilityID, false);
		}
	}
}
