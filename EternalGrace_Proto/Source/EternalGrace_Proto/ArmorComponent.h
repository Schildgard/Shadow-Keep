// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "ArmorComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ETERNALGRACE_PROTO_API UArmorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UArmorComponent();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess))
	UDataTable* BreastPlateArmorTable;
	UPROPERTY()
	FName CurrentBreastplateName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess))
	UDataTable* PantsTable;
	UPROPERTY()
	FName CurrentPantsName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess))
	UDataTable* HelmetTable;
	UPROPERTY()
	FName CurrentHelmetsName;

	//Breastplate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", meta = (AllowPrivateAccess))
	USkeletalMeshComponent* Body;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", meta = (AllowPrivateAccess))
	USkeletalMeshComponent* Arms;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", meta = (AllowPrivateAccess))
	USkeletalMeshComponent* Shoulders;
	//Pants
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", meta = (AllowPrivateAccess))
	USkeletalMeshComponent* Pants;
	//Helmet
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment", meta = (AllowPrivateAccess))
	USkeletalMeshComponent* Helmet;


public:

	UFUNCTION()
	void EquipBreastPlate(FName ArmorName);

	UFUNCTION()
	void EquipPants(FName PantsName);

	UFUNCTION()
	void EquipHelmet(FName HelmetName);

	UFUNCTION()
	USkeletalMeshComponent* GetBodyArmorMesh();
	UFUNCTION()
	USkeletalMeshComponent* GetArmsArmorMesh();
	UFUNCTION()
	USkeletalMeshComponent* GetShoulderArmorMesh();

	UFUNCTION()
	USkeletalMeshComponent* GetPantsMesh();

	UFUNCTION()
	USkeletalMeshComponent* GetHelmetMesh();
	//This Getter functions are used to load Data from SaveGame
	UFUNCTION()
	FName GetArmorName();
	UFUNCTION()
	FName GetPantsName();
	UFUNCTION()
	FName GetHelmetsName();

};
