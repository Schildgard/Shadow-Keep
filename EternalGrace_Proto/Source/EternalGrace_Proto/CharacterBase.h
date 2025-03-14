// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Saveable.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UEternalGrace_SaveGame;
class UEternalGrace_GameInstance;
UCLASS()
class ETERNALGRACE_PROTO_API ACharacterBase : public ACharacter, public ISaveable
{
	GENERATED_BODY()

public:
	ACharacterBase();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(CallInEditor, Category = "SaveGame")
	virtual void SaveData_Implementation()override;
	UFUNCTION(CallInEditor, Category = "SaveGame")
	virtual void LoadData_Implementation()override;

	UFUNCTION(CallInEditor, Category = "SaveGame")
	void MakeSaveCall();
	UFUNCTION(CallInEditor, Category = "SaveGame")
	void MakeLoadCall();

	UPROPERTY()
	UEternalGrace_GameInstance* CurrentGameInstance;
	UPROPERTY()
	UEternalGrace_SaveGame* SaveGameObject;


public:	

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
