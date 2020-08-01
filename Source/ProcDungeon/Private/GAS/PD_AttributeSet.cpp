// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcDungeon/Public/GAS/PD_AttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

//---------------------------------------------------------------------------------------------------------------------
UPD_AttributeSet::UPD_AttributeSet()
{
	MaxHealth = 100.f;
	Health = MaxHealth;
	HealthRegen = 0.5f;

	MaxMana = 100.f;
	Mana = MaxMana;
	ManaRegen = 1.f;

	MaxStamina = 100.f;
	Stamina = MaxStamina;
	StaminaRegen = 5.f;

	MoveSpeed = 600.f;
}

//---------------------------------------------------------------------------------------------------------------------
void UPD_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute()) // GetMaxHealthAttribute comes from the Macros defined at the top of the header
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
	}
	else if (Attribute == GetMoveSpeedAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 150.f, 1000.f);
	}
}

//---------------------------------------------------------------------------------------------------------------------
void UPD_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// Checks if Health attribute was affected
	if (
		Data.EvaluatedData.Attribute.GetUProperty() ==
		FindFieldChecked<UProperty>(UPD_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UPD_AttributeSet, Health))
	)
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.f, MaxHealth.GetCurrentValue()));
		OnHealthChanged.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
	}
	// Checks if Mana attribute was affected
	else if (
		Data.EvaluatedData.Attribute.GetUProperty() ==
		FindFieldChecked<UProperty>(UPD_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UPD_AttributeSet, Mana))
	)
	{
		Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.f, MaxMana.GetCurrentValue()));
		Mana.SetBaseValue(FMath::Clamp(Mana.GetBaseValue(), 0.f, MaxMana.GetCurrentValue()));
		OnManaChanged.Broadcast(Mana.GetCurrentValue(), MaxMana.GetCurrentValue());
	}
	// Checks if Stamina attribute was affected
	else if (
		Data.EvaluatedData.Attribute.GetUProperty() ==
		FindFieldChecked<UProperty>(UPD_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UPD_AttributeSet, Stamina))
	)
	{
		Stamina.SetCurrentValue(FMath::Clamp(Stamina.GetCurrentValue(), 0.f, MaxStamina.GetCurrentValue()));
		Stamina.SetBaseValue(FMath::Clamp(Stamina.GetBaseValue(), 0.f, MaxStamina.GetCurrentValue()));
		OnStaminaChanged.Broadcast(Stamina.GetCurrentValue(), MaxStamina.GetCurrentValue());
	}
}

//---------------------------------------------------------------------------------------------------------------------
void UPD_AttributeSet::AdjustAttributeForMaxChange(
	FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute,
	float NewMaxValue,
	const FGameplayAttribute& AffectedAttributeProperty
)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}
