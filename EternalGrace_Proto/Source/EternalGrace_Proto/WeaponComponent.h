// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponType.h"
#include "WeaponComponent.generated.h"


class AWeaponBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ETERNALGRACE_PROTO_API UWeaponComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TSubclassOf<AWeaponBase> CurrentWeaponClass;

	UPROPERTY()
	AWeaponBase* CurrentWeaponObject;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	EWeaponType ChangeWeapon(TSubclassOf<AWeaponBase> WeaponToEquip);

};
