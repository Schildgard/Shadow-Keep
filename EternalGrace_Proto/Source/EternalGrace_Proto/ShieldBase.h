// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectType.h"
#include "WeaponBase.h"
#include "ShieldBase.generated.h"

UCLASS()
class ETERNALGRACE_PROTO_API AShieldBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:	
	AShieldBase();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	UStaticMeshComponent* ShieldMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	FName ShieldName;

public:	
	float BlockValue;
	float Balance;

};
