// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor_Trigger.h"
#include "EternalGrace_GameInstance.h"
#include "Triggerable.h"
#include "Components/SphereComponent.h"
#include "EternalGrace_ProtoCharacter.h"
#include "InventoryComponent.h"

AInteractableActor_Trigger::AInteractableActor_Trigger()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
	SetRootComponent(Mesh);
	InteractionSphere->SetupAttachment(RootComponent);
	RequiredKeyObjectID = "None";
}

void AInteractableActor_Trigger::TriggerRelatedObject()
{
	UEternalGrace_GameInstance* CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GetWorld()->GetGameInstance());
	if (CurrentGameInstance)
	{
		AActor* TriggerableObject = CurrentGameInstance->FindTriggerableActor(TriggerableObjectID);
		if (TriggerableObject)
		{
			if (TriggerableObject->Implements<UTriggerable>())
			{
				ITriggerable::Execute_Trigger(TriggerableObject);
			}
		}
	}
}

void AInteractableActor_Trigger::GetInteractedWith_Implementation(AEternalGrace_ProtoCharacter* InteractingPlayer)
{
	if (!bCanbeActivated)
	{
		UE_LOG(LogTemp, Error, TEXT("%s can not be Activated anymore"), *GetFName().ToString());
		return;
	}
	if (RequiredKeyObjectID != "None")
	{
		bool bHasPlayerKey = InteractingPlayer->GetInventory()->KeyItemInventoryMap.Contains(RequiredKeyObjectID);
		if (bHasPlayerKey)
		{
			bCanbeActivated = false;
			TriggerRelatedObject();
			InteractingPlayer->GetInventory()->KeyItemInventoryMap.Remove(RequiredKeyObjectID);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Missing required Key"));
		}
	}
}
