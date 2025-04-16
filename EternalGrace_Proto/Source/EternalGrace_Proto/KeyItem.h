// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectType.h"
#include "KeyItem.generated.h"


USTRUCT(BlueprintType)
struct FKeyItem : public FTableRowBase
{
	GENERATED_BODY()
public:
	FKeyItem();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> ThumpnailImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EObjectType ObjectType;
};
