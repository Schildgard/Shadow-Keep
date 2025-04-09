// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class ETERNALGRACE_PROTO_API AEnemyControllerBase : public AAIController
{
	GENERATED_BODY()
protected:
	AEnemyControllerBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess))
	UBehaviorTree* BehaviourTree;

	void BeginPlay()override;

public:

	UFUNCTION()
	UBehaviorTree* GetBehaviorTree();
	
};
