// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBase.h"
#include "EternalGrace_ProtoCharacter.h"
#include "Components/SphereComponent.h"


AInteractableBase::AInteractableBase()
{
	PrimaryActorTick.bCanEverTick = false;
	SphereRadius = 200.0f;
	InteractionSphere = CreateDefaultSubobject<USphereComponent>("InteractionRadius");
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetSphereRadius(SphereRadius);

	bCanbeActivated = true;

}

void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractableBase::RegisterInteractionRadiusEntry);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AInteractableBase::RegisterInteractionRadiusLeave);
}

void AInteractableBase::RegisterInteractionRadiusEntry(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Show Interactable UI and Set CurrentInteractable on Player 
	if (OtherActor->Implements<UPlayable>())
	{
		UE_LOG(LogTemp, Display, TEXT("%s registered radius entry"), *GetFName().ToString());
			IPlayable::Execute_ShowInteractUI(OtherActor, this); //Rename this function so it is clear that it also sets player current interactable
	}
}

void AInteractableBase::RegisterInteractionRadiusLeave(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UPlayable>())
	{
		//Hide Interactable UI
		UE_LOG(LogTemp, Display, TEXT("%s registered radius Leave"), *GetFName().ToString());
		IPlayable::Execute_HideInteractUI(OtherActor); //Rename this function so it is clear that it also removes player current interactable
	}
}

void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableBase::GetInteractedWith_Implementation(AEternalGrace_ProtoCharacter* InteractingPlayer)
{
		//Actual Functionality
	if(bCanbeActivated)
	{
		UE_LOG(LogTemp, Display, TEXT("%s gets Interacted with"), *GetFName().ToString())
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("%s is not Activated"), *GetFName().ToString())
			return;
	}
}

