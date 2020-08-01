// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "ProcDungeon/ProcDungeon.h"
#include "PD_BaseCharacter.generated.h"

class UPD_AbilitySystemComponent;
class UPD_AttributeSet;
class UPD_GameplayAbility;
class UPD_GameplayEffect;
class UAbilitySystemComponent;
class UAnimMontage;

UCLASS()
class PROCDUNGEON_API APD_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APD_BaseCharacter(const class FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "ProcDungeon")
	void BP_Die();

	//////////////////////////////////////////
	// GAS Setup

	/** Ability System Component Pointer */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Abilities")
	UPD_AbilitySystemComponent* AbilitySystemComponent;

	/** Attribute Set */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Abilities")
	UPD_AttributeSet* AttributeSet;

	/** Starting Abilities Array */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ProcDungeon|Abilities")
	TArray<TSubclassOf<UPD_GameplayAbility>> StartingAbilities;

	/** Starting Effects Array */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ProcDungeon|Abilities")
	TArray<TSubclassOf<UPD_GameplayEffect>> StartingEffects;

	void BindASCInput();

	/** Abilities Setup Function */
	void AddCharacterAbilities();

	/** Abilities Removal Function */
	void RemoveCharacterAbilities();

	/** Effects Setup Function */
	void AddStartupEffects();

	/** Effects Removal Function */
	void RemoveGameplayEffects();

	FGameplayTag EffectRemoveOnDeathTag;

	FGameplayTag DeadTag;

public:
	UFUNCTION(BlueprintCallable, Category ="Character")
	bool IsCharacterAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	float GetMoveSpeed() const;

	UFUNCTION()
	void OnHealthChanged(float Health, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "Abilities")
	void BP_OnHealthChanged(float Health, float MaxHealth);

	UFUNCTION()
	void OnManaChanged(float Mana, float MaxMana);

	UFUNCTION(BlueprintImplementableEvent, Category = "Abilities")
	void BP_OnManaChanged(float Mana, float MaxMana);

	UFUNCTION()
	void OnStaminaChanged(float Stamina, float MaxStamina);

	UFUNCTION(BlueprintImplementableEvent, Category = "Abilities")
	void BP_OnStaminaChanged(float Stamina, float MaxStamina);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void OnTargetTrigger(EPD_AbilityInputID AbilityID, bool Trigger);

	UFUNCTION(BlueprintImplementableEvent, Category = "Abilities")
	void BP_TargetTrigger(EPD_AbilityInputID AbilityID, bool Trigger);

	/** ASC Getter */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
