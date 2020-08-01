// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcDungeon/Public/GAS/PD_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

//---------------------------------------------------------------------------------------------------------------------
// Sets default values
APD_Projectile::APD_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	RootComponent = SphereComponent;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent >(TEXT("Static Mesh Component"));
	ParticleSystemComponent->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
}

//---------------------------------------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void APD_Projectile::BeginPlay()
{
	Super::BeginPlay();
}

//---------------------------------------------------------------------------------------------------------------------
// Called every frame
void APD_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

