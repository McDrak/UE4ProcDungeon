// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcDungeon/Public/Core/PD_Generator.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "ProcDungeon/Public/Architecture/PD_Architecture.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

//---------------------------------------------------------------------------------------------------------------------
// Sets default values
APD_Generator::APD_Generator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FloorMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor Mesh"));
	FloorMesh->SetupAttachment(RootComponent);
}

//---------------------------------------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void APD_Generator::BeginPlay()
{
	Super::BeginPlay();
}

//---------------------------------------------------------------------------------------------------------------------
// Called every frame
void APD_Generator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//---------------------------------------------------------------------------------------------------------------------
void APD_Generator::GenerateDungeon()
{
	int RoomsCounter = 0;
	// Spawns the Base Room on the World -> The 0 position should the Room with most posible exits
	APD_Architecture* BaseRoom = GetWorld()->SpawnActor<APD_Architecture>(RoomArray[0], GetActorTransform());
	// Init empty socket array
	TArray<USceneComponent*> SocketsArray;
	// Append the BaseRoom's own socket array to the main one
	SocketsArray.Append(BaseRoom->SocketsArray);

	while (SocketsArray.Num() > 0 && RoomsCounter < MaxRooms)
	{
		// Select a random socket within the array to start working with
		int RandomSocketIndex = FMath::RandRange(0, SocketsArray.Num() - 1);
		// Get the instance of the selected socket
		USceneComponent* CurrentSocket = SocketsArray[RandomSocketIndex];
		// Get the Owner from the select socket
		APD_Architecture* CurrentSocketParent = Cast<APD_Architecture>(CurrentSocket->GetOwner());

		// If the Architecture it's a Room
		if (CurrentSocketParent->ArchitectureType == EArchitectureType::AT_Room)
		{
			// Get a random index within the Corridor array length
			int RandomCorridorIndex = FMath::RandRange(0, CorridorArray.Num() - 1);
			// Spawn a new Structure on the center of the map
			APD_Architecture* SelectedCorridor = GetWorld()->
				SpawnActor<APD_Architecture>(CorridorArray[RandomCorridorIndex], GetActorTransform());

			// Get a random index of a socket where the current Corridor it's the owner
			int CorridoRandomSocketIndex = FMath::RandRange(0, SelectedCorridor->SocketsArray.Num() - 1);
			// Get the instance of that socket
			USceneComponent* SelectedCorridorSocket = SelectedCorridor->SocketsArray[CorridoRandomSocketIndex];

			// Creates a temporal position vector where the structure will translate
			FVector TargetTranslation = 
				CurrentSocket->GetComponentLocation() - SelectedCorridorSocket->GetComponentLocation();
			// Creates a vector from the selected Corridor socket to the current room socket
			FVector InverseTL = UKismetMathLibrary::InverseTransformLocation
				(SelectedCorridorSocket->GetComponentTransform(), CurrentSocket->GetForwardVector());
			// Creates a vector to target the rotator
			FVector PreRotatorVector = InverseTL * SelectedCorridorSocket->GetForwardVector().GetSignVector();
			// Creates a rotator on the X axis of the previous vector
			FRotator TargetRotation = UKismetMathLibrary::MakeRotFromZ(PreRotatorVector);

			// Translates and rotates the Architecture
			SelectedCorridor->SetActorLocationAndRotation(
				TargetTranslation,
				FRotator(0.f, TargetRotation.Yaw + CurrentSocket->GetComponentRotation().Yaw, 0.f)
			);

			// Pushes the structure to it's final position matching both sockets locations
			FVector FinalTranslatePosition = 
				(CurrentSocket->GetComponentLocation() - SelectedCorridorSocket->GetComponentLocation()) + 
				SelectedCorridor->GetActorLocation();
			SelectedCorridor->SetActorLocation(FinalTranslatePosition);

			// Array to contain overlapping actors
			TArray<AActor*> OverlappingActors;
			// Overlapping Flag
			bool IsOverlappingCorridor = false;
			// Gets the current overlapping actors and cycle through each of them
			SelectedCorridor->BoxCollisionComponent->GetOverlappingActors(OverlappingActors);
			for (int OverlapIterator = 0; OverlapIterator < OverlappingActors.Num() && IsOverlappingCorridor == false; OverlapIterator++)
			{
				// Gets the current OverlappingActor and checks if Overlapping with another corridor or another room
				APD_Architecture* OverlapArchitecture = Cast<APD_Architecture>(OverlappingActors[OverlapIterator]);
				if (
					OverlapArchitecture &&
					((OverlapArchitecture->ArchitectureType == EArchitectureType::AT_Corridor &&
					OverlapArchitecture != SelectedCorridor)
					||
					(OverlapArchitecture->ArchitectureType == EArchitectureType::AT_Room &&
					OverlapArchitecture != CurrentSocketParent))
				)
				{
					IsOverlappingCorridor = true;
				}
			}

			// If we are overlapping something
			if (IsOverlappingCorridor)
			{
				GetWorld()->DestroyActor(SelectedCorridor);
			}
			else
			{
				// Remove the used socket from it's owner array
				SelectedCorridor->SocketsArray.RemoveAt(CorridoRandomSocketIndex);
				if (SelectedCorridor->SocketsArray.Num() > 0)
				{
					// If the are still sockets to be used, then append them to the main array
					SocketsArray.Append(SelectedCorridor->SocketsArray);
				}
			}
		}
		else if (CurrentSocketParent->ArchitectureType == EArchitectureType::AT_Corridor)
		{
			// Get a random index within the Corridor array length
			int RandomRoomIndex = FMath::RandRange(0, RoomArray.Num() - 1);
			// Spawn a new Structure on the center of the map
			APD_Architecture* SelectedRoom = GetWorld()->
				SpawnActor<APD_Architecture>(RoomArray[RandomRoomIndex], GetActorTransform());

			// Get a random index of a socket where the current Corridor it's the owner
			int RoomRandomSocketIndex = FMath::RandRange(0, SelectedRoom->SocketsArray.Num() - 1);
			// Get the instance of that socket
			USceneComponent* SelectedRoomSocket = SelectedRoom->SocketsArray[RoomRandomSocketIndex];

			// Creates a temporal position vector where the structure will translate
			FVector TargetTranslation =
				CurrentSocket->GetComponentLocation() - SelectedRoomSocket->GetComponentLocation();
			// Creates a vector from the selected Corridor socket to the current room socket
			FVector InverseTL = UKismetMathLibrary::InverseTransformLocation
				(SelectedRoomSocket->GetComponentTransform(), CurrentSocket->GetForwardVector());
			// Creates a vector to target the rotator
			FVector PreRotatorVector = InverseTL * SelectedRoomSocket->GetForwardVector().GetSignVector();
			// Creates a rotator on the X axis of the previous vector
			FRotator TargetRotation = UKismetMathLibrary::MakeRotFromZ(PreRotatorVector);

			// Gets the Y Forward Vector Axis rounded
			int RoundedYAxis = UKismetMathLibrary::Round(SelectedRoomSocket->GetForwardVector().Y);
			// Angle offset to add to the final rotation
			float AngleOffset = 0.f;
			// Direction Verification
			if (RoundedYAxis == 0)
			{
				AngleOffset = 270.f;
			}
			else if (RoundedYAxis == 1)
			{
				AngleOffset = 180.f;
			}

			// Translates and rotates the Architecture
			SelectedRoom->SetActorLocationAndRotation(
				TargetTranslation,
				FRotator(0.f, TargetRotation.Yaw + CurrentSocket->GetComponentRotation().Yaw + AngleOffset, 0.f)
			);

			// Pushes the structure to it's final position matching both sockets locations
			FVector FinalTranslatePosition =
				(CurrentSocket->GetComponentLocation() - SelectedRoomSocket->GetComponentLocation()) +
				SelectedRoom->GetActorLocation();
			SelectedRoom->SetActorLocation(FinalTranslatePosition);

			// Array to contain overlapping actors
			TArray<AActor*> OverlappingActors;
			// Overlapping flag
			bool IsOverlappingRoom = false;
			// Gets the current overlapping actors and cycle thropugh each of them
			SelectedRoom->BoxCollisionComponent->GetOverlappingActors(OverlappingActors);
			for (int OverlapIterator = 0; OverlapIterator < OverlappingActors.Num() && IsOverlappingRoom == false; OverlapIterator++)
			{
				// If we are overlapping with another room set the flag
				APD_Architecture* OverlapArchitecture = Cast<APD_Architecture>(OverlappingActors[OverlapIterator]);
				if (
					OverlapArchitecture &&
					OverlapArchitecture->ArchitectureType == EArchitectureType::AT_Room &&
					OverlapArchitecture != SelectedRoom
				)
				{
					IsOverlappingRoom = true;
				}
			}

			// if we are overlapping with another room
			if (IsOverlappingRoom)
			{
				GetWorld()->DestroyActor(SelectedRoom);
			}
			else
			{
				// Remove the used socket from it's owner array
				SelectedRoom->SocketsArray.RemoveAt(RoomRandomSocketIndex);
				if (SelectedRoom->SocketsArray.Num() > 0)
				{
					// If the are still sockets to be used, then append them to the main array
					SocketsArray.Append(SelectedRoom->SocketsArray);
				}

				RoomsCounter++;
			}
		}

		SocketsArray.RemoveAt(RandomSocketIndex);
	}
}
