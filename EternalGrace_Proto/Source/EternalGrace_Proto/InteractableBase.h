// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Playable.h"
#include "InteractableBase.generated.h"

class USphereComponent;
class AEternalGrace_ProtoCharacter;
UCLASS()
class ETERNALGRACE_PROTO_API AInteractableBase : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AInteractableBase();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, meta =(AllowPrivateAccess))
	bool bCanbeActivated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	USphereComponent* InteractionSphere;

	UPROPERTY()
	float SphereRadius;

	UFUNCTION()
	virtual void RegisterInteractionRadiusEntry(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	UFUNCTION()
	virtual void RegisterInteractionRadiusLeave(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex
	);



public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void GetInteractedWith_Implementation(AEternalGrace_ProtoCharacter* InteractingPlayer)override;

};
