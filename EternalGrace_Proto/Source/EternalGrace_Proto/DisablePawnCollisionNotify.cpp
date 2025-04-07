// Fill out your copyright notice in the Description page of Project Settings.


#include "DisablePawnCollisionNotify.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

void UDisablePawnCollisionNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ACharacter* PerformingActor = Cast <ACharacter>(MeshComp->GetOwner());
	if (PerformingActor)
	{
		UCapsuleComponent* ActorCollisionCapsule = PerformingActor->GetCapsuleComponent();
		if (ActorCollisionCapsule)
		{
			ActorCollisionCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		}
	}
}

void UDisablePawnCollisionNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UDisablePawnCollisionNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	ACharacter* PerformingActor = Cast <ACharacter>(MeshComp->GetOwner());
	if (PerformingActor)
	{
		UCapsuleComponent* ActorCollisionCapsule = PerformingActor->GetCapsuleComponent();
		if (ActorCollisionCapsule)
		{
			ActorCollisionCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		}
	}
}
