// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Components/AudioComponent.h"
#include "ValueBarWidgetBase.h"
#include "Staggerable.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	HitSoundComponent = CreateDefaultSubobject<UAudioComponent>("HitSoundComponent");
	HitSoundComponent->SetAutoActivate(false);

	MaxHealth = 1000.f;
	CurrentHealth = MaxHealth;
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

void UHealthComponent::GetDamage(AActor* Attacker, float DamageValue,float PoiseDamage, EAttackDirection Direction)
{
	AActor* Owner = GetOwner();
	CurrentHealth -= DamageValue;
	UE_LOG(LogTemp, Error, TEXT("Current Health is %f, DamageIncome was %f"), CurrentHealth, DamageValue)
	HitSoundComponent->Play();
	if(Owner->Implements<UStaggerable>())
	{
		IStaggerable::Execute_Stagger(Owner, Direction, PoiseDamage, Attacker);
	}
}

void UHealthComponent::ShowHPBar(APlayerController* PlayerController)
{
	if(HPBarClass)
	{
		HPBar = CreateWidget<UValueBarWidgetBase>(PlayerController, HPBarClass);
		if(HPBar)
		{
			HPBar->AddToPlayerScreen();
		}
	}
}

UValueBarWidgetBase* UHealthComponent::GetHPBar()
{
	return HPBar;
}

void UHealthComponent::UpdateHPBar()
{
	UE_LOG(LogTemp, Error, TEXT("HP Gets Updated"))
	HPBar->UpdateProgressBar(CurrentHealth, MaxHealth);
}

