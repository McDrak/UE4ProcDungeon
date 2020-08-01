// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PD_AttributeSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, float, Health, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaChangedDelegate, float, Mana, float, MaxMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChangedDelegate, float, Stamina, float, MaxStamina);

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
  GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
  GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
  GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
  GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class PROCDUNGEON_API UPD_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
    UPD_AttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	void AdjustAttributeForMaxChange(
		FGameplayAttributeData & AffectedAttribute,
		const FGameplayAttributeData & MaxAttribute,
		float NewMaxValue,
		const FGameplayAttribute & AffectedAttributeProperty
	);

    UPROPERTY(BlueprintReadOnly, Category = "Health")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UPD_AttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "MaxHealth")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UPD_AttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "HealthRegen")
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UPD_AttributeSet, HealthRegen);

	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UPD_AttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, Category = "MaxMana")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UPD_AttributeSet, MaxMana);

	UPROPERTY(BlueprintReadOnly,Category = "ManaRegen")
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UPD_AttributeSet, ManaRegen);

	UPROPERTY(BlueprintReadOnly,Category = "Stamina")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UPD_AttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly,Category = "StaminaStaminaRegen")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UPD_AttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly,Category = "StaminaRegen")
	FGameplayAttributeData StaminaRegen;
	ATTRIBUTE_ACCESSORS(UPD_AttributeSet, StaminaRegen);

	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UPD_AttributeSet, MoveSpeed);

	UPROPERTY(BlueprintReadWrite, Category = "Health")
	FOnHealthChangedDelegate OnHealthChanged;

	UPROPERTY(BlueprintReadWrite, Category = "Mana")
	FOnManaChangedDelegate OnManaChanged;

	UPROPERTY(BlueprintReadWrite, Category = "Stamina")
	FOnStaminaChangedDelegate OnStaminaChanged;
};
