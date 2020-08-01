// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PD_CharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROCDUNGEON_API UPD_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	virtual float GetMaxSpeed() const override;
};
