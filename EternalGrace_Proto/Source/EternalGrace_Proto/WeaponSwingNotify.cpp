// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSwingNotify.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Attackable.h"
#include "Components/CapsuleComponent.h"
#include "TraceUtils.h"
#include "NiagaraFunctionLibrary.h"

void UWeaponSwingNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!IgnoreList.IsEmpty())
	{
		IgnoreList.Empty();
	}

	AttackingActor = MeshComp->GetOwner();
	if (AttackingActor)
	{
		if (AttackingActor->Implements<UAttackable>())
		{
			AttackingWeapon = IAttackable::Execute_GetWeapon(AttackingActor);
			if (AttackingWeapon)
			{
				WeaponElement = AttackingWeapon->ElementalType;
				Hitbox = AttackingWeapon->GetHitbox();
				if (!Hitbox)
				{
					UE_LOG(LogTemp, Error, TEXT("(WeaponSwingNotify) failed to get Hitbox from %s"), *AttackingActor->GetFName().ToString())
						return;
				}
				HittableObjectTypes = IAttackable::Execute_GetHittableObjectTypes(AttackingActor);

				HitCapsuleRadius = Hitbox->GetScaledCapsuleRadius();
				HitCapsuleHalfHeight = Hitbox->GetScaledCapsuleHalfHeight();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("(WeaponSwingNotify) Coult not get Weapon from Attacker!"));
			}
		}
	}
	else //redundant.
	{
		UE_LOG(LogTemp, Error, TEXT("(WeaponSwingNotify) failed to get Actor"));
	}

}

void UWeaponSwingNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (Hitbox)
	{
		FVector CapsulePosition = Hitbox->GetComponentLocation();
		FRotator CapsuleRotation = Hitbox->GetComponentRotation();
		IgnoreList.Add(AttackingActor);
		TArray<FHitResult> Hitted;
		UTraceUtils::CapsuleTraceMultiForObjects(AttackingActor->GetWorld(), CapsulePosition, CapsulePosition, HitCapsuleRadius, HitCapsuleHalfHeight, CapsuleRotation, HittableObjectTypes, false, IgnoreList, EDrawDebugTrace::ForDuration, Hitted, true);

		for (FHitResult Hit : Hitted)
		{
			UE_LOG(LogTemp, Display, TEXT("Hitted %s"), *Hit.GetActor()->GetFName().ToString())
				IgnoreList.Add(Hit.GetActor());

			UNiagaraSystem* HitEffectToSpawn = *HitReactionMap.Find(WeaponElement);
			if (HitEffectToSpawn)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(AttackingActor->GetWorld(), HitEffectToSpawn, Hit.Location, FRotator(Hit.ImpactNormal.X, Hit.ImpactNormal.Y, Hit.ImpactNormal.Z));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Effect for this ElementType"));
			}
		}
	}
}

void UWeaponSwingNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!IgnoreList.IsEmpty())
	{
		IgnoreList.Empty();
	}
}
