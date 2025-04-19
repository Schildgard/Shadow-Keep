// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveableWall.h"
#include "EternalGrace_SaveGame.h"
#include "EternalGrace_GameInstance.h"


AMoveableWall::AMoveableWall()
{
	MoveDestination = FVector(0.0f);
	MoveSpeed = 20.f;
}

void AMoveableWall::BeginPlay()
{
	Super::BeginPlay();
	UEternalGrace_GameInstance* CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GetWorld()->GetGameInstance());
	if (!CurrentGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Cast Current Game Instance (Moveable Wall Class)"))
			return;
	}
	CurrentSaveGame = CurrentGameInstance->GetCurrentSaveGame();
	if (!CurrentSaveGame)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get Current SaveGame from Game Instance (Moveable Wall Class)"))
			return;
	}
}

void AMoveableWall::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bIsTriggered)
	{
		MoveToDestination(DeltaSeconds);
	}
}


void AMoveableWall::Trigger_Implementation()
{
	Super::Trigger_Implementation();
	bIsTriggered = true;
	SaveDataInfo.bHasBeenTriggered = true;
	ISaveable::Execute_SaveData(this);
}


void AMoveableWall::SaveData_Implementation()
{
	CurrentSaveGame->SaveTriggerableData(ObjectID, SaveDataInfo);

}

void AMoveableWall::LoadData_Implementation()
{
	FTriggerableSaveData* SavedData = CurrentSaveGame->LoadTriggerableSaveData(ObjectID);
	if (SavedData)
	{
		SaveDataInfo = *SavedData;
		if (SavedData->bHasBeenTriggered == true)
		{
			Destroy();
		}
	}
	else UE_LOG(LogTemp, Warning, TEXT("%s found no Load Data"), *GetFName().ToString())
}


void AMoveableWall::MoveToDestination(float DeltaSeconds)
{
	FVector CurrentLocation = GetActorLocation();

	if (CurrentLocation.Z >= MoveDestination.Z)
	{
		SetActorLocation(FVector(CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z - (DeltaSeconds * MoveSpeed)));
	}
	else
	{
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("%s reached its Destination and destroys itself"), *GetFName().ToString())
	}
}
