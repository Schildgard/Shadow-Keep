// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlayerSaveData.h"
#include "EternalGrace_GameInstance.generated.h"

/**
 *
 */
class UEternalGrace_SaveGame;
class AEternalGrace_ProtoCharacter;
UCLASS()
class ETERNALGRACE_PROTO_API UEternalGrace_GameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	UEternalGrace_GameInstance();

	void Init() override;

	//SaveGame Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SaveGame", meta = (AllowPrivateAccess))
	TSubclassOf<UEternalGrace_SaveGame> SaveGameClass;

	UPROPERTY()
	TSubclassOf<UInterface> SaveableInterfaceClass;
	UPROPERTY()
	UEternalGrace_SaveGame* CurrentSaveGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame", meta =(AllowPrivateAccess))
	TArray<FName> SaveGameSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelLoading", meta = (AllowPrivateAccess))
	FName EntryLevelName;

	UPROPERTY()
	FName ActiveSaveSlot;
	//Multiplayer Section
	//Two booleans work like a const and makes sure the MultiplayerPlayer bool can be set only once, since the second bool is not accessible by any Getter function
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame", meta = (AllowPrivateAccess))
	bool bIsMultiplayerActivated;
	UPROPERTY()
	bool bHasMultiplayerBeenSet;

	//ClassSelection
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Players", meta = (AllowPrivateAccess))
	TSubclassOf<AEternalGrace_ProtoCharacter> CharacterClass0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Players", meta = (AllowPrivateAccess))
	TSubclassOf<AEternalGrace_ProtoCharacter> CharacterClass1;

	//Player Map is used in the Class Selection to determine both Players Starting Class. The Gamemode can also use this for assigning the class on spawn,
	// even though he right now uses CharacterClass0 and 1 which I thing might be obsolete by now..
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TMap<int, TSubclassOf<AEternalGrace_ProtoCharacter>> PlayerMap;

	//World Actor Management
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Progress", meta = (AllowPrivateAccess))
	TMap<FName, AActor*> TriggerableActorMap;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Progress", meta = (AllowPrivateAccess))
	TMap<FName, bool> DependencyActorPuzzleMap;
	UFUNCTION()
	bool GetTwoPlayerMode();

	UFUNCTION()
	AActor* FindTriggerableActor(FName ObjectID);

	UFUNCTION()
	void RegisterTriggerableActor(FName ObjectID, AActor* ActorToRegister);



public:
	UFUNCTION()
	TArray<AActor*> GetAllSaveables();

	UFUNCTION()
	void RequestSave();
	UFUNCTION()
	void RequestLoad();

	UFUNCTION()
	UEternalGrace_SaveGame* GetCurrentSaveGame();

	UFUNCTION()
	FName GetActiveSaveGameSlot();
	UFUNCTION()
	void SetActiveSaveGameSlot(FName SlotID);

	UFUNCTION()
	TArray<FName> GetAllSaveGames();
	UFUNCTION()
	void FindAllSaveFiles();

	UFUNCTION()
	void DeleteFile(FName SlotID);

	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void SetMultiplayer(bool bShouldMultiplayerBeActive);

	UFUNCTION()
	void SetPlayerClass(int PlayerIndex, int ClassIndex);
	UFUNCTION()
	TSubclassOf<AEternalGrace_ProtoCharacter> GetPlayerClass(int PlayerIndex);

	UFUNCTION()
	void TestLoadLevel(UWorld* CurrentWorld);

	UFUNCTION()
	void SavePlayerData();

	UFUNCTION()
	void ResetGameStatus(UWorld* LoadedWorld);
};
