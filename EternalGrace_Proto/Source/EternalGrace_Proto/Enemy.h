// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBase.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class ETERNALGRACE_PROTO_API AEnemy : public ANPCBase
{
	GENERATED_BODY()

protected:
	AEnemy();
	void NoticePlayer(APawn* SpottedPawn)override;
	
};
