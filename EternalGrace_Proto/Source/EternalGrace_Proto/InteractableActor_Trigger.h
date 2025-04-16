// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "InteractableActor_Trigger.generated.h"

/**
 * 
 */
UCLASS()
class ETERNALGRACE_PROTO_API AInteractableActor_Trigger : public AInteractableBase
{
	GENERATED_BODY()

	AInteractableActor_Trigger();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	FName TriggerableObjectID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	FName RequiredKeyObjectID;

public:
	UFUNCTION()
	void TriggerRelatedObject();

	virtual void GetInteractedWith_Implementation(AEternalGrace_ProtoCharacter* InteractingPlayer)override;
};
