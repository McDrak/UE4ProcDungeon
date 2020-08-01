// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PD_Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class PROCDUNGEON_API APD_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APD_Projectile();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities")
	float Range;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USphereComponent* SphereComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UParticleSystemComponent * ParticleSystemComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
