// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_StrafeAroundTarget.generated.h"

/**
 * 
 */
class UCharacterAnimInstanceBase;
UCLASS()
class ETERNALGRACE_PROTO_API UBTTask_StrafeAroundTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_StrafeAroundTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//The sideward distance to left or right the character moves when strafing


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float StrafeDistance = 300.f;

	UPROPERTY()
	AAIController* AIController;
	UPROPERTY()
	ACharacter* AICharacter;

	UPROPERTY()
	UCharacterAnimInstanceBase* AnimInstance;
};
