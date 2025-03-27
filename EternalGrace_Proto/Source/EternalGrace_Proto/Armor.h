// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "ObjectType.h"
#include "Armor.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct  FArmor : public FTableRowBase
{
	GENERATED_BODY()
public:
	FArmor();
	UPROPERTY()
	EObjectType ObjectType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSoftObjectPtr<UTexture2D> ThumpNailImage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ArmorName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> BreastplateMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> ShoulderMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> ArmsMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsHeavyArmor;
};
