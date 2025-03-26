// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/CapsuleComponent.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponCategory = EWeaponType::StraightSword;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon Mesh");
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	Hitbox = CreateDefaultSubobject<UCapsuleComponent>("Hitbox");
	Hitbox->SetupAttachment(RootComponent);

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

