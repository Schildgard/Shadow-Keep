// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "InteractableActor_Dynamic.generated.h"

/**
 * 
 */
class USkeletalMeshComponent;
UCLASS()
class ETERNALGRACE_PROTO_API AInteractableActor_Dynamic : public AInteractableBase
{
	GENERATED_BODY()
	
protected:
	AInteractableActor_Dynamic();
	UPROPERTY(EditAnywhere,BlueprintReadOnly, meta =(AllowPrivateAccess))
	USkeletalMeshComponent* Mesh;

	virtual void GetInteractedWith_Implementation(AEternalGrace_ProtoCharacter* InteractingPlayer)override;
};
