// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StrafeAroundTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"
#include "CharacterAnimInstanceBase.h"

UBTTask_StrafeAroundTarget::UBTTask_StrafeAroundTarget()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_StrafeAroundTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get References
	AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	AICharacter = Cast<ACharacter>(AIController->GetPawn());
	if(!AICharacter) return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject("TargetPlayer"));
	if(!TargetActor) return EBTNodeResult::Failed;

	AnimInstance = Cast<UCharacterAnimInstanceBase>(AICharacter->GetMesh()->GetAnimInstance());
	if(!AnimInstance) return EBTNodeResult::Failed;

	AIController->SetFocus(TargetActor);
	AICharacter->GetCharacterMovement()->MaxWalkSpeed = 125.f;
	AnimInstance->CurrentActionState = EActionState::Strafing;

	//Calculate TargetStrafe Position
	FVector ForwardLocation = AICharacter->GetActorForwardVector() * 25;
	FVector CurrentLocation = AICharacter->GetActorLocation();

	FVector StrafeDirection = FMath::RandBool() ? AICharacter->GetActorRightVector() : AICharacter->GetActorRightVector() * -1;

	FVector SideLocation = ForwardLocation + CurrentLocation + (StrafeDirection * StrafeDistance);

	AIController->MoveToLocation(SideLocation,5.0f);

	return EBTNodeResult::InProgress;
}

void UBTTask_StrafeAroundTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if(!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("No AI Controller (Tick Task, Starfing Task)"));
		return;
	}
	EPathFollowingStatus::Type MoveStatus = AIController->GetMoveStatus();
	if(MoveStatus == EPathFollowingStatus::Idle || MoveStatus == EPathFollowingStatus::Paused)
	{
		AICharacter->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("bCanAttack", true);
		AnimInstance->CurrentActionState = EActionState::Idle;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
