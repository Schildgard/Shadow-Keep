// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenChestNotify.h"
#include "TreasureChest.h"

void UOpenChestNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ATreasureChest* TreasureChest = Cast<ATreasureChest>(MeshComp->GetOwner());
	if(TreasureChest)
	{
		TreasureChest->GetAllTreasure();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OpenChestNotify could not Cast its MeshComp into Treasure Chest"));
	}
}
