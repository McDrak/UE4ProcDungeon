// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PD_AbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROCDUNGEON_API UPD_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	bool StartupAbilitiesGiven = false;
	bool StartupEffectsApplied = false;
};
