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
}

void ADefaultGameMode::BeginPlay()
{
	Super::BeginPlay();

	//Check if MultiplayerMode is activated
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
	AActor* StartActor = ChoosePlayerStart(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	// Get Class Type of first player from GameInstance, which is set by the Character Selection Button.
	Player1Character = CurrentGameInstance->GetPlayerClass(0);
	if (Player1Character)
	{
		//Spawn Character and let Controller posses him. 
		AEternalGrace_ProtoCharacter* Player1 = GetWorld()->SpawnActor<AEternalGrace_ProtoCharacter>(Player1Character, StartActor->GetActorLocation(), StartActor->GetActorRotation());
		if (!Player1)
		{
			UE_LOG(LogTemp, Error, TEXT("Player1 Could not be spawned! (DefaultGameMode)"));
			return;
		}
		APlayerController* PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerControllerRef)
		{
			AEG_PlayerController* FirstPlayerController = Cast<AEG_PlayerController>(PlayerControllerRef);
			if (FirstPlayerController)
			{
				Player1->SetPlayerIndex(0);
				FirstPlayerController->Possess(Player1);
				FirstPlayerController->SetOwningCharacterVariable(Player1);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("First Player Controller could not be Cast (Game Mode)"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("First Player Controller Ref could not be getted"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode Could not get Player1 Character Class"))
	}

	//repeat the some for second player.
	if (bIsTwoPlayerModeActivated)
	{
		Player2Character = CurrentGameInstance->GetPlayerClass(1);
		if (Player2Character)
		{
			APlayerController* PlayerControllerRef2 = UGameplayStatics::GetPlayerController(GetWorld(), 1);
			if(!PlayerControllerRef2)
			{
				//If a game is loaded, the second controller does not exist yet (since it is created in class selection usually. So a new one has to be created.
				PlayerControllerRef2 = UGameplayStatics::CreatePlayer(GetWorld(), 1);
			}
			AEG_PlayerController* SecondPlayerController = Cast<AEG_PlayerController>(PlayerControllerRef2);
			if (SecondPlayerController)
			{
				AEternalGrace_ProtoCharacter* Player2 = GetWorld()->SpawnActor<AEternalGrace_ProtoCharacter>(Player2Character, StartActor->GetActorLocation(), StartActor->GetActorRotation());
				if (!Player2)
				{
					UE_LOG(LogTemp, Error, TEXT("Player2 Could not be spawned! (DefaultGameMode)"));
					return;
				}

				Player2->SetPlayerIndex(1);
				SecondPlayerController->Possess(Player2);
				SecondPlayerController->SetOwningCharacterVariable(Player2);
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
