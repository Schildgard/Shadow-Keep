// Fill out your copyright notice in the Description page of Project Settings.


#include "EG_AnimInstance.h"
#include "EternalGrace_ProtoCharacter.h"

UEG_AnimInstance::UEG_AnimInstance()
{
	WeaponClass = EWeaponType::NoWeapon;
	AttackCount = 0;
}

void UEG_AnimInstance::NativeBeginPlay()
{
	OwningCharacter = Cast<AEternalGrace_ProtoCharacter>(GetOwningActor());
	if(!OwningCharacter)
	{
		UE_LOG(LogTemp, Display, TEXT("Failed to Cast Character (EG_AnimInstance)"));
	}

}

void UEG_AnimInstance::SetWeaponType(EWeaponType NewWeaponClass)
{
	WeaponClass = NewWeaponClass;
}

void UEG_AnimInstance::ResetAttackState()
{
	AttackCount = 0;
	if(OwningCharacter)
	{
		OwningCharacter->SetCurrentActionState(EActionState::Idle);
	}

}
