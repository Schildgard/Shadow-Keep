// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ShieldComponent.generated.h"

class AShieldBase;
class ACharacterBase;
class UNiagaraSystem;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ETERNALGRACE_PROTO_API UShieldComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShieldComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<AShieldBase> CurrentShieldClass;
	UPROPERTY()
	AShieldBase* CurrentShieldObject;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	USoundBase* BlockSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UNiagaraSystem* BlockEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TArray<UAnimMontage*> BlockAnimations;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	UAnimMontage* ShieldBashAttack;

	UPROPERTY()
	ACharacterBase* OwningCharacter;




public:	
	float BlockValue;
	float Balance;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION()
	void EquipShield(TSubclassOf<AShieldBase> ShieldToEquip);
	UFUNCTION()
	USoundBase* GetBlockSound();
	UFUNCTION()
	UNiagaraSystem* GetBlockEffect();
	UFUNCTION()
	UAnimMontage* GetNormalBlockAnimation();
	UFUNCTION()
	TSubclassOf<AShieldBase> GetCurrentShieldClass();
	UFUNCTION()
	AShieldBase* GetCurrentShieldObject();
	UFUNCTION()
	UAnimMontage* GetShieldAttack();

//	UFUNCTION()
//	void PlayBlockAnimation()

		
};
