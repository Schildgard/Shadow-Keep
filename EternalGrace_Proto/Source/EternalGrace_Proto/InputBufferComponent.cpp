// Fill out your copyright notice in the Description page of Project Settings.


#include "InputBufferComponent.h"

// Sets default values for this component's properties
UInputBufferComponent::UInputBufferComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InputBuffer = EInputType::NoInput;
	BufferTime = 0.25f;
	Timer = BufferTime;
}

void UInputBufferComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
}


// Called every frame
void UInputBufferComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (InputBuffer != EInputType::NoInput)
	{
		Timer -= DeltaTime;
		if (Timer <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Input Component resets Timer"))
			InputBuffer = EInputType::NoInput;
			Timer = BufferTime;
			SetComponentTickEnabled(false);
		}
	}
}

void UInputBufferComponent::SaveInput(EInputType Input)
{
	UE_LOG(LogTemp, Warning, TEXT("Input Component Saves Input"))
	if (Input != EInputType::NoInput)
	{
		SetComponentTickEnabled(true);
	}
	InputBuffer = Input;
}

