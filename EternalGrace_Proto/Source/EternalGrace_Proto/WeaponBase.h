// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponType.h"
#include "ObjectType.h"
#include "ElementalType.h"
#include "WeaponBase.generated.h"

class UCapsuleComponent;
class UCameraShakeSourceComponent;
UCLASS()
class ETERNALGRACE_PROTO_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TArray<UAnimMontage*> RegularAttackMontages;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UAnimMontage* OffhandAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UAnimMontage* RunningAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UAnimMontage* DodgeAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UCapsuleComponent* Hitbox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UCameraShakeSourceComponent* CameraShakeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	FName WeaponName;



public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float ImpactForce;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float PhysicalDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float PoiseDamage;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSoftObjectPtr<UTexture2D> ThumpnailImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(AllowPrivateAccess))
	UStaticMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	EWeaponType WeaponCategory;
	UPROPERTY()
	EElementalType ElementalType;
	UPROPERTY()
	EObjectType ObjectType;
	UFUNCTION()
	TArray<UAnimMontage*> GetNormalAttacks();

	UFUNCTION()
	UAnimMontage* GetOffhandAttack();

	UFUNCTION()
	UAnimMontage* GetRunningAttack();

	UFUNCTION()
	UAnimMontage* GetDodgeAttack();

	UFUNCTION()
	UCapsuleComponent* GetHitbox();

	UFUNCTION()
	FTransform GetSocket(FName SocketName);

	UFUNCTION()
	UAudioComponent* GetAudioComponent();

	UFUNCTION()
	UCameraShakeSourceComponent* GetCameraShakeComponent();

	UFUNCTION()
	FName GetWeaponName();

};
