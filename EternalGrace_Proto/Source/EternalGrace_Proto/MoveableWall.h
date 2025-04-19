// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TriggerableActor.h"
#include "Saveable.h"
#include "TriggerableSaveData.h"
#include "MoveableWall.generated.h"

/**
 * 
 */
class UEternalGrace_SaveGame;
UCLASS()
class ETERNALGRACE_PROTO_API AMoveableWall : public ATriggerableActor, public ISaveable
{
	GENERATED_BODY()
	AMoveableWall();

	virtual void Trigger_Implementation()override;
	virtual void Tick(float DeltaSeconds)override;
	virtual void BeginPlay()override;
	virtual void SaveData_Implementation()override;
	virtual void LoadData_Implementation()override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	FVector MoveDestination;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float MoveSpeed;

	UPROPERTY()
	FTriggerableSaveData SaveDataInfo;

	UEternalGrace_SaveGame* CurrentSaveGame;

public:
	UFUNCTION()
	void MoveToDestination(float DeltaSeconds);
	
};
