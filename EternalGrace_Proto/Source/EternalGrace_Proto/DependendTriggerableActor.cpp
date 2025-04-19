// Fill out your copyright notice in the Description page of Project Settings.


#include "DependendTriggerableActor.h"
#include "EternalGrace_GameInstance.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Triggerable.h"

ADependendTriggerableActor::ADependendTriggerableActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
	SetRootComponent(Mesh);
	InteractionSphere->SetupAttachment(RootComponent);

	NiagaraSystemComponent = CreateDefaultSubobject<UNiagaraComponent>("Niagara Component");
	NiagaraSystemComponent->SetupAttachment(Mesh);
}

void ADependendTriggerableActor::Tick(float DeltaSeconds)
{
}

void ADependendTriggerableActor::BeginPlay()
{
	Super::BeginPlay();
	UEternalGrace_GameInstance* CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GetWorld()->GetGameInstance());
	if (!CurrentGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT(" Failed to Cast Game Instance (DependendTriggerableActor Class) %s"), *GetFName().ToString())
			return;
	}
	CurrentGameInstance->RegisterTriggerableActor(ObjectID, this);
}

void ADependendTriggerableActor::GetInteractedWith_Implementation(AEternalGrace_ProtoCharacter* InteractingPlayer)
{
	UEternalGrace_GameInstance* CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GetWorld()->GetGameInstance());
	if (!CurrentGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT(" Failed to Cast Game Instance (DependendTriggerableActor Class) %s"), *GetFName().ToString())
			return;
	}


	if (DependingObjectID == "None")
	{
		Activate(CurrentGameInstance);
		return;
	}


	ADependendTriggerableActor* Dependency = Cast<ADependendTriggerableActor>(CurrentGameInstance->FindTriggerableActor(DependingObjectID));
	if (!Dependency)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not Cast or Find Dependency Actor in Game Instance Map (DependendTriggerable Actor Class) %s"), *GetFName().ToString())
			return;
	}

	/*This actor needs its dependency to already be activated, to be triggered*/
	bool bIsRequirementMet = !Dependency->bCanbeActivated;
	if (!bIsRequirementMet)
	{
		UE_LOG(LogTemp, Error, TEXT(" Requirement not met (DependendTriggerableActor Class) %s"), *GetFName().ToString())
			Dependency->ResetPuzzle();
		return;
	}
	Activate(CurrentGameInstance);

}

void ADependendTriggerableActor::ResetPuzzle()
{
	bCanbeActivated = true;
	if (NiagaraSystemComponent && NiagaraSystemComponent->IsActive())
	{
		NiagaraSystemComponent->SetActive(false, true);
	}

	if (DependingObjectID == "None")
	{
		return;
	}

	UEternalGrace_GameInstance* CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GetWorld()->GetGameInstance());
	if (!CurrentGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT(" Failed to Cast Game Instance (DependendTriggerableActor Class) %s"), *GetFName().ToString())
			return;
	}

	ADependendTriggerableActor* Dependency = Cast<ADependendTriggerableActor>(CurrentGameInstance->FindTriggerableActor(DependingObjectID));
	if (!Dependency)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not Cast or Find Dependency Actor in Game Instance Map (DependendTriggerable Actor Class) %s"), *GetFName().ToString())
			return;
	}
	Dependency->ResetPuzzle();
}

void ADependendTriggerableActor::Activate(UEternalGrace_GameInstance* GameInstance)
{
	UE_LOG(LogTemp, Error, TEXT(" Condition met (DependendTriggerableActor Class) %s"), *GetFName().ToString())
		if (bCanbeActivated)
		{
			bCanbeActivated = false;
			if (NiagaraSystemComponent && !NiagaraSystemComponent->IsActive())
			{
				NiagaraSystemComponent->Activate();
			}
			//If the activation of this Actor triggers another Actor. Usually used in puzzles for the last puzzle actor to the trigger the key actor.
			if(TriggerableObjectID != "None")
			{
				AActor* TriggerableOtherActor = GameInstance->FindTriggerableActor(TriggerableObjectID);
				if(TriggerableOtherActor && TriggerableOtherActor->Implements<UTriggerable>())
				{
					ITriggerable::Execute_Trigger(TriggerableOtherActor);
				}
			}
		}
}
