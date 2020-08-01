// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProcDungeon/Public/Characters/PD_BaseCharacter.h"
#include "PD_AICharacter.generated.h"

UCLASS()
class PROCDUNGEON_API APD_AICharacter : public APD_BaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APD_AICharacter(const class FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
