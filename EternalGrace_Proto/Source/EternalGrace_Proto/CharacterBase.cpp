// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "EternalGrace_SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "EternalGrace_GameInstance.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SaveGameObject = nullptr;

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* GameInstanceReference = UGameplayStatics::GetGameInstance(GetWorld());
	CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GameInstanceReference);

	SaveGameObject = CurrentGameInstance->GetCurrentSaveGame();
	
}

void ACharacterBase::SaveData_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("%s Saves its Data"), *GetName())
}

void ACharacterBase::LoadData_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("%s Loads its Data"), *GetName())
}

void ACharacterBase::MakeSaveCall()
{
	if(CurrentGameInstance)
	{
		CurrentGameInstance->RequestSave();
	}
}

void ACharacterBase::MakeLoadCall()
{
	if(CurrentGameInstance)
	{
		CurrentGameInstance->RequestLoad();
	}
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

