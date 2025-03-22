// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterClassSelectWidget.h"
#include "ButtonWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "EternalGrace_GameInstance.h"

void UCharacterClassSelectWidget::NativeConstruct()
{
	Class1Button->GetWidgetButton()->OnClicked.AddDynamic(this, &UCharacterClassSelectWidget::SetClass1);
	Class2Button->GetWidgetButton()->OnClicked.AddDynamic(this, &UCharacterClassSelectWidget::SetClass2);


	CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GetGameInstance());
	if(!CurrentGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Character Selection could not cast Game Instance!"));
	}
}

void UCharacterClassSelectWidget::SetClass1()
{
	APlayerController* ControllingPlayer = GetOwningPlayer();
	if (ControllingPlayer && CurrentGameInstance)
	{
		if (ControllingPlayer == UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			//Set Class 1 as default class for player
			CurrentGameInstance->SetPlayerClass(0, 0);
			UE_LOG(LogTemp, Display, TEXT("Chose Class0 for Player1"));
		}
		else if (ControllingPlayer == UGameplayStatics::GetPlayerController(GetWorld(), 1))
		{
			//Set Class 1 as default class for player
			CurrentGameInstance->SetPlayerClass(1, 0);
			UE_LOG(LogTemp, Display, TEXT("Chose Class0 for Player2"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("The PlayerController created by CharacterClassSelectionWidget is wether Player 1 nor Player 2"))
				return;
		}
		RemoveFromParent();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player Controller or GameInstance is Null (CharacterSelectionWidget)"))
	}
}

void UCharacterClassSelectWidget::SetClass2()
{
	APlayerController* ControllingPlayer = GetOwningPlayer();
	if (ControllingPlayer && CurrentGameInstance)
	{
		if (ControllingPlayer == UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			//Set Class 2 as default class for player
			CurrentGameInstance->SetPlayerClass(0, 1);
			UE_LOG(LogTemp, Display, TEXT("Chose Class1 for Player1"));
		}
		else if (ControllingPlayer == UGameplayStatics::GetPlayerController(GetWorld(), 1))
		{
			//Set Class 2 as default class for player
			CurrentGameInstance->SetPlayerClass(1, 1);
			UE_LOG(LogTemp, Display, TEXT("Chose Class1 for Player2"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("The PlayerController created by CharacterClassSelectionWidget is wether Player 1 nor Player 2"))
				return;
		}
		RemoveFromParent();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player Controller or GameInstance is Null (CharacterSelectionWidget)"))
	}
}
