// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Saveable.h"
#include "GameFramework/Character.h"
#include "GroomComponent.h"
#include "ActionState.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(AllowPrivateAccess))
	USkeletalMeshComponent* HeadMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	UGroomComponent* HairComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	UGroomComponent* BeardComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	UGroomComponent* MustacheComponent;

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

	UPROPERTY()
	EActionState CurrentActionState;


public:	

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	EActionState GetCurrentActionState();

	UFUNCTION(BlueprintCallable)
	void SetCurrentActionState(EActionState ActionState);

};
