// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBase.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "WeaponComponent.h"
#include "EnemyControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Saveable.h"
#include "EternalGrace_SaveGame.h"

ANPCBase::ANPCBase()
{
	AnimationInstance = nullptr;
	BlackboardComponent = nullptr;
	NoticedPlayer = nullptr;
	SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("Sensing");
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("Weapon");
	AIController = nullptr;
	bIsHostile = false;
	AttackRange = 150.f;
	bIsAlive = true;
	StopChasingTimer = MaxChasingTime;
}

void ANPCBase::BeginPlay()
{
	Super::BeginPlay();

	ISaveable::Execute_LoadData(this);
	bIsAlive = SaveDataInfo.bIsAlive;
	if (!bIsAlive)
	{
		UE_LOG(LogTemp, Error, TEXT("Destroy this NPC because it is deafeted"));
		Destroy();
		return;
	}


	SensingComponent->OnSeePawn.AddDynamic(this, &ANPCBase::NoticePlayer);

	AIController = Cast<AEnemyControllerBase>(GetController());
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy failed to get AI Controller(NPC Base Class)"));
		return;
	}
	BlackboardComponent = AIController->GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy failed to get BlackboardComponent (NPC Base Class)"));
		return;
	}
	AnimationInstance = GetMesh()->GetAnimInstance();
	if (!AnimationInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy failed to get AnimationInstance (NPC Base Class)"));
		return;
	}

	BlackboardComponent->SetValueAsVector("StartPosition", GetActorLocation());

}

void ANPCBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (NoticedPlayer && bIsHostile)
	{
		/*Check Distance to Player and Update IsInAttackRangeBool. If Enemy is too long out of reach, it returns to its Initial Position and leaves Battle*/
		float DistanceToTarget = CheckDistanceToPlayer();
		BlackboardComponent->SetValueAsBool("bIsInAttackRange", DistanceToTarget <= AttackRange);
		if(DistanceToTarget >= AttackRange)
		{
			MakeEnemyCeasyFromChasing(DeltaSeconds);
		}
		else if (StopChasingTimer < MaxChasingTime)
		{
			StopChasingTimer = MaxChasingTime;
		}
	}
}

void ANPCBase::NoticePlayer(APawn* SpottedPawn)
{
	if (bIsAlive && !NoticedPlayer)
	{
		NoticedPlayer = Cast<ACharacter>(SpottedPawn);
		if (NoticedPlayer)
		{
			BlackboardComponent->SetValueAsObject("TargetPlayer", NoticedPlayer);
		}
	}
}

float ANPCBase::CheckDistanceToPlayer()
{
	FVector ActorLocation = GetActorLocation();
	FVector TargetLocation = NoticedPlayer->GetActorLocation();
	FVector DistanceVector = TargetLocation - ActorLocation;
	float Distance = DistanceVector.Length();
	return Distance;
}

void ANPCBase::MakeEnemyCeasyFromChasing(float DeltaSeconds)
{
	StopChasingTimer -= DeltaSeconds;
	if (StopChasingTimer <= 0)
	{
		BlackboardComponent->SetValueAsObject("TargetPlayer", nullptr);
		AIController->SetFocus(nullptr);
	}
}

void ANPCBase::Attack_Implementation()
{
	Super::Attack_Implementation();
	BlackboardComponent->SetValueAsBool("bCanAttack", false);
	if(CurrentActionState == EActionState::Staggered)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy Attack failed because of Stagger %s"), *GetFName().ToString())
		return;
	}

	if (NormalAttackArray.Num() >= 1)
	{
		int AttackIndex = UKismetMathLibrary::RandomIntegerInRange(0, NormalAttackArray.Num() - 1);
		UE_LOG(LogTemp, Error, TEXT("Attack Index is %i"), AttackIndex)
			AnimationInstance->Montage_Play(NormalAttackArray[AttackIndex], true);

		//	FOnMontageEnded AttackEndDelegate;
		//	AttackEndDelegate.BindUObject(this, &ANPCBase::ResetAttackState);
		//	AnimationInstance->Montage_SetEndDelegate(AttackEndDelegate, NormalAttackArray[AttackIndex]);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Attacks assigned for %s"), *GetFName().ToString())
	}
}

void ANPCBase::SaveData_Implementation()
{
	Super::SaveData_Implementation();
	SaveGameObject->SaveNPCData(ObjectID, SaveDataInfo);
}

void ANPCBase::LoadData_Implementation()
{
	Super::LoadData_Implementation();
	FNPCSaveDataInfoBase* SavedData = SaveGameObject->LoadNPCData(ObjectID);
	if (SavedData)
	{
		SaveDataInfo = *SaveGameObject->LoadNPCData(ObjectID);
	}
}

void ANPCBase::GetDamage_Implementation(AActor* Attacker, float DamageValue, float PoiseDamage, EAttackDirection Direction, FVector HitLocation, FRotator HitRotation)
{
	if(!bIsAlive)
	{
		return;
	}
	Super::GetDamage_Implementation(Attacker, DamageValue, PoiseDamage, Direction, HitLocation, HitRotation);
	StopChasingTimer = MaxChasingTime; /*Reset Chase Timer when getting Damage*/
}

void ANPCBase::ResetAttackState(UAnimMontage* AttackAnimation, bool Interrupted)
{
	BlackboardComponent->SetValueAsBool("bCanAttack", true);
}

void ANPCBase::RaiseAggro_Implementation(AActor* Attacker, float AggroValue)
{
	if (!bIsAlive)
	{
		return;
	}

	if (AggroList.Contains(Attacker))
	{
		AggroList[Attacker] += AggroValue;
	}
	else
	{
		AggroList.Add(Attacker, AggroValue);
	}

	AActor* HighestAggroActor = nullptr;
	float HighestAggroValue = 0;
	for (TPair<AActor*, float> AggroCandidate : AggroList)
	{
		if (AggroCandidate.Value > HighestAggroValue)
		{
			HighestAggroActor = AggroCandidate.Key;
			HighestAggroValue = AggroCandidate.Value;
		}
	}
	if (HighestAggroActor && NoticedPlayer != HighestAggroActor)
	{
		NoticedPlayer = Cast<ACharacter>(HighestAggroActor);
		BlackboardComponent->SetValueAsObject("TargetPlayer", NoticedPlayer);
		AIController->SetFocus(HighestAggroActor, EAIFocusPriority::Gameplay);
	}
}

void ANPCBase::Die_Implementation()
{

	if (!bIsAlive)
	{
		return;
	}
	Super::Die_Implementation();


	AIController->StopMovement();
	BlackboardComponent->SetValueAsObject("TargetPlayer", nullptr);
	AIController->SetFocus(nullptr);

	bIsAlive = false;
	SaveDataInfo.bIsAlive = false;
	BlackboardComponent->SetValueAsBool("bIsAlive", false);

	NoticedPlayer = nullptr;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	ISaveable::Execute_SaveData(this);

}
