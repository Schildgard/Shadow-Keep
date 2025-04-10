// Fill out your copyright notice in the Description page of Project Settings.


#include "EternalGrace_GameInstance.h"
#include "EternalGrace_SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Saveable.h"
#include "HAL/FileManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "EternalGrace_ProtoCharacter.h"

UEternalGrace_GameInstance::UEternalGrace_GameInstance()
{
	SaveGameClass = UEternalGrace_SaveGame::StaticClass();
	SaveableInterfaceClass = USaveable::StaticClass();
	ActiveSaveSlot = FName("Slot1");
	CurrentSaveGame = nullptr;
	bIsMultiplayerActivated = false;
	bHasMultiplayerBeenSet = false;
}

void UEternalGrace_GameInstance::Init()
{
	Super::Init();
	FindAllSaveFiles();
}

bool UEternalGrace_GameInstance::GetTwoPlayerMode()
{
	return bIsMultiplayerActivated;
}

TArray<AActor*> UEternalGrace_GameInstance::GetAllSaveables()
{
	TArray<AActor*> SaveableActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), SaveableInterfaceClass, SaveableActors);

	for (AActor* SavedActor : SaveableActors)
	{
		UE_LOG(LogTemp, Display, TEXT("Interface got Information from %s"), *SavedActor->GetName())
	}

	return SaveableActors;
}

void UEternalGrace_GameInstance::RequestSave()
{
	FString SaveGameName = ActiveSaveSlot.ToString();
	USaveGame* DynamicSaveGame = nullptr;
	//check if Savegame exists and apply dynamic SaveGameReference
	bool bDoesSaveGameExist = UGameplayStatics::DoesSaveGameExist(SaveGameName, 0);
	if (bDoesSaveGameExist)
	{
		DynamicSaveGame = UGameplayStatics::LoadGameFromSlot(SaveGameName, 0);
	}
	else
	{
		DynamicSaveGame = UGameplayStatics::CreateSaveGameObject(SaveGameClass);
		CurrentSaveGame = Cast<UEternalGrace_SaveGame>(DynamicSaveGame);
	}
	if (CurrentSaveGame)
	{
		//Save
		CurrentSaveGame->SaveAllData(GetAllSaveables());
	}


}

void UEternalGrace_GameInstance::RequestSingleSave(FName ObjectID, FPlayerSaveData NewSaveData)
{
	//	FString SaveGameName = ActiveSaveSlot.ToString();
	//	//Add any Conditions here, that may prevent Saving
	//	USaveGame* DynamicSaveGame = nullptr;
	//	//check if Savegame exists and apply dynamic SaveGameReference
	//	bool bDoesSaveGameExist = UGameplayStatics::DoesSaveGameExist(SaveGameName, 0);
	//	if (bDoesSaveGameExist)
	//	{
	//		DynamicSaveGame = UGameplayStatics::LoadGameFromSlot(SaveGameName, 0);
	//	}
	//	else
	//	{
	//		DynamicSaveGame = UGameplayStatics::CreateSaveGameObject(SaveGameClass);
	//	}
	//	CurrentSaveGame = Cast<UEternalGrace_SaveGame>(DynamicSaveGame);
	//	if (CurrentSaveGame)
	//	{
	//		//Save
	//		CurrentSaveGame->SavePlayerData(ObjectID, NewSaveData);
	//	}


}

void UEternalGrace_GameInstance::RequestLoad()
{
	FString SaveGameName = ActiveSaveSlot.ToString(); //Vielleicht wird der ActiveSaveSlot nicht zuverlässig gesetzt?
	UE_LOG(LogTemp, Error, TEXT("Request Load called. Temporary SaveGameName is %s (GameInstance)"), *SaveGameName);
	//Add any Conditions here, that may prevent Loading 
	USaveGame* DynamicSaveGame = nullptr;
	bool bDoesSaveGameExist = UGameplayStatics::DoesSaveGameExist(SaveGameName, 0);
	if (!bDoesSaveGameExist)
	{
		UE_LOG(LogTemp, Error, TEXT("SaveGame did not Exist (GameInstance)"));
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("SaveGame Slot ID was %s"), *CurrentSaveGame->SlotID);
	//	DynamicSaveGame = UGameplayStatics::LoadGameFromSlot(SaveGameName, 0);
	//	CurrentSaveGame = Cast<UEternalGrace_SaveGame>(DynamicSaveGame);
	if (CurrentSaveGame)
	{
		UE_LOG(LogTemp, Error, TEXT("SaveGame Slot ID is %s"), *CurrentSaveGame->SlotID);
		CurrentSaveGame->LoadAllData(GetAllSaveables());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SaveGame Cast fail (GameInstance)"));
	}
}

UEternalGrace_SaveGame* UEternalGrace_GameInstance::GetCurrentSaveGame()
{
	return CurrentSaveGame;
}

FName UEternalGrace_GameInstance::GetActiveSaveGameSlot()
{
	return ActiveSaveSlot;
}

void UEternalGrace_GameInstance::SetActiveSaveGameSlot(FName SlotID)
{
	//Overwrite CurrentActive Slot
	ActiveSaveSlot = SlotID;

	//Check if a SaveGame of that ID already exists.
	USaveGame* DynamicSaveGame;
	FString FileName = *ActiveSaveSlot.ToString();
	if (!UGameplayStatics::DoesSaveGameExist(FileName, 0))
	{
		DynamicSaveGame = UGameplayStatics::CreateSaveGameObject(SaveGameClass);
		UGameplayStatics::SaveGameToSlot(DynamicSaveGame, FileName, 0);
		if (!DynamicSaveGame)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create a new SaveGame Object. Save Function will probably not work"));
			return;
		}
	}
	//Assign CurrentSaveGameReference to desired SaveGameFile
	DynamicSaveGame = UGameplayStatics::LoadGameFromSlot(FileName, 0);
	CurrentSaveGame = Cast<UEternalGrace_SaveGame>(DynamicSaveGame);
	if (!CurrentSaveGame)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast Current SaveGame into EternalGrace SaveGame (GameInstance)"));
		return;
	}
	CurrentSaveGame->SlotID = FileName;
}

TArray<FName> UEternalGrace_GameInstance::GetAllSaveGames()
{
	//This Function is used to return the FName List of the SaveGames
	return SaveGameSlots;
}

void UEternalGrace_GameInstance::FindAllSaveFiles()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance now searches for SaveFiles"));
	TArray<FString> SaveGameFiles;
	FString SaveDirectory = FPaths::ProjectSavedDir() + TEXT("SaveGames/");

	IFileManager::Get().FindFiles(SaveGameFiles, *SaveDirectory, TEXT(".sav"));

	for (FString& SaveFile : SaveGameFiles)
	{
		SaveFile = FPaths::GetBaseFilename(SaveFile);
		SaveGameSlots.Add(FName(SaveFile));
		UE_LOG(LogTemp, Display, TEXT("Found Save File %s"), *SaveFile);
	}

}

void UEternalGrace_GameInstance::DeleteFile(FName SlotID)
{
	FString FileToDelete = *SlotID.ToString();
	if (UGameplayStatics::DoesSaveGameExist(FileToDelete, 0))
	{
		UGameplayStatics::DeleteGameInSlot(FileToDelete, 0);
		UE_LOG(LogTemp, Warning, TEXT("SaveGame Found and Deleted (Game Instance)"));
		if (SaveGameSlots.Contains(SlotID))
		{
			SaveGameSlots.Remove(SlotID);
			UE_LOG(LogTemp, Warning, TEXT("Removed Slot from SaveGameSlots (Game Instance)"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT(" Could not remove Slot from SaveGameSlots (Game Instance)"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Delete failed. SaveGameSlot didnt found (Game Instance)"));
	}
}

void UEternalGrace_GameInstance::StartGame()
{
	int SlotNumber = GetAllSaveGames().Num() + 1;
	FName NewSlotName;
	if (bIsMultiplayerActivated)
	{
		NewSlotName = FName(TEXT("Multiplayer Session "), SlotNumber);
	}
	else
	{
		NewSlotName = FName(TEXT("Singleplayer Session "), SlotNumber);
	}
	SetActiveSaveGameSlot(NewSlotName);

	CurrentSaveGame->SetTwoPlayerModeInfo(bIsMultiplayerActivated);
	CurrentSaveGame->SetPlayerMap(PlayerMap);
	//Load The Game
	UGameplayStatics::OpenLevel(GetWorld(), FName("Map_EntryLevel"));
}

void UEternalGrace_GameInstance::ResumeGame()
{
	SetMultiplayer(CurrentSaveGame->GetTwoPlayerModeInfo());
	PlayerMap = CurrentSaveGame->GetPlayerMap();
	UGameplayStatics::OpenLevel(GetWorld(), FName("Map_EntryLevel"));
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UEternalGrace_GameInstance::TestLoadLevel);
}

void UEternalGrace_GameInstance::SetMultiplayer(bool bShouldMultiplayerBeActive)
{
	if (bHasMultiplayerBeenSet)
	{
		UE_LOG(LogTemp, Error, TEXT("Multiplayer can only be set before starting a Game in the Main Menu."))
			return;
	}
	bIsMultiplayerActivated = bShouldMultiplayerBeActive;
	bHasMultiplayerBeenSet = true;
}

void UEternalGrace_GameInstance::SetPlayerClass(int PlayerIndex, int ClassIndex)
{

	if (PlayerIndex > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid PlayerIndex (GameInstance)"));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Set Player %i Default Class to %i (GameInstance)"), PlayerIndex, ClassIndex);
	switch (ClassIndex)
	{
	case 0:
		PlayerMap.Add(PlayerIndex, CharacterClass0);
		break;
	case 1:
		PlayerMap.Add(PlayerIndex, CharacterClass1);
		break;
	default:
		PlayerMap.Add(PlayerIndex, CharacterClass0);
		break;
	}
	UE_LOG(LogTemp, Display, TEXT("PlayerMap Contains %i PlayerData now (GameInstance)"), PlayerMap.Num());

	if (bIsMultiplayerActivated)
	{
		if (!PlayerMap.Contains(0))
		{
			UE_LOG(LogTemp, Display, TEXT("Waiting for Player 1"));
			return;
		}
		else if (!PlayerMap.Contains(1))
		{
			UE_LOG(LogTemp, Display, TEXT("Waiting for Player 2"));
			return;
		}
	}
	StartGame();
}

TSubclassOf<AEternalGrace_ProtoCharacter> UEternalGrace_GameInstance::GetPlayerClass(int PlayerIndex)
{
	return *PlayerMap.Find(PlayerIndex);
}

void UEternalGrace_GameInstance::TestLoadLevel(UWorld* CurrentWorld)
{
	RequestLoad();
}

void UEternalGrace_GameInstance::SavePlayerData()
{
	APlayerController* FirstPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (FirstPlayer)
	{
		AEternalGrace_ProtoCharacter* FirstPlayerChar = Cast<AEternalGrace_ProtoCharacter>(FirstPlayer->GetCharacter());
		if(FirstPlayerChar)
		{
			FirstPlayerChar->Execute_SaveData(FirstPlayerChar);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to Chast first character in savePlayer Function (GameInstance)"))
		}
	}

	if(bIsMultiplayerActivated)
	{
		APlayerController* SecondPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 1);
		if (SecondPlayer)
		{
			AEternalGrace_ProtoCharacter* SecondPlayerChar = Cast<AEternalGrace_ProtoCharacter>(SecondPlayer->GetCharacter());
			if (SecondPlayerChar)
			{
				SecondPlayerChar->Execute_SaveData(SecondPlayerChar);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to Chast second character in savePlayer Function (GameInstance)"))
		}

	}

}
/*This Function is used when returning to Main Menu*/
void UEternalGrace_GameInstance::ResetGameStatus(UWorld* LoadedWorld)
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	bIsMultiplayerActivated = false;
	bHasMultiplayerBeenSet = false;
	PlayerMap.Empty();
	APlayerController* SecondPlayerController = UGameplayStatics::GetPlayerController(LoadedWorld, 1);
	if(SecondPlayerController)
	{
		//UGameplayStatics::RemovePlayer(SecondPlayerController, false);
		RemoveLocalPlayer(GetLocalPlayerByIndex(1));
		//SecondPlayerController->Destroy();
		UE_LOG(LogTemp, Error, TEXT("Destroyed second player controller (GameInstance)"))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("failed to get second player controller (GameInstance)"))
	}
}


