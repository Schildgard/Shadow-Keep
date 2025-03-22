// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DefaultGameMode.generated.h"

/**
 * 
 */
class AEternalGrace_ProtoCharacter;
class AEG_PlayerController;
UCLASS()
class ETERNALGRACE_PROTO_API ADefaultGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	ADefaultGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Players", meta = (AllowPrivateAccess))
	TSubclassOf<AEternalGrace_ProtoCharacter> Player1Character;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Players", meta = (AllowPrivateAccess))
	TSubclassOf<AEternalGrace_ProtoCharacter> Player2Character;
	

	void BeginPlay()override;
};
