// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputType.h"
#include "InputBufferComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ETERNALGRACE_PROTO_API UInputBufferComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInputBufferComponent();

protected:

	void BeginPlay()override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInputType InputBuffer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buffering", meta = (AllowPrivateAccess))
	float BufferTime;

	float Timer;


public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SaveInput(EInputType Input);

		
};
