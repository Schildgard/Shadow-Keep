// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UNiagaraSystem;
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

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	UNiagaraSystem* GetHitEffect();
	UFUNCTION()
	UAudioComponent* GetHitSoundComponent();

		
};
