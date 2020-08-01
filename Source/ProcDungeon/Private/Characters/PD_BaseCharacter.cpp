// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcDungeon/Public/Characters/PD_BaseCharacter.h"
#include "ProcDungeon/Public/Characters/PD_CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "ProcDungeon/Public/GAS/PD_AbilitySystemComponent.h"
#include "ProcDungeon/Public/GAS/PD_AttributeSet.h"
#include "ProcDungeon/Public/GAS/PD_GameplayAbility.h"
#include "ProcDungeon/Public/GAS/PD_GameplayEffect.h"

//---------------------------------------------------------------------------------------------------------------------
// Sets default values
APD_BaseCharacter::APD_BaseCharacter(const class FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer.SetDefaultSubobjectClass<UPD_CharacterMovementComponent>
		(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// GAS Init
	AbilitySystemComponent = CreateDefaultSubobject<UPD_AbilitySystemComponent>(TEXT("Ability System Component"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	AttributeSet = CreateDefaultSubobject<UPD_AttributeSet>(TEXT("Attribute Set"));
}

//---------------------------------------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void APD_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	AttributeSet->OnHealthChanged.AddDynamic(this, &APD_BaseCharacter::OnHealthChanged);
	AttributeSet->OnManaChanged.AddDynamic(this, &APD_BaseCharacter::OnManaChanged);
	AttributeSet->OnStaminaChanged.AddDynamic(this, &APD_BaseCharacter::OnStaminaChanged);
}

//---------------------------------------------------------------------------------------------------------------------
// Called every frame
void APD_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//---------------------------------------------------------------------------------------------------------------------
// Called to bind functionality to input
void APD_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//---------------------------------------------------------------------------------------------------------------------
void APD_BaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsValid(AbilitySystemComponent))
	{
		AddCharacterAbilities();
		AddStartupEffects();
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

//---------------------------------------------------------------------------------------------------------------------
void APD_BaseCharacter::BindASCInput()
{
	if (IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(
			InputComponent,
			FGameplayAbilityInputBinds(
				FString("ConfirmTarget"),
				FString("CancelTarget"),
				FString("EPD_AbilityInputID"),
				static_cast<int32>(EPD_AbilityInputID::Confirm),
				static_cast<int32>(EPD_AbilityInputID::Cancel)
			)
		);
	}
}

//---------------------------------------------------------------------------------------------------------------------
void APD_BaseCharacter::AddCharacterAbilities()
{
	// Grants Abilities on Server (if any)
	if (
		GetLocalRole() != ROLE_Authority ||
		!IsValid(AbilitySystemComponent) ||
		AbilitySystemComponent->StartupAbilitiesGiven
		)
	{
		return;
	}

	for (TSubclassOf<UPD_GameplayAbility>& StartupAbility : StartingAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(
				StartupAbility,
				1,
				static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID),
				this
			)
		);
	}

	AbilitySystemComponent->StartupAbilitiesGiven = true;
}

//---------------------------------------------------------------------------------------------------------------------
void APD_BaseCharacter::RemoveCharacterAbilities()
{
	// Remove Abilities on Server (if any)
	if (
		GetLocalRole() != ROLE_Authority ||
		!IsValid(AbilitySystemComponent) ||
		AbilitySystemComponent->StartupAbilitiesGiven
		)
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && StartingAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for (int32 AbilityIterator = 0; AbilityIterator < AbilitiesToRemove.Num(); AbilityIterator++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[AbilityIterator]);
	}

	AbilitySystemComponent->StartupAbilitiesGiven = false;
}

//---------------------------------------------------------------------------------------------------------------------
void APD_BaseCharacter::AddStartupEffects()
{
	if (
		GetLocalRole() != ROLE_Authority ||
		!IsValid(AbilitySystemComponent) ||
		AbilitySystemComponent->StartupEffectsApplied
		)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UPD_GameplayEffect> StartingEffect : StartingEffects)
	{
		FGameplayEffectSpecHandle NewHandle =
			AbilitySystemComponent->MakeOutgoingSpec(StartingEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle =
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
					*NewHandle.Data.Get(), AbilitySystemComponent
				);
		}
	}

	AbilitySystemComponent->StartupEffectsApplied = true;
}

//---------------------------------------------------------------------------------------------------------------------
void APD_BaseCharacter::RemoveGameplayEffects()
{
	if (
		GetLocalRole() != ROLE_Authority ||
		!IsValid(AbilitySystemComponent) ||
		AbilitySystemComponent->StartupEffectsApplied
		)
	{
		return;
	}

	AbilitySystemComponent->CancelAllAbilities();

	FGameplayTagContainer EffectTagsToRemove;
	EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
	int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);
}

//---------------------------------------------------------------------------------------------------------------------
bool APD_BaseCharacter::IsCharacterAlive() const
{
	return AttributeSet->GetHealth() > 0.f;
}

//---------------------------------------------------------------------------------------------------------------------
float APD_BaseCharacter::GetMoveSpeed() const
{
	if (IsValid(AttributeSet))
	{
		return AttributeSet->GetMoveSpeed();
	}

	return 0.f;
}

//---------------------------------------------------------------------------------------------------------------------
void APD_BaseCharacter::OnHealthChanged(float Health, float MaxHealth)
{
	if (Health <= 0.f)
	{
		BP_Die();
	}
	BP_OnHealthChanged(Health, MaxHealth);
}

//---------------------------------------------------------------------------------------------------------------------
void APD_BaseCharacter::OnManaChanged(float Mana, float MaxMana)
{
	BP_OnManaChanged(Mana, MaxMana);
}

//---------------------------------------------------------------------------------------------------------------------
void APD_BaseCharacter::OnStaminaChanged(float Stamina, float MaxStamina)
{
	BP_OnStaminaChanged(Stamina, MaxStamina);
}

//---------------------------------------------------------------------------------------------------------------------
void APD_BaseCharacter::OnTargetTrigger(EPD_AbilityInputID AbilityID, bool Trigger)
{
	BP_TargetTrigger(AbilityID, Trigger);
}

//---------------------------------------------------------------------------------------------------------------------
UAbilitySystemComponent* APD_BaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}