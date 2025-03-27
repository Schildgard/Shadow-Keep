// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraShakeSourceComponent.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponCategory = EWeaponType::StraightSword;
	ObjectType = EObjectType::Weapon;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon Mesh");
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	Hitbox = CreateDefaultSubobject<UCapsuleComponent>("Hitbox");
	Hitbox->SetupAttachment(RootComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio Component");
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->SetAutoActivate(false);

	CameraShakeComponent = CreateDefaultSubobject<UCameraShakeSourceComponent>("CameraShake");
	CameraShakeComponent->SetupAttachment(RootComponent);
	OffhandAttack = nullptr;

}

TArray<UAnimMontage*> AWeaponBase::GetNormalAttacks()
{
	return RegularAttackMontages;
}

UAnimMontage* AWeaponBase::GetOffhandAttack()
{
	return OffhandAttack;
}

UCapsuleComponent* AWeaponBase::GetHitbox()
{
	return Hitbox;
}

FTransform AWeaponBase::GetSocket(FName SocketName)
{
	if(WeaponMesh->DoesSocketExist(SocketName))
	{
		return WeaponMesh->GetSocketTransform(SocketName);
	}
	return FTransform::Identity;
}

UAudioComponent* AWeaponBase::GetAudioComponent()
{
	return AudioComponent;
}

UCameraShakeSourceComponent* AWeaponBase::GetCameraShakeComponent()
{
	return CameraShakeComponent;
}

FName AWeaponBase::GetWeaponName()
{
	return WeaponName;
}

