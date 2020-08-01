// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcDungeon/Public/GAS/PD_GameplayAbility.h"

//---------------------------------------------------------------------------------------------------------------------
UPD_GameplayAbility::UPD_GameplayAbility()
{
	// Init Properties
	AbilityInputID = EPD_AbilityInputID::None;
	AbilityID = EPD_AbilityInputID::None;

	// Default to Instance Per Actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Default tags that block this ability from activating
	// ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Status.Dead")));
	// ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Status.Debuff.Stun")));
}
