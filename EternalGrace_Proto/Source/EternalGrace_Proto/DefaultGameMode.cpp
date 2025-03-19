// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameMode.h"
#include "EternalGrace_ProtoCharacter.h"
#include "EternalGrace_GameInstance.h"
#include "EG_PlayerController.h"
#include "Kismet/GameplayStatics.h"

ADefaultGameMode::ADefaultGameMode()
	: Player1Character(nullptr),
	Player2Character(nullptr)
{
	SecondPlayerController = nullptr;
}

void ADefaultGameMode::BeginPlay()
{
	Super::BeginPlay();
	bool bIsTwoPlayerModeActivated;
	UGameInstance* GameInstanceReference = GetGameInstance();
	UEternalGrace_GameInstance* CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GameInstanceReference);
	if (CurrentGameInstance)
	{
		bIsTwoPlayerModeActivated = CurrentGameInstance->GetTwoPlayerMode();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameMade could not cast Game Instance. Game locked to Singleplayer"));
		bIsTwoPlayerModeActivated = false;
	}

	Player1Character = CurrentGameInstance->GetPlayerClass(0);
	if (Player1Character)
	{
		FVector Player1SpawnLocation = FVector(0);
		FRotator Player1SpawnRotation = FRotator(0);
		AEternalGrace_ProtoCharacter* Player1 = GetWorld()->SpawnActor<AEternalGrace_ProtoCharacter>(Player1Character, Player1SpawnLocation, Player1SpawnRotation);
		if (Player1)
		{
			Player1->SetPlayerIndex(0);
			GetWorld()->GetFirstPlayerController()->Possess(Player1);
		}
		else UE_LOG(LogTemp, Error, TEXT("Player1 Could not be spawned! (DefaultGameMode)"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode Could not get Player1 Character Class"))
	}

	if (bIsTwoPlayerModeActivated)
	{
		Player2Character = CurrentGameInstance->GetPlayerClass(1);
		if (Player2Character)
		{
			FVector Player2SpawnLocation = FVector(100);
			FRotator Player2SpawnRotation = FRotator(0);

			APlayerController* NewPlayerController = UGameplayStatics::CreatePlayer(GetWorld(), -1);
			SecondPlayerController = Cast<AEG_PlayerController>(NewPlayerController);
			if (SecondPlayerController)
			{
				AEternalGrace_ProtoCharacter* Player2 = GetWorld()->SpawnActor<AEternalGrace_ProtoCharacter>(Player2Character, Player2SpawnLocation, Player2SpawnRotation);
				if (!Player2)
				{
					UE_LOG(LogTemp, Error, TEXT("Player2 Could not be spawned! (DefaultGameMode)"));
					return;
				}

				Player2->SetPlayerIndex(1);
				SecondPlayerController->Possess(Player2);
				SecondPlayerController->SetOwningCharacterVariable(Player2);
				if (SecondPlayerController->GetOwningCharacter())
				{
					UE_LOG(LogTemp, Warning, TEXT("Second Player Controller Owner has been set to %s"), *Player2->GetFName().ToString());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("player controller 2 failed to stand its ground"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Second Player Controller Spawn Failed"))
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GameMode Could not get Player2 Character Class"))
		}
	}


}
