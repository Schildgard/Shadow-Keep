// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"

AEnemyControllerBase::AEnemyControllerBase()
{
}

void AEnemyControllerBase::BeginPlay()
{
	Super::BeginPlay();
	if (BehaviourTree)
	{
		RunBehaviorTree(BehaviourTree);
		
	}
}
