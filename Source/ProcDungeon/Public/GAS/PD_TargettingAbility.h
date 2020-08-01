// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/PD_GameplayAbility.h"
#include "Containers/Queue.h"
#include "PD_TargettingAbility.generated.h"

class APD_BaseCharacter;
class UPD_GameplayEffect;

/**
 * 
 */
UCLASS()
class PROCDUNGEON_API UPD_TargettingAbility : public UPD_GameplayAbility
{
	GENERATED_BODY()

	UPD_TargettingAbility();
	
public:
	/** Gameplay tag corresponding to the gameplay effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Target")
	FGameplayTag TargettingGameplayTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Target")
	bool bCanAutoTarget;
	
protected:
	/** Character References that are currently being targets */
	TQueue<APD_BaseCharacter*> TargetCharacters;

	/** Maximum amount of targets to take into account */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Target")
	int MaxTargetCount;

	/** Current number of targeted characters */
	int CurrentTargetCount;

	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate);

	/** Target new character and add it to the queue */
	UFUNCTION(BlueprintCallable, Category = "Target")
	bool EnqueCharacter(APD_BaseCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category = "Target")
	void ApplyEffectToQueue();

	UFUNCTION(BlueprintImplementableEvent, Category = "Target")
	void BP_ApplyTargetEffectOnCharacter(APD_BaseCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category = "Target")
	void ClearQueueAndEffects();
};
