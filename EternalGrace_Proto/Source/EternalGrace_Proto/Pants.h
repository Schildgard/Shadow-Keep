// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectType.h"
#include "Pants.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct  FPants : public FTableRowBase
{
	GENERATED_BODY()

public:

	FPants();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName PantsName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> PantsMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EObjectType ObjectType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSoftObjectPtr<UTexture2D> ThumpNailImage;


};
