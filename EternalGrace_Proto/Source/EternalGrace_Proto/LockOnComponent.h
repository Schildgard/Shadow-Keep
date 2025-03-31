// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnComponent.generated.h"


class ACharacterBase;
class AEternalGrace_ProtoCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ETERNALGRACE_PROTO_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockOnComponent();


public:
	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void LockOnTarget();
	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void UpdateLockOn(float DeltaTime);
	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void UnlockTarget();
	UFUNCTION()
	ACharacterBase* GetLockedOnTarget();

	UFUNCTION()
	void ToggeLockOn();

	UFUNCTION()
	void FilterTargetsByDirection(float InputAxisValue);
	UFUNCTION()
	TArray<AActor*> ScanForTargets();

	UFUNCTION()
	AActor* FindTarget();
 

private:

	void BeginPlay()override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LockOn", meta = (AllowPrivateAccess))
	float CameraRotationInterpolation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LockOn", meta = (AllowPrivateAccess))
	float CharacterRotationInterpolation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LockOn", meta = (AllowPrivateAccess))
	float DistanceInfluenceOnZ;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LockOn", meta = (AllowPrivateAccess))
	float MinZOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LockOn", meta = (AllowPrivateAccess))
	float MaxZDrop;
	UPROPERTY()
	ACharacterBase* LockedOnTarget;
	UPROPERTY()
	AEternalGrace_ProtoCharacter* LockingActor;
	UPROPERTY()
	APlayerController* OwningController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LockOn", meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> VisualizerWidgetClass;

	UPROPERTY()
	UUserWidget* Visualizer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LockOn, meta = (AllowPrivateAccess))
	TArray<TEnumAsByte<EObjectTypeQuery>> ValidObjectTypes;

		
};
