// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerableActor.h"
#include "EternalGrace_GameInstance.h"

// Sets default values
ATriggerableActor::ATriggerableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void ATriggerableActor::BeginPlay()
{
	Super::BeginPlay();

	UEternalGrace_GameInstance* CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GetWorld()->GetGameInstance());
	if (CurrentGameInstance)
	{
		/* CheckIfthisActor is already Registered. For Example when a SaveFile is loaded */
		if (!CurrentGameInstance->FindTriggerableActor(ObjectID))
		{
			CurrentGameInstance->RegisterTriggerableActor(ObjectID, this);
		}
	}

}

// Called every frame
void ATriggerableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerableActor::Trigger_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("Trigger: %s"), *GetFName().ToString())
}

