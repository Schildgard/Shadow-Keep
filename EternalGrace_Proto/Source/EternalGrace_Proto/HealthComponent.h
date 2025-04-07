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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitReaction", meta = (AllowPrivateAccess))
	UNiagaraSystem* HitEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitReaction", meta = (AllowPrivateAccess))
	UAudioComponent* HitSoundComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess))
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess))
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess))
	TSubclassOf<UValueBarWidgetBase> HPBarClass;
	UPROPERTY()
	UValueBarWidgetBase* HPBar;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	UNiagaraSystem* GetHitEffect();
	UFUNCTION()
	UAudioComponent* GetHitSoundComponent();

	UFUNCTION()
	void GetDamage(AActor* Attacker, float DamageValue, float PoiseDamage, EAttackDirection Direction);

	UFUNCTION()
	void ShowHPBar();

	UFUNCTION()
	void ShowEnemyHPBar(APlayerController* PlayerController);

	UFUNCTION()
	UValueBarWidgetBase* GetHPBar();
	UFUNCTION()
	TSubclassOf<UValueBarWidgetBase>& GetHPBarClass();

	UFUNCTION()
	void UpdateHPBar();

		
};
