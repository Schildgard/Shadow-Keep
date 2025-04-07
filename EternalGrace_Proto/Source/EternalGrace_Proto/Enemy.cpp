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

void AEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateHealthbarPosition();
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
		if (bCharacterGetStaggered == true)
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
	//Get Damage
	Super::GetDamage_Implementation(Attacker, DamageValue, PoiseDamage, AttackDirection);
	//Check Which Player did the Damage, if it was a Player at all
	AEternalGrace_ProtoCharacter* PlayerChar = Cast<AEternalGrace_ProtoCharacter>(Attacker);
	if (PlayerChar)
	{
		int PlayerIndex = PlayerChar->GetPlayerIndex();
		//Check if The Player already sees the enemies HP Bar
		bool bIsHpBarInstanceActive;
		if (PlayerIndex == 0) //If Player One
		{
			bIsHpBarInstanceActive = TemporaryHPBarInstance1 != nullptr;
		}
		else //If Player Two
		{
			bIsHpBarInstanceActive = TemporaryHPBarInstance2 != nullptr;
		}

		//If not show HPBar to Player
		if (!bIsHpBarInstanceActive)
		{
			if (PlayerIndex == 0)
			{
				TemporaryHPBarInstance1 = ShowTemporaryBar(0);
			}
			else if (PlayerIndex == 1)
			{
				TemporaryHPBarInstance2 = ShowTemporaryBar(1);
			}
		}
		UpdateTemporaryHPBarValues();
	}

}

UValueBarWidgetBase* AEnemy::ShowTemporaryBar(int PlayerIndex)
{

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex);
	if (PlayerController)
	{
		UValueBarWidgetBase* TemporaryHPBarInstance;
		TSubclassOf<UValueBarWidgetBase> HPBarClass = HealthComponent->GetHPBarClass();
		if (HPBarClass)
		{
			TemporaryHPBarInstance = CreateWidget<UValueBarWidgetBase>(PlayerController, HPBarClass);
			if (TemporaryHPBarInstance)
			{
				TemporaryHPBarInstance->AddToPlayerScreen();
				return TemporaryHPBarInstance;
			}
		}
	}
	return nullptr;
}

void AEnemy::UpdateTemporaryHPBarValues()
{
	if(TemporaryHPBarInstance1)
	{
		TemporaryHPBarInstance1->UpdateProgressBar(HealthComponent->CurrentHealth, HealthComponent->GetMaxHealth());
	}
	if(TemporaryHPBarInstance2)
	{
		TemporaryHPBarInstance2->UpdateProgressBar(HealthComponent->CurrentHealth, HealthComponent->GetMaxHealth());
	}
}

void AEnemy::UpdateHealthbarPosition()
{
	FVector SocketPosition = GetMesh()->GetSocketLocation("s_healthbar");
	FVector2D Screen1Position;
	FVector2D Screen2Position;

	if (TemporaryHPBarInstance1)
	{
		bool Projected1 = UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 0),SocketPosition, Screen1Position, true);
		if(Projected1)
		{
			TemporaryHPBarInstance1->SetPositionInViewport(Screen1Position);
		}
	}
	if (TemporaryHPBarInstance2)
	{
		bool Projected2 = UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 1), SocketPosition, Screen2Position, true);
		if (Projected2)
		{
			TemporaryHPBarInstance2->SetPositionInViewport(Screen2Position);
		}
	}
}
