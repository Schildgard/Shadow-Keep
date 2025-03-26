// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponType.h"
#include "ElementalType.h"
#include "WeaponBase.generated.h"

class UCapsuleComponent;
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
	UCapsuleComponent* Hitbox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UAudioComponent* AudioComponent;


public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(AllowPrivateAccess))
	UStaticMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	EWeaponType WeaponCategory;
	UPROPERTY()
	EElementalType ElementalType;
	UFUNCTION()
	TArray<UAnimMontage*> GetNormalAttacks();

	UFUNCTION()
	UAnimMontage* GetOffhandAttack();

	UFUNCTION()
	UCapsuleComponent* GetHitbox();

	UFUNCTION()
	FTransform GetSocket(FName SocketName);

	UFUNCTION()
	UAudioComponent* GetAudioComponent();

};
