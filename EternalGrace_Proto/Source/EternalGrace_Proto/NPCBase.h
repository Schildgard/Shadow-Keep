// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Aggroable.h"
#include "NPCBase.generated.h"

/**
 * 
 */
class AAIController;
class UWeaponComponent;
class UBlackboardComponent;
class UPawnSensingComponent;
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
	AAIController* AIController;
	UPROPERTY()
	bool bIsHostile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, meta = (AllowPrivateAccess))
	float AttackRange;

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

	UFUNCTION()
	virtual void NoticePlayer(APawn* SpottedPawn);
	UFUNCTION()
	float CheckDistanceToPlayer();

	virtual void Attack_Implementation()override;

	//In Case for Delegate
	UFUNCTION()
	void ResetAttackState(UAnimMontage* AttackAnimation, bool Interrupted);
public:
	//Test for Blueprint
	UFUNCTION(BlueprintCallable)
	void ResetAttackState2();

	virtual void RaiseAggro_Implementation(AActor* Attacker, float AggroValue);
	
};
