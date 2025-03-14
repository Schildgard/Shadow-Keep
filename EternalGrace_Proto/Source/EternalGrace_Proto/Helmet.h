// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectType.h"
#include "Helmet.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct  FHelmet : public FTableRowBase
{
	GENERATED_BODY()

public:
	FHelmet() : HelmetName("NoHelmet"), HelmetMesh(nullptr), ObjectType(EObjectType::HeadEquipment), ThumpNailImage(nullptr)
	{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName HelmetName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> HelmetMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EObjectType ObjectType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSoftObjectPtr<UTexture2D> ThumpNailImage;

};
