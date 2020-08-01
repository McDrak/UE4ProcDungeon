// Fill out your copyright notice in the Description page of Project Settings.


#include "PD_EngineSubsystem.h"
#include "AbilitySystemGlobals.h"

//---------------------------------------------------------------------------------------------------------------------
void UPD_EngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UAbilitySystemGlobals::Get().InitGlobalData();
}
