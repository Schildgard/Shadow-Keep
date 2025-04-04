// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSwingNotify.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Attackable.h"
#include "Components/CapsuleComponent.h"
#include "TraceUtils.h"
#include "Components/AudioComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Damageable.h"
#include "Camera/CameraShakeSourceComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"


UWeaponSwingNotify::UWeaponSwingNotify()
{
	AttackingActor = nullptr;
	AttackingWeapon = nullptr;
	WeaponElement = EElementalType::Physical;
	World = nullptr;
	AudioComponent = nullptr;
	bIsOffHandWeapon = false;
	Hitbox = nullptr;
	HitCapsuleHalfHeight = 0.0f;
	HitCapsuleRadius = 0.0f;

}

void UWeaponSwingNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!IgnoreList.IsEmpty())
	{
		IgnoreList.Empty();
	}

	// Save Attacker in Variable
	AttackingActor = MeshComp->GetOwner();
	if (AttackingActor)
	{
		// World reference for Hittrace, niagara and sound spawning
		World = AttackingActor->GetWorld();
		//Check Interface to get Attack Properties
		if (AttackingActor->Implements<UAttackable>())
		{
			if (bIsOffHandWeapon) //Is setted in Editor of the Montage Notify
			{
				AttackingWeapon = IAttackable::Execute_GetOffhandWeapon(AttackingActor);
			}
			else
			{
				AttackingWeapon = IAttackable::Execute_GetWeapon(AttackingActor);
			}
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


}

void UWeaponSwingNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (Hitbox)
	{
		//Prepare Hit Detection via Capsule Trace
		FVector CapsulePosition = Hitbox->GetComponentLocation();
		FRotator CapsuleRotation = Hitbox->GetComponentRotation();
		IgnoreList.Add(AttackingActor);
		TArray<FHitResult> Hitted;
		UTraceUtils::CapsuleTraceMultiForObjects(World, CapsulePosition, CapsulePosition, HitCapsuleRadius, HitCapsuleHalfHeight, CapsuleRotation, HittableObjectTypes, false, IgnoreList, EDrawDebugTrace::ForDuration, Hitted, true);

		//On Hit, Add to Ignore List, Spawn Niagara and Weapon Hit Sound Effect
		for (const FHitResult& Hit : Hitted)
		{
			AActor* HittedActor = Hit.GetActor();
			if (!HittedActor)return;
			UE_LOG(LogTemp, Display, TEXT("Hitted %s"), *HittedActor->GetFName().ToString())

				IgnoreList.Add(HittedActor);

			UNiagaraSystem* HitNiagara;

			//Check if Hitted Actor Implements Interface. Determine wether to use weapons environmental hit properties or hittable targets properties.
			if (HittedActor->Implements<UDamageable>())
			{
				HitNiagara = IDamageable::Execute_GetHitEffectSystem(HittedActor);
				AudioComponent = AttackingWeapon->GetAudioComponent();
				IDamageable::Execute_GetDamage(HittedActor, AttackingActor, AttackingWeapon->ImpactForce, Hit.Location);
			}
			else
			{
				HitNiagara = *HitReactionMap.Find(WeaponElement); // TO Do: Change the way to determine the hit effect..maybe datatables ?
				AudioComponent = AttackingWeapon->GetAudioComponent();
			}

			//Play Audio and Niagara
			if (HitNiagara)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, HitNiagara, Hit.Location, FRotator(Hit.ImpactNormal.X, Hit.ImpactNormal.Y, Hit.ImpactNormal.Z));
			}
			if (AudioComponent)
			{
				AudioComponent->Play();
			}

			if (*AttackingWeapon->GetCameraShakeComponent()->CameraShake)
			{
				AttackingWeapon->GetCameraShakeComponent()->Start();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No Camera Shake (WeaponSwingNotify)"));
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
