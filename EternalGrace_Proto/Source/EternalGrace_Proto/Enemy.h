// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBase.h"
#include "Staggerable.h"
#include "AttackDirection.h"
#include "Enemy.generated.h"

/**
 * 
 */
class UStaggerComponent; // todo: Make that CharacterBase has a stagger component
UCLASS()
class ETERNALGRACE_PROTO_API AEnemy : public ANPCBase, public IStaggerable
{
	GENERATED_BODY()

protected:
	AEnemy();
	virtual void BeginPlay()override;
	virtual void NoticePlayer(APawn* SpottedPawn)override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess))
	USoundBase* NoticePlayerSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UStaggerComponent* StaggerComponent;

	virtual AWeaponBase* GetWeapon_Implementation()override;
	virtual void Stagger_Implementation(EAttackDirection Direction, float PoiseDamage, AActor* DamageInstigator)override;
	
};
