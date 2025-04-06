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
		switch (Direction)
		{
		case EAttackDirection::Front:
			UE_LOG(LogTemp, Warning, TEXT("Front"))
		CharacterAnimInstance->Montage_Play(StaggerAnimations[0]);
			break;
		case EAttackDirection::Back:
			UE_LOG(LogTemp, Warning, TEXT("Back"))
			CharacterAnimInstance->Montage_Play(StaggerAnimations[1]);
				break;
		case EAttackDirection::Right:
			UE_LOG(LogTemp, Warning, TEXT("Right"))
			CharacterAnimInstance->Montage_Play(StaggerAnimations[2]);
			break;
		case EAttackDirection::Left:
			UE_LOG(LogTemp, Warning, TEXT("Left"))
			CharacterAnimInstance->Montage_Play(StaggerAnimations[3]);
			break;
		}
		CurrentPoise = MaxPoise;
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

