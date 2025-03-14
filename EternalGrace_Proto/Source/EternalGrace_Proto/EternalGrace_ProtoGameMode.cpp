// Copyright Epic Games, Inc. All Rights Reserved.

#include "EternalGrace_ProtoGameMode.h"
#include "EternalGrace_ProtoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEternalGrace_ProtoGameMode::AEternalGrace_ProtoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
