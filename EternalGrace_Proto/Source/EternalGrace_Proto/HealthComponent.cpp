// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Components/AudioComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	HitSoundComponent = CreateDefaultSubobject<UAudioComponent>("HitSoundComponent");
	HitSoundComponent->SetAutoActivate(false);
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

UNiagaraSystem* UHealthComponent::GetHitEffect()
{
	return HitEffect;
}

UAudioComponent* UHealthComponent::GetHitSoundComponent()
{
	return HitSoundComponent;
}

