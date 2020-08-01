// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcDungeon/Public/Characters/PD_AICharacter.h"

// Sets default values
APD_AICharacter::APD_AICharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APD_AICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APD_AICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APD_AICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

