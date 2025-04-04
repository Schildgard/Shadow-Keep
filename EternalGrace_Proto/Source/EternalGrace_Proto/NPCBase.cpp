// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBase.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"


ANPCBase::ANPCBase()
{
	SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("Sensing");
	AIController = nullptr;
	bIsHostile = false;
	AttackRange = 150.f;
}

void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
	SensingComponent->OnSeePawn.AddDynamic(this, &ANPCBase::NoticePlayer);

	AIController = Cast<AAIController>(GetController());
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
	if(!AnimationInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy failed to get AnimationInstance (NPC Base Class)"));
		return;
	}

}

void ANPCBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (NoticedPlayer && bIsHostile)
	{
		BlackboardComponent->SetValueAsBool("bIsInAttackRange", CheckDistanceToPlayer() <= AttackRange);
		if(CheckDistanceToPlayer() > AttackRange)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Current Distance is %f from attackrange : %f"), CheckDistanceToPlayer(), AttackRange)
		}
		
	}
}

void ANPCBase::NoticePlayer(APawn* SpottedPawn)
{
	if (!NoticedPlayer)
	{
		NoticedPlayer = Cast<ACharacter>(SpottedPawn);
		if (NoticedPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s Spotted %s"), *GetFName().ToString(), *NoticedPlayer->GetFName().ToString())
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

void ANPCBase::Attack_Implementation()
{
	Super::Attack_Implementation();
	BlackboardComponent->SetValueAsBool("bCanAttack", false);

	if (NormalAttackArray.Num() >= 1)
	{
		int AttackIndex = UKismetMathLibrary::RandomIntegerInRange(0, NormalAttackArray.Num() - 1);
		UE_LOG(LogTemp, Error, TEXT("Attack Index is %i"), AttackIndex)
		AnimationInstance->Montage_Play(NormalAttackArray[AttackIndex]);

	//	FOnMontageEnded AttackEndDelegate;
	//	AttackEndDelegate.BindUObject(this, &ANPCBase::ResetAttackState);
	//	AnimationInstance->Montage_SetEndDelegate(AttackEndDelegate, NormalAttackArray[AttackIndex]);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Attacks assigned for %s"), *GetFName().ToString())
	}
}

void ANPCBase::ResetAttackState(UAnimMontage* AttackAnimation, bool Interrupted)
{
	BlackboardComponent->SetValueAsBool("bCanAttack", true);
	UE_LOG(LogTemp, Error, TEXT("Reseted its AttackState %s"), *GetFName().ToString())
}

void ANPCBase::ResetAttackState2()
{
	BlackboardComponent->SetValueAsBool("bCanAttack", true);
	UE_LOG(LogTemp, Error, TEXT("Reseted its AttackState %s"), *GetFName().ToString())
}
