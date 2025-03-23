// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponType.h"
#include "WeaponBase.generated.h"

UCLASS()
class ETERNALGRACE_PROTO_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	virtual void BeginPlay() override;





public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(AllowPrivateAccess))
	UStaticMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TArray<UAnimMontage*> RegularAttackMontages;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	EWeaponType WeaponCategory;

};
