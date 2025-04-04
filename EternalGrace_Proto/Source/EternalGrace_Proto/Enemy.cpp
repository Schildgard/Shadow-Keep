// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

AEnemy::AEnemy()
{
	bIsHostile = true;
	Tags.Add("Enemy");
	Tags.Add("Targetable");
}

void AEnemy::NoticePlayer(APawn* SpottedPawn)
{
	Super::NoticePlayer(SpottedPawn);
}
