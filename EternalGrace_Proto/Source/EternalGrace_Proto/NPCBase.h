// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Aggroable.h"
#include "NPCSaveDataInfoBase.h"
#include "NPCBase.generated.h"

/**
 * 
 */
class AAIController;
class UWeaponComponent;
class UBlackboardComponent;
class UPawnSensingComponent;
class AEnemyControllerBase;
UCLASS()
class ETERNALGRACE_PROTO_API ANPCBase : public ACharacterBase, public IAggroable
{
	GENERATED_BODY()

protected:
	ANPCBase();

	void BeginPlay()override;
	void Tick(float DeltaSeconds)override;


	//PLAYER DETECTION
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess))
	UPawnSensingComponent* SensingComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess))
	AEnemyControllerBase* AIController;
	UPROPERTY()
	bool bIsHostile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, meta = (AllowPrivateAccess))
	float AttackRange;

	float StopChasingTimer;
	const float MaxChasingTime = 15.0f;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, meta = (AllowPrivateAccess))
	TArray<UAnimMontage*> NormalAttackArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, meta = (AllowPrivateAccess))
	TMap<AActor*, float> AggroList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, meta = (AllowPrivateAccess))
	UWeaponComponent* WeaponComponent;

	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY()
	ACharacter* NoticedPlayer;

	UPROPERTY()
	UAnimInstance* AnimationInstance;

	UPROPERTY()
	bool bIsAlive;

	UFUNCTION()
	virtual void NoticePlayer(APawn* SpottedPawn);
	UFUNCTION()
	float CheckDistanceToPlayer();
	UFUNCTION()
	void MakeEnemyCeasyFromChasing(float DeltaSeconds);

	virtual void Attack_Implementation()override;

	virtual void SaveData_Implementation()override;
	virtual void LoadData_Implementation()override;
	virtual void GetDamage_Implementation(AActor* Attacker, float DamageValue, float PoiseDamage, EAttackDirection Direction, FVector HitLocation, FRotator HitRotation)override;

	//In Case for Delegate
	UFUNCTION()
	void ResetAttackState(UAnimMontage* AttackAnimation, bool Interrupted);

	UPROPERTY()
	FNPCSaveDataInfoBase SaveDataInfo;
public:
	//Test for Blueprint


	virtual void RaiseAggro_Implementation(AActor* Attacker, float AggroValue);
	virtual void Die_Implementation()override;
	
};
