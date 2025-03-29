// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementalType.h"
#include "NiagaraSystem.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "WeaponSwingNotify.generated.h"

/**
 * 
 */
class AWeaponBase;
class UCapsuleComponent;
class AEternalGrace_CharacterBase;
UCLASS(Blueprintable)
class ETERNALGRACE_PROTO_API UWeaponSwingNotify : public UAnimNotifyState
{
	GENERATED_BODY()

	UWeaponSwingNotify();

	 void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)override;
	 void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)override;
	 void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)override;

	 UPROPERTY()
	 AActor* AttackingActor;
	 UPROPERTY()
	 AWeaponBase* AttackingWeapon;
	 UPROPERTY()
	 EElementalType WeaponElement;
	 UPROPERTY()
	 UWorld* World;
	 UPROPERTY()
	 UAudioComponent* AudioComponent;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OnHit", meta = (AllowPrivateAccess))
	 bool bIsOffHandWeapon;

	 UPROPERTY()
	 UCapsuleComponent* Hitbox;
	 float HitCapsuleRadius;
	 float HitCapsuleHalfHeight;

	 UPROPERTY()
	 TArray<TEnumAsByte<EObjectTypeQuery>> HittableObjectTypes;
	 UPROPERTY()
	 TArray<AActor*> IgnoreList;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OnHit", meta =(AllowPrivateAccess))
	 TMap<EElementalType, UNiagaraSystem*> HitReactionMap;
	
};
