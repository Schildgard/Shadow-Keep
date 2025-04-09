// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackDirection.h"
#include "HealthComponent.generated.h"

class UNiagaraSystem;
class UValueBarWidgetBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ETERNALGRACE_PROTO_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

	
	UPROPERTY()
	ACharacter* OwningCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitReaction", meta = (AllowPrivateAccess))
	UNiagaraSystem* HitEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitReaction", meta = (AllowPrivateAccess))
	UAudioComponent* HitSoundComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess))
	TSubclassOf<UValueBarWidgetBase> HPBarClass;
	UPROPERTY()
	UValueBarWidgetBase* HPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Health", meta = (AllowPrivateAccess))
	UAnimMontage* DeathAnimation;

	UPROPERTY()
	TArray<UValueBarWidgetBase*> TemporaryHPBars;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess))
	float CurrentHealth;

	UFUNCTION()
	UNiagaraSystem* GetHitEffect();
	UFUNCTION()
	UAudioComponent* GetHitSoundComponent();

	UFUNCTION()
	void GetDamage(AActor* Attacker, float DamageValue, float PoiseDamage, EAttackDirection Direction);

	UFUNCTION()
	void ShowHPBar(APlayerController* PlayerController);

	UFUNCTION()
	UValueBarWidgetBase* GetHPBar();
	UFUNCTION()
	TSubclassOf<UValueBarWidgetBase>& GetHPBarClass();

	UFUNCTION()
	void UpdateHPBar();

	UFUNCTION()
	float GetMaxHealth();

	UFUNCTION()
	UAnimMontage* GetDeathAnimation();

		
};
