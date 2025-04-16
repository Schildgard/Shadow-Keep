// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Triggerable.h"
#include "TriggerableActor.generated.h"

UCLASS()
class ETERNALGRACE_PROTO_API ATriggerableActor : public AActor, public ITriggerable
{
	GENERATED_BODY()
	
public:	
	ATriggerableActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	FName ObjectID;
public:	
	virtual void Tick(float DeltaTime) override;

	void Trigger_Implementation()override;

};
