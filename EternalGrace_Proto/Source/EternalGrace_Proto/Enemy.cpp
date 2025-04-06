// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "WeaponComponent.h"
#include "StaggerComponent.h"

AEnemy::AEnemy()
{
	bIsHostile = true;
	Tags.Add("Enemy");
	Tags.Add("Targetable");
	StaggerComponent = CreateDefaultSubobject<UStaggerComponent>("StaggerSystem");
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponComponent->CurrentWeaponClass)
	{
		WeaponComponent->ChangeWeapon(WeaponComponent->CurrentWeaponClass);
	}
}

void AEnemy::NoticePlayer(APawn* SpottedPawn)
{
	if (!NoticedPlayer && NoticePlayerSound)
	{
		VoiceComponent->SetSound(NoticePlayerSound);
		VoiceComponent->Play();
	}
	Super::NoticePlayer(SpottedPawn);
}

AWeaponBase* AEnemy::GetWeapon_Implementation()
{
	return WeaponComponent->CurrentWeaponObject;
}

void AEnemy::Stagger_Implementation(EAttackDirection Direction, float PoiseDamage, AActor* DamageInstigator)
{
	if (StaggerComponent)
	{
		bool bCharacterGetStaggered = StaggerComponent->GetStaggered(Direction, PoiseDamage, DamageInstigator);
		if(bCharacterGetStaggered == true)
		{
			SetCurrentActionState(EActionState::Staggered);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no StaggerComponent! (Enemy Class)"), *GetFName().ToString());
	}
}
