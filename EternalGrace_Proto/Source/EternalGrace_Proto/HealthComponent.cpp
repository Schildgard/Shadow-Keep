// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Components/AudioComponent.h"
#include "ValueBarWidgetBase.h"
#include "Staggerable.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

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

	OwningCharacter = Cast<ACharacter>(GetOwner());

	//	if (HPBarClass)
	//	{
	//		HPBar = CreateWidget<UValueBarWidgetBase>(GetWorld(), HPBarClass);
	//	}

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

void UHealthComponent::GetDamage(AActor* Attacker, float DamageValue, float PoiseDamage, EAttackDirection Direction)
{
	//Not Using OwningCharacter here, because of no particular reason. Might as well change this
	AActor* Owner = GetOwner(); 
	CurrentHealth -= DamageValue;
		HitSoundComponent->Play();
	if (OwningCharacter->Implements<UStaggerable>())
	{
		IStaggerable::Execute_Stagger(Owner, Direction, PoiseDamage, Attacker);
	}
}

void UHealthComponent::ShowHPBar(APlayerController* PlayerController)
{
	if (HPBarClass && !HPBar && PlayerController)
	{
		HPBar = CreateWidget<UValueBarWidgetBase>(PlayerController, HPBarClass);

	}
	if (HPBar)
	{
		HPBar->AddToPlayerScreen();
	}
}

UValueBarWidgetBase* UHealthComponent::GetHPBar()
{
	return HPBar;
}

TSubclassOf<UValueBarWidgetBase>& UHealthComponent::GetHPBarClass()
{
	return HPBarClass;
}

void UHealthComponent::UpdateHPBar()
{
	if (HPBar)
	{
		HPBar->UpdateProgressBar(CurrentHealth, MaxHealth);
	}
}

float UHealthComponent::GetMaxHealth()
{
	return MaxHealth;
}

