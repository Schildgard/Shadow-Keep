// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackDirection.h"
#include "StaggerComponent.generated.h"


class UCharacterAnimInstanceBase;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ETERNALGRACE_PROTO_API UStaggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStaggerComponent();

	UFUNCTION()
	bool GetStaggered(EAttackDirection Direction, float PoiseDamage, AActor* Instigator);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseValues, meta = (AllowPrivateAccess))
	float MaxPoise;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseValues, meta = (AllowPrivateAccess))
	TArray<UAnimMontage*> StaggerAnimations;

	UPROPERTY()
	UCharacterAnimInstanceBase* CharacterAnimInstance;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseValues, meta = (AllowPrivateAccess))
	float CurrentPoise;

	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
