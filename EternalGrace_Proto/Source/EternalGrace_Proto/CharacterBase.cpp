// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "WeaponBase.h"
#include "EternalGrace_SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet//KismetSystemLibrary.h"
#include "EternalGrace_GameInstance.h"
#include "HealthComponent.h"

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
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	World = GetWorld();

	UGameInstance* GameInstanceReference = UGameplayStatics::GetGameInstance(GetWorld());
	CurrentGameInstance = Cast<UEternalGrace_GameInstance>(GameInstanceReference);

	SaveGameObject = CurrentGameInstance->GetCurrentSaveGame();
	
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
	if(CurrentGameInstance)
	{
		CurrentGameInstance->RequestSave();
	}
}

void ACharacterBase::MakeLoadCall()
{
	if(CurrentGameInstance)
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

void ACharacterBase::GetDamage_Implementation()
{
}

UNiagaraSystem* ACharacterBase::GetHitEffectSystem_Implementation()
{
	return nullptr;
}

UAudioComponent* ACharacterBase::GetHitSoundComponent_Implementation()
{
	return nullptr;
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

