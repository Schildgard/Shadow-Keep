// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "DependendTriggerableActor.generated.h"

/**
 * 
 */
class UNiagaraComponent;
UCLASS()
class ETERNALGRACE_PROTO_API ADependendTriggerableActor : public AInteractableBase
{
	GENERATED_BODY()

	ADependendTriggerableActor();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TriggerRelation", meta = (AllowPrivateAccess))
	FName DependingObjectID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TriggerRelation", meta = (AllowPrivateAccess))
	FName ObjectID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TriggerRelation", meta = (AllowPrivateAccess))
	FName TriggerableObjectID; //In Case this actor triggers another actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UNiagaraComponent* NiagaraSystemComponent;


	void Tick(float DeltaSeconds)override;
	void BeginPlay()override;

	virtual void GetInteractedWith_Implementation(AEternalGrace_ProtoCharacter* InteractingPlayer)override;

	UFUNCTION()
	void ResetPuzzle();

	UFUNCTION()
	void Activate(UEternalGrace_GameInstance* GameInstance);
	
};
