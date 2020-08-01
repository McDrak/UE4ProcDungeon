// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PD_Architecture.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UBoxComponent;

UENUM(BlueprintType)
enum class EArchitectureType : uint8
{
	AT_Room          UMETA(DisplayName="Room"),
	AT_Corridor      UMETA(DisplayName="Corridor")
};

UCLASS()
class PROCDUNGEON_API APD_Architecture : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APD_Architecture();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generation")
	UStaticMeshComponent* FloorStaticMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generation")
	UBoxComponent* BoxCollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Generation")
	EArchitectureType ArchitectureType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Generation")
	TArray<USceneComponent*> SocketsArray;
};
