// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnComponent.h"
#include "Blueprint//UserWidget.h"
#include "Kismet//GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "CharacterBase.h"
#include "EternalGrace_ProtoCharacter.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
ULockOnComponent::ULockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Visualizer = nullptr;
	LockedOnTarget = nullptr;
	LockingActor = nullptr;
	OwningController = nullptr;

	ValidObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	ValidObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
	ValidObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);
	ValidObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery4);
	ValidObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery5);
}

void ULockOnComponent::LockOnTarget()
{
	LockedOnTarget = Cast<ACharacterBase>(FindTarget());

	if (LockedOnTarget && VisualizerWidgetClass)
	{
		if (!Visualizer)
		{
			Visualizer = CreateWidget<UUserWidget>(OwningController, VisualizerWidgetClass);
			if(LockedOnTarget->ActorHasTag("Ally"))
			{
				Visualizer->SetColorAndOpacity(FLinearColor::Yellow);
			}
			else
			{
				Visualizer->SetColorAndOpacity(FLinearColor::Red);
			}
			if(!Visualizer)
			{
				UE_LOG(LogTemp, Error, TEXT("No Visualizer (LockOnComponent), return!!"))
				return;
			}
		}
		if (!Visualizer->IsInViewport())
		{
			Visualizer->AddToPlayerScreen();
		}
		LockingActor->SetLockOn(true);
	}
}

void ULockOnComponent::UpdateLockOn(float DeltaTime)
{
	if (!LockedOnTarget || !LockingActor)
	{
		return;
	}
	//While sprinting the character rotates freely
	if (LockingActor->GetCurrentActionState() != EActionState::Running)
	{
		LockingActor->RotateTowardsTarget(LockedOnTarget);
	}

	//GET RELEVANT POSITIONS
	FVector CameraPosition = LockingActor->GetFollowCamera()->GetComponentLocation();
	FVector TargetPosition = LockedOnTarget->GetTargetLocation();

	//DIVIDE DISTANCE TO OPTIMIZE CAMERA Z ANGLE
	float Influence = UKismetMathLibrary::Vector_Distance(CameraPosition, TargetPosition) / DistanceInfluenceOnZ;
	float ZAdjustment = FMath::Clamp(Influence, MinZOffset, MaxZDrop);
	FVector TargetCameraDirection = FVector(TargetPosition.X, TargetPosition.Y, TargetPosition.Z - ZAdjustment);

	//SET CAMERA ROTATION
	FRotator CameraLook = UKismetMathLibrary::FindLookAtRotation(CameraPosition, TargetCameraDirection);
	FRotator InterpolatedRotation = FMath::RInterpTo(OwningController->GetControlRotation(), CameraLook, DeltaTime, CameraRotationInterpolation);

	// ONLY USE INTERPOLATED PITCH AND YAW
	FRotator UltimateRotation = FRotator(InterpolatedRotation.Pitch, InterpolatedRotation.Yaw, OwningController->GetControlRotation().Roll);
	OwningController->SetControlRotation(UltimateRotation);


	//UPDATE LOCK ON VISUALIZER POSITION
	if (Visualizer && LockedOnTarget)
	{
		FVector TargetSocketLocation = LockedOnTarget->GetMesh()->GetSocketLocation(FName("Target"));
		FVector2D ScreenPosition;

		bool Projected = UGameplayStatics::ProjectWorldToScreen(OwningController, TargetSocketLocation, ScreenPosition, true);

		if (Projected)
		{
		Visualizer->SetPositionInViewport(ScreenPosition, true);
		}
	}
}

void ULockOnComponent::UnlockTarget()
{
	LockedOnTarget = nullptr;
	LockingActor->SetLockOn(false);
	if (Visualizer)
	{
		Visualizer->RemoveFromViewport();
	}
}

ACharacterBase* ULockOnComponent::GetLockedOnTarget()
{
	return nullptr;
}

void ULockOnComponent::ToggeLockOn()
{
	if (!OwningController)
	{
		OwningController = UGameplayStatics::GetPlayerController(GetWorld(), LockingActor->GetPlayerIndex());
	}

	if (LockedOnTarget)
	{
		UnlockTarget();
	}
	else
	{
		LockOnTarget();
	}
}

void ULockOnComponent::FilterTargetsByDirection(float InputAxisValue)
{
	if (!LockedOnTarget)return;

	TArray<AActor*> ScannedActors = ScanForTargets();
	if (ScannedActors.Num() <= 0)return;

	AActor* ClosestTarget = nullptr;
	float comparison = 0.0f;
	float distance = 10000.0f;


	if (InputAxisValue >= 0.5f)
	{
		//ITERATE THROUGH SCANNED ACTORS TO FIND SMALLEST DISTANCE
		for (AActor* Actor : ScannedActors)
		{
			if (Actor != LockedOnTarget)
			{
				FVector TargetsRelativeLocation = LockingActor->GetTransform().InverseTransformPosition(Actor->GetActorLocation());
				//if (Actor->GetActorLocation().X < LockedOnTarget->GetActorLocation().X)
				if (TargetsRelativeLocation.Y >= 0)
				{
				//	UE_LOG(LogTemp, Warning, TEXT("Found %s right to LockedOnTarget, YPos: %f"), *Actor->GetFName().ToString(), TargetsRelativeLocation.Y);
					comparison = LockingActor->GetDistanceTo(Actor);
					if (comparison <= distance)
					{
						distance = comparison; //SAVE SMALLEST DISTANCE
						ClosestTarget = Actor; //SAVE ACTOR IF DISTANCE IS SMALLEST
					}
				}

			}
		}
	}
	else if (InputAxisValue <= -0.5f)
	{
		//ITERATE THROUGH SCANNED ACTORS TO FIND SMALLEST DISTANCE
		for (AActor* Actor : ScannedActors)
		{
			if (Actor != LockedOnTarget)
			{
				FVector TargetsRelativeLocation = LockingActor->GetTransform().InverseTransformPosition(Actor->GetActorLocation());
				//if (Actor->GetActorLocation().X > LockedOnTarget->GetActorLocation().X)
				if (TargetsRelativeLocation.Y <= 0)
				{
					comparison = LockingActor->GetDistanceTo(Actor);
					if (comparison <= distance)
					{
						distance = comparison; //SAVE SMALLEST DISTANCE
						ClosestTarget = Actor; //SAVE ACTOR IF DISTANCE IS SMALLEST
					}
				}
			}
		}
	}
	if (ClosestTarget)
	{
		LockedOnTarget = Cast<ACharacterBase>(ClosestTarget);
		if (LockedOnTarget->ActorHasTag("Ally"))
		{
			Visualizer->SetColorAndOpacity(FLinearColor::Yellow);
		}
		else
		{
			Visualizer->SetColorAndOpacity(FLinearColor::Red);
		}
	}

}

TArray<AActor*> ULockOnComponent::ScanForTargets()
{
	FVector PlayerPosition = LockingActor->GetActorLocation();
	TArray<FHitResult> ScanHits; //SET UP A LIST FOR HITTED OBJECTS
	TArray<AActor*> ActorsToIgnore; //SET UP A LIST SO ACTORS WONT GET SCANNED MULTIPLE TIMES
	TArray<AActor*> ScannedActors;
	if (!LockedOnTarget) //This is Called when Player toggles Locking On.
	{
		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), PlayerPosition, PlayerPosition, 750.0f, ValidObjectTypes, true, ActorsToIgnore, EDrawDebugTrace::None, ScanHits, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
	}
	else //This is called when Player switches targets. The difference is that the SphereTrave Position starts at the Target,
	{
		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), LockedOnTarget->GetActorLocation(), LockedOnTarget->GetActorLocation(), 750.0f, ValidObjectTypes, true, ActorsToIgnore, EDrawDebugTrace::None, ScanHits, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
	}

	if (ScanHits.Num() > 0)
	{
		//ITERARE THROUGH SCANHITS
		for (const FHitResult& Hit : ScanHits)
		{
			AActor* HitActor = Hit.GetActor();
			//ADD HITTED ACTOR TO IGNORE AND VIABLE TARGET LIST
			if (HitActor && HitActor != LockingActor && !ActorsToIgnore.Contains(HitActor) && HitActor->ActorHasTag("Targetable"))
			{
				ScannedActors.Add(HitActor);
				ActorsToIgnore.Add(HitActor);
			}
		}
	}
	return ScannedActors;
}

AActor* ULockOnComponent::FindTarget()
{
	TArray<AActor*> ScannedActors = ScanForTargets();
	if (ScannedActors.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Find Nearest Target: No Actors Scanned"))
			return nullptr;
	}
	AActor* ClosestTarget = nullptr;
	float comparison = 0.0f;
	float distance = 10000.0f;

	//ITERATE THROUGH SCANNED ACTORS TO FIND SMALLEST DISTANCE
	for (AActor* Actor : ScannedActors)
	{
		comparison = LockingActor->GetDistanceTo(Actor);
		if (comparison <= distance)
		{
			distance = comparison; //SAVE SMALLEST DISTANCE
			ClosestTarget = Actor; //SAVE ACTOR IF DISTANCE IS SMALLEST
		}
	}
	return ClosestTarget;

}

void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();
	LockingActor = Cast<AEternalGrace_ProtoCharacter>(GetOwner());
}

void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateLockOn(DeltaTime);
}

