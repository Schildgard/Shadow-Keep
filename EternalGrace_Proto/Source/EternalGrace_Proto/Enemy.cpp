// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "WeaponComponent.h"
#include "StaggerComponent.h"
#include "Components/WidgetComponent.h"
#include "HealthComponent.h"
#include "ValueBarWidgetBase.h"
#include "EternalGrace_ProtoCharacter.h"
#include "Kismet/GameplayStatics.h"

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

void AEnemy::GetDamage_Implementation(AActor* Attacker, float DamageValue, float PoiseDamage, EAttackDirection AttackDirection)
{
	Super::GetDamage_Implementation(Attacker, DamageValue, PoiseDamage, AttackDirection);

	AEternalGrace_ProtoCharacter* PlayerChar = Cast<AEternalGrace_ProtoCharacter>(Attacker);
	if(PlayerChar)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerChar->GetPlayerIndex());
		if(PlayerController)
		{
			HealthComponent->ShowEnemyHPBar(PlayerController);

		}

	}

//	//Show HP Bar
//	HealthComponent->ShowHPBar();

}
