// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldBase.h"

// Sets default values
AShieldBase::AShieldBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ShieldMesh = nullptr;
	ObjectType = EObjectType::Shield;
	BlockValue = 100.0f;
	Balance = 100.0f;

//	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>("Shield Mesh");
//	RootComponent = ShieldMesh;
//	ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//	ShieldMesh->SetCollisionResponseToAllChannels(ECR_Overlap);

}

