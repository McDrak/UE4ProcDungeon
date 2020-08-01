// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcDungeon/Public/Architecture/PD_Architecture.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
APD_Architecture::APD_Architecture()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FloorStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorStaticMesh"));
	RootComponent = FloorStaticMesh;

	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Component"));
	BoxCollisionComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APD_Architecture::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APD_Architecture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

