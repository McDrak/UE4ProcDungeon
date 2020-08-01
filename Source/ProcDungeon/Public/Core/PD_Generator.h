// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PD_Generator.generated.h"

class UInstancedStaticMeshComponent;
class UStaticMesh;
class APD_Architecture;

UCLASS()
class PROCDUNGEON_API APD_Generator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APD_Generator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Generation")
	TArray<TSubclassOf<APD_Architecture>> RoomArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Generation")
		TArray<TSubclassOf<APD_Architecture>> CorridorArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Generation")
	UInstancedStaticMeshComponent* FloorMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Generation")
	int MaxDepth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Generation")
	int MaxRooms;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Generation")
	void GenerateDungeon();
};
