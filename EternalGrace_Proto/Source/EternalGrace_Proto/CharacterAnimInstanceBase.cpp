// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstanceBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "KismetAnimationLibrary.h"
#include "CharacterBase.h"

void UCharacterAnimInstanceBase::NativeInitializeAnimation()
{
	Character = Cast<ACharacterBase>(GetOwningActor());
	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
		UE_LOG(LogTemp, Error, TEXT("Succesfully Cast Character (AnimInstanceBase)"))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Cast Character (AnimInstanceBase)"))
			return;
	}
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Cast MovementComponent (AnimInstanceBase)"))
	}
}

void UCharacterAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Character && Character->IsValidLowLevel() && MovementComponent->IsValidLowLevel())
	{
		FVector HorizontalMovementSpeed = FVector(MovementComponent->Velocity.X, MovementComponent->Velocity.Y, 0.0f);
		GroundSpeed = HorizontalMovementSpeed.Length();
		FacingDirection = UKismetAnimationLibrary::CalculateDirection(HorizontalMovementSpeed, Character->GetActorRotation());

		bShouldMove = GroundSpeed > 3.0f;

		bIsFalling = MovementComponent->IsFalling();

		Velocity = MovementComponent->Velocity;
	}
}

void UCharacterAnimInstanceBase::ResetCharacterState(UAnimMontage* Montage, bool Interrupted)
{
	CurrentActionState = EActionState::Idle;
	Character->SetCurrentActionState(EActionState::Idle);
}
