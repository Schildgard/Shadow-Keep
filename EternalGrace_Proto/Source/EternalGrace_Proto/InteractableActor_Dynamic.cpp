// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor_Dynamic.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SphereComponent.h"

AInteractableActor_Dynamic::AInteractableActor_Dynamic()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SetRootComponent(Mesh);
	InteractionSphere->SetupAttachment(RootComponent);
}

void AInteractableActor_Dynamic::GetInteractedWith_Implementation(AEternalGrace_ProtoCharacter* InteractingPlayer)
{
	Super::GetInteractedWith_Implementation(InteractingPlayer);
	if (Mesh->AnimationData.AnimToPlay)
	{
		Mesh->Play(false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no Animation assigned"), *GetFName().ToString());
	}
}
