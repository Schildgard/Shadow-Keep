// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "WeaponBase.h"
#include "ShieldBase.h"
#include "ShieldComponent.h"
#include "EternalGrace_SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet//KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "EternalGrace_GameInstance.h"
#include "HealthComponent.h"
#include "ValueBarWidgetBase.h"
#include "NiagaraFunctionLibrary.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	SaveGameObject = nullptr;
	GroundSurfaceDataTable = nullptr;
	CurrentActionState = EActionState::Idle;

	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>("FaceComp");
	HeadMesh->SetupAttachment(GetMesh());
	HairComponent = CreateDefaultSubobject<UGroomComponent>("HairComp");
	HairComponent->SetupAttachment(HeadMesh, "head");
	BeardComponent = CreateDefaultSubobject<UGroomComponent>("BeardComp");
	BeardComponent->SetupAttachment(HeadMesh);
	MustacheComponent = CreateDefaultSubobject<UGroomComponent>("MustacheComp");
	MustacheComponent->SetupAttachment(HeadMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->GetHitSoundComponent()->SetupAttachment(GetMesh());

	VoiceComponent = CreateDefaultSubobject<UAudioComponent>("Voice");
	VoiceComponent->SetupAttachment(GetMesh(), "head");

	ShieldComponent = CreateDefaultSubobject<UShieldComponent>("Shield Component");
	ShieldComponent->SetupAttachment(GetMesh(), "s_hand_l");
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	World = GetWorld();

	UGameInstance* GameInstanceReference = UGameplayStatics::GetGameInstance(GetWorld());
	CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GameInstanceReference);

	SaveGameObject = CurrentGameInstance->GetCurrentSaveGame();

	if(ShieldComponent && ShieldComponent->GetCurrentShieldClass())
	{
		ShieldComponent->EquipShield(ShieldComponent->GetCurrentShieldClass());
	}

}

void ACharacterBase::SaveData_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("%s Saves its Data"), *GetName())
}

void ACharacterBase::LoadData_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("%s Loads its Data"), *GetName())
}

void ACharacterBase::MakeSaveCall()
{
	if (CurrentGameInstance)
	{
		CurrentGameInstance->RequestSave();
	}
}

void ACharacterBase::MakeLoadCall()
{
	if (CurrentGameInstance)
	{
		CurrentGameInstance->RequestLoad();
	}
}

UCapsuleComponent* ACharacterBase::GetHitBox_Implementation()
{
	return nullptr;
}

AWeaponBase* ACharacterBase::GetWeapon_Implementation()
{
	return nullptr;
}
AWeaponBase* ACharacterBase::GetOffhandWeapon_Implementation()
{
	return nullptr;
}
AShieldBase* ACharacterBase::GetShield_Implementation()
{
	return ShieldComponent->GetCurrentShieldObject();
}
void ACharacterBase::Attack_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("%s perfoms an Attack"), *GetFName().ToString())
}

void ACharacterBase::GetDamage_Implementation(AActor* Attacker, float DamageValue, float PoiseDamage, EAttackDirection Direction, FVector HitLocation, FRotator HitRotation)
{
	//Check if here if Damage goes through Block or something
	if (CurrentActionState == EActionState::Guarding && Direction == EAttackDirection::Front)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Blocks Attack"), *GetFName().ToString())

		float ReducedDamage = DamageValue - ((DamageValue / 100) * ShieldComponent->BlockValue);
		HealthComponent->CurrentHealth -= ReducedDamage;
		GetMesh()->GetAnimInstance()->Montage_Play(ShieldComponent->GetNormalBlockAnimation());
		UGameplayStatics::SpawnSoundAtLocation(World, ShieldComponent->GetBlockSound(), HitLocation, HitRotation);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, ShieldComponent->GetBlockEffect(), HitLocation, HitRotation);
	}
	else
	{
		HealthComponent->GetDamage(Attacker, DamageValue, PoiseDamage, Direction, HitLocation, HitRotation);
	}
	HealthComponent->UpdateHPBar();

	if (HealthComponent->CurrentHealth <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("%s call Die Implementation"), *GetFName().ToString())
			HealthComponent->CurrentHealth = 0;
		IDamageable::Execute_Die(this);
	}
}

UNiagaraSystem* ACharacterBase::GetHitEffectSystem_Implementation()
{
	return HealthComponent->GetHitEffect();
}

UAudioComponent* ACharacterBase::GetHitSoundComponent_Implementation()
{
	return HealthComponent->GetHitSoundComponent();
}

void ACharacterBase::Die_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("%s is defeated"), *GetFName().ToString())
		if (HealthComponent && HealthComponent->GetDeathAnimation())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(HealthComponent->GetDeathAnimation());
		}
}

TArray<TEnumAsByte<EObjectTypeQuery>> ACharacterBase::GetHittableObjectTypes_Implementation()
{
	return HittableObjectTypes;
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

EActionState ACharacterBase::GetCurrentActionState()
{
	return CurrentActionState;
}

void ACharacterBase::CancelGuard()
{
	if (CurrentActionState == EActionState::Guarding)
	{
		CurrentActionState = EActionState::Idle;
	}
	else return;
}

void ACharacterBase::SetCurrentActionState(EActionState ActionState)
{
	CurrentActionState = ActionState;
	//Also Update ActionState of AnimInstance
}

void ACharacterBase::PlayFootStepSound(FName FootSocket)
{
	UE_LOG(LogTemp, Warning, TEXT("No override for PlayFootStepSound (Character Base Class) %s"), *GetFName().ToString())
}

UPhysicalMaterial* ACharacterBase::GetGroundMaterial()
{
	FVector ActorLocation = GetActorLocation();
	FVector ScanDirection = FVector(ActorLocation.X, ActorLocation.Y, ActorLocation.Z - GroundScanDistance);
	FHitResult OutHit;;

	bool Hit = UKismetSystemLibrary::LineTraceSingle(World, ActorLocation, ScanDirection, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, {}, EDrawDebugTrace::None, OutHit, true);

	if (!Hit)
	{
		return nullptr;
	}
	UPhysicalMaterial* HitMaterial = OutHit.PhysMaterial.Get();
	return HitMaterial;
}

void ACharacterBase::RotateTowardsTarget(AActor* Target)
{
	//GET LOOK ROTATION BETWEEN PLAYER AND TARGET
	FRotator Look = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
	//BREAK LOOK ROTATION TO YAW ONLY
	FRotator DesiredRotation = FRotator(0, Look.Yaw, 0);
	SetActorRotation(DesiredRotation);
}

UHealthComponent* ACharacterBase::GetHealthComponent()
{
	return HealthComponent;
}

