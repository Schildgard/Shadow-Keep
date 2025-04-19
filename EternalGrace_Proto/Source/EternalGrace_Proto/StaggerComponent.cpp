// Fill out your copyright notice in the Description page of Project Settings.


#include "StaggerComponent.h"
#include "CharacterAnimInstanceBase.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UStaggerComponent::UStaggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxPoise = 100.0f;
	CurrentPoise = MaxPoise;
}

bool UStaggerComponent::GetStaggered(EAttackDirection Direction, float PoiseDamage, AActor* Instigator)
{
	CurrentPoise -= PoiseDamage;
	if(CurrentPoise <= 0)
	{
		int StaggerAnimationIndex = 0;
		switch (Direction)
		{
		case EAttackDirection::Front:
				StaggerAnimationIndex = 0;
			break;
		case EAttackDirection::Back:
				StaggerAnimationIndex = 1;
				break;
		case EAttackDirection::Right:
				StaggerAnimationIndex = 2;;
			break;
		case EAttackDirection::Left:
				StaggerAnimationIndex = 3;
			break;
		}
		CharacterAnimInstance->Montage_Play(StaggerAnimations[StaggerAnimationIndex]);
		CurrentPoise = MaxPoise;

	//	FOnMontageEnded EndDelegate;
	//	EndDelegate.BindUObject(CharacterAnimInstance, &UCharacterAnimInstanceBase::ResetCharacterState);
	//	CharacterAnimInstance->Montage_SetEndDelegate(EndDelegate, StaggerAnimations[StaggerAnimationIndex]);

		FOnMontageBlendingOutStarted BlendOutDelegate;
		BlendOutDelegate.BindUObject(CharacterAnimInstance, &UCharacterAnimInstanceBase::ResetCharacterState);
		CharacterAnimInstance->Montage_SetBlendingOutDelegate(BlendOutDelegate, StaggerAnimations[StaggerAnimationIndex]);
		


		return true;
	}
	return false;
}


// Called when the game starts
void UStaggerComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(Character)
	{
		CharacterAnimInstance = Cast<UCharacterAnimInstanceBase>(Character->GetMesh()->GetAnimInstance());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s Failed to get Character (StaggerComponent)"), *GetOwner()->GetFName().ToString())
	}
	
}


// Called every frame
//void UStaggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

