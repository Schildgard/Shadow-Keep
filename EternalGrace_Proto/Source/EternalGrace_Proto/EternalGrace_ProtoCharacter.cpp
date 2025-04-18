// Copyright Epic Games, Inc. All Rights Reserved.

#include "EternalGrace_ProtoCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "EternalGrace_SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "EternalGrace_GameInstance.h"
#include "ArmorComponent.h"
#include "EG_PlayerController.h"
#include "InventoryComponent.h"
#include "Armor.h"
#include "Pants.h"
#include "Interactable.h"
#include "WeaponComponent.h"
#include "ShieldComponent.h"
#include "WeaponBase.h"
#include "EG_AnimInstance.h"
#include "InputBufferComponent.h"
#include "HealthComponent.h"
#include "SurfaceType.h"
#include "LockOnComponent.h"
#include "StaggerComponent.h"
#include "AttackDirection.h"
#include "Blueprint/UserWidget.h"
DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AEternalGrace_ProtoCharacter

AEternalGrace_ProtoCharacter::AEternalGrace_ProtoCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	UpperArmor = CreateDefaultSubobject<UArmorComponent>("Upper Armor Component");

	UpperArmor->GetBodyArmorMesh()->SetupAttachment(GetMesh());
	UpperArmor->GetShoulderArmorMesh()->SetupAttachment(GetMesh());
	UpperArmor->GetArmsArmorMesh()->SetupAttachment(GetMesh());

	UpperArmor->GetBodyArmorMesh()->SetLeaderPoseComponent(GetMesh());
	UpperArmor->GetShoulderArmorMesh()->SetLeaderPoseComponent(GetMesh());
	UpperArmor->GetArmsArmorMesh()->SetLeaderPoseComponent(GetMesh());

	UpperArmor->GetPantsMesh()->SetupAttachment(GetMesh());
	UpperArmor->GetPantsMesh()->SetLeaderPoseComponent(GetMesh());

	UpperArmor->GetHelmetMesh()->SetupAttachment(GetMesh(), "s_head");
	UpperArmor->GetHelmetMesh()->SetBoundsScale(2.0f);

	UpperArmor->GetArmorSoundComponent()->SetupAttachment(GetMesh());

	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("Weapon");
	WeaponComponent->SetupAttachment(GetMesh(), "s_hand_r");

	InputBufferingComponent = CreateDefaultSubobject<UInputBufferComponent>("InputBuffering");
	LockOnComponent = CreateDefaultSubobject<ULockOnComponent>("LockOnSystem");
	StaggerComponent = CreateDefaultSubobject<UStaggerComponent>("StaggerSystem");

	ScanDistance = 150.0f;
	WallDistanceOffset = 20.f;
	CapsuleHeightOffset = 10.f;

}

//////////////////////////////////////////////////////////////////////////
// Input

void AEternalGrace_ProtoCharacter::ShowInventory()
{
	APlayerController* ActiveController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex);
	if (ActiveController)
	{
		AEG_PlayerController* EGController = Cast<AEG_PlayerController>(ActiveController);
		if (EGController)
		{
			EGController->ShowInventory();
		}
	}
}

void AEternalGrace_ProtoCharacter::NormalAttack()
{
	int AttackCount = EGAnimInstance->AttackCount;
	if (AttackCount >= WeaponComponent->NormalWeaponAttacks.Num() || bIsCurrentlyClimbingUp) return; // Make sure the Attack does not break Attack Array Size
	switch (CurrentActionState)
	{
	case EActionState::Running: //if Running, perform running Attack
		CancelSprint();
		SetCurrentActionState(EActionState::Attacking);
		EGAnimInstance->Montage_Play(WeaponComponent->RunningAttack, true);
		break;
	case EActionState::Jumping: //if in Midair, do Nothing. Maybe implement JumpAttacks later.
		UE_LOG(LogTemp, Warning, TEXT("No Jumping Attack Implemented yet"));
		break;
	case EActionState::Attacking:
		if (!EGAnimInstance->bCanContinueAttack) //If already in Attack, buffer input
		{
			InputBufferingComponent->SaveInput(EInputType::NormalAttack);
			return;
		}
		EGAnimInstance->Montage_Play(WeaponComponent->NormalWeaponAttacks[AttackCount], true);
		break;
	case EActionState::Dodging:
		UE_LOG(LogTemp, Warning, TEXT("Buffer Dodge Attack"))
			InputBufferingComponent->SaveInput(EInputType::DodgeAttack);
		break;
	case EActionState::Climbing:
		InputBufferingComponent->SaveInput(EInputType::NormalAttack);
		break;
	case EActionState::Staggered:
		InputBufferingComponent->SaveInput(EInputType::NormalAttack);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Trigger Attack"))
		SetCurrentActionState(EActionState::Attacking);
		EGAnimInstance->Montage_Play(WeaponComponent->NormalWeaponAttacks[AttackCount], true);
		break;
	}
}

void AEternalGrace_ProtoCharacter::PerformDodgeAttack()
{
	SetCurrentActionState(EActionState::Attacking);
	EGAnimInstance->Montage_Play(WeaponComponent->DodgeAttack, true);
}

void AEternalGrace_ProtoCharacter::PerformOffhandAction()
{
	int AttackCount = EGAnimInstance->AttackCount;
	if (AttackCount >= WeaponComponent->NormalWeaponAttacks.Num()) return; // Make sure the Attack does not break Attack Array Size
	switch (CurrentActionState)
	{
	case EActionState::Idle: //if in Idle, go into Guard Pose
		SetCurrentActionState(EActionState::Guarding);
		break;
	case EActionState::Attacking: //if attacking, check if an offhand attack can be performed and perform or buffer if necessary
		if (!ShieldComponent->GetShieldAttack() || !EGAnimInstance->bCanOffhandAttack) return;
		if (!EGAnimInstance->bCanContinueAttack)
		{
			InputBufferingComponent->SaveInput(EInputType::OffhandAttack);
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("Perform ShieldBash"))
		EGAnimInstance->Montage_Play(ShieldComponent->GetShieldAttack(), true);
		break;
	case EActionState::Climbing:
		InputBufferingComponent->SaveInput(EInputType::OffhandAttack);
		break;
	default:
		break;
	}
}

void AEternalGrace_ProtoCharacter::TriggerCurrentInteractable()
{
	Interact_Implementation();
}

void AEternalGrace_ProtoCharacter::Sprint()
{
	if (CurrentActionState == EActionState::Idle)
	{
			CurrentActionState = EActionState::Running;
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
	}
}

void AEternalGrace_ProtoCharacter::CancelSprint()
{
	if (CurrentActionState == EActionState::Running)
	{
		SetCurrentActionState(EActionState::Idle);
		GetCharacterMovement()->MaxWalkSpeed = 250.f;
	}
}

void AEternalGrace_ProtoCharacter::Dodge()
{
	if (CurrentActionState == EActionState::Idle)
	{
		SetCurrentActionState(EActionState::Dodging);
		EGAnimInstance->Montage_Play(DodgeMontage, true);
	}
}

void AEternalGrace_ProtoCharacter::LockOn()
{
	LockOnComponent->ToggeLockOn();
}

void AEternalGrace_ProtoCharacter::SwitchTarget(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	LockOnComponent->FilterTargetsByDirection(LookAxisVector.X);
}

void AEternalGrace_ProtoCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Animation
	EGAnimInstance = Cast<UEG_AnimInstance>(GetMesh()->GetAnimInstance());
	if (!EGAnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Cast AnimInstance (PlayerCharacter)"))
			return;
	}
	EGAnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AEternalGrace_ProtoCharacter::FinishClimbing);
	//Weapon
	if (WeaponComponent->CurrentWeaponClass)
	{
		EquipWeapon(WeaponComponent->CurrentWeaponClass);
	}
	if (WeaponComponent->OffhandWeaponClass)
	{
		EquipOffhandWeapon(WeaponComponent->OffhandWeaponClass);
	}

}

void AEternalGrace_ProtoCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEternalGrace_ProtoCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AEternalGrace_ProtoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEternalGrace_ProtoCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEternalGrace_ProtoCharacter::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AEternalGrace_ProtoCharacter::Interact_Implementation);
		EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Triggered, this, &AEternalGrace_ProtoCharacter::ShowInventory);

		//Attacking
		EnhancedInputComponent->BindAction(NormalAttackAction, ETriggerEvent::Triggered, this, &AEternalGrace_ProtoCharacter::NormalAttack);
		EnhancedInputComponent->BindAction(NormalOffhandAction, ETriggerEvent::Triggered, this, &AEternalGrace_ProtoCharacter::PerformOffhandAction);
		EnhancedInputComponent->BindAction(NormalOffhandAction, ETriggerEvent::Completed, this, &AEternalGrace_ProtoCharacter::CancelGuard);
		EnhancedInputComponent->BindAction(NormalOffhandAction, ETriggerEvent::Canceled, this, &AEternalGrace_ProtoCharacter::CancelGuard);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AEternalGrace_ProtoCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AEternalGrace_ProtoCharacter::CancelSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &AEternalGrace_ProtoCharacter::CancelSprint);

		//Dodging
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AEternalGrace_ProtoCharacter::Dodge);

		//LockOn
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Triggered, this, &AEternalGrace_ProtoCharacter::LockOn);
		EnhancedInputComponent->BindAction(SwitchTargetAction, ETriggerEvent::Triggered, this, &AEternalGrace_ProtoCharacter::SwitchTarget);

		//Climbing
		EnhancedInputComponent->BindAction(HoldOnLedgeAction, ETriggerEvent::Triggered, this, &AEternalGrace_ProtoCharacter::ClimpCheckForward);
		EnhancedInputComponent->BindAction(HoldOnLedgeAction, ETriggerEvent::Completed, this, &AEternalGrace_ProtoCharacter::DropFromLedge);
		EnhancedInputComponent->BindAction(ClimbAction, ETriggerEvent::Triggered, this, &AEternalGrace_ProtoCharacter::Climb);


	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AEternalGrace_ProtoCharacter::ChangeUpperArmor(FName ArmorID)
{
	UpperArmor->EquipBreastPlate(ArmorID);
}

void AEternalGrace_ProtoCharacter::ChangePants(FName PantsID)
{
	UpperArmor->EquipPants(PantsID);
}

void AEternalGrace_ProtoCharacter::ChangeHelmet(FName HelmetID)
{
	UpperArmor->EquipHelmet(HelmetID);
	if (UpperArmor->GetHelmetsName() != "NoHelmet")
	{
		HairComponent->SetVisibility(false);
		BeardComponent->SetVisibility(false);
	}
	else
	{
		HairComponent->SetVisibility(true);
		BeardComponent->SetVisibility(true);
	}
}

UArmorComponent* AEternalGrace_ProtoCharacter::GetArmorComponent()
{
	return UpperArmor;
}

UInventoryComponent* AEternalGrace_ProtoCharacter::GetInventory()
{
	return PlayerInventory;
}

void AEternalGrace_ProtoCharacter::ObtainArmor(FName ArmorName, EObjectType ItemCategory)
{
	PlayerInventory->TryToObtainItem(ArmorName, ItemCategory);
}

void AEternalGrace_ProtoCharacter::SetPlayerIndex(int AssignedPlayerIndex)
{
	PlayerIndex = AssignedPlayerIndex;
	if (PlayerIndex == 1)
	{
		ObjectID = "PlayerTwo";
	}
	else
	{
		ObjectID = "PlayerOne";
	}
}

int AEternalGrace_ProtoCharacter::GetPlayerIndex()
{
	return PlayerIndex;
}

void AEternalGrace_ProtoCharacter::EquipWeapon(TSubclassOf<AWeaponBase> WeaponSubclass)
{
	EWeaponType NewWeaponCategory = WeaponComponent->ChangeWeapon(WeaponSubclass);
	EGAnimInstance->SetWeaponType(NewWeaponCategory);
}

void AEternalGrace_ProtoCharacter::EquipOffhandWeapon(TSubclassOf<AWeaponBase> WeaponSubclass)
{
	WeaponComponent->ChangeOffhandWeapon(WeaponSubclass);
}

void AEternalGrace_ProtoCharacter::FireBufferedInput(EInputType BufferedInput)
{
		switch (BufferedInput)
		{
		case EInputType::NormalAttack:
			NormalAttack();
			break;
		case EInputType::HeavyAttack:
			//NO HEAVY ATTACK YET
			break;
		case EInputType::OffhandAttack:
			PerformOffhandAction();
			break;
		case EInputType::Dodge:
			//NO DODGE YET
			break;
		case EInputType::Interact:
			Interact_Implementation();
			break;
		case EInputType::Jump:
			Jump();
			break;
		case EInputType::DodgeAttack:
				PerformDodgeAttack();
			break;
		default:
			break;
		}
}

UInputBufferComponent* AEternalGrace_ProtoCharacter::GetInputBufferComponent()
{
	return InputBufferingComponent;
}

void AEternalGrace_ProtoCharacter::SetLockOn(bool Value)
{
	EGAnimInstance->bIsLockedOn = Value;
}

void AEternalGrace_ProtoCharacter::ClimpCheckForward()
{
	if (CurrentActionState != EActionState::Climbing)
	{
		FVector Destination = GetActorLocation() + (GetActorForwardVector() * ScanDistance);
		FHitResult Hit;
		bool bHitScan = UKismetSystemLibrary::SphereTraceSingle(World, GetActorLocation(), Destination, 10.0f, ETraceTypeQuery::TraceTypeQuery1, false, ClimpActorsToIgnore, EDrawDebugTrace::None, Hit, true);

		if (bHitScan)
		{
			if (Hit.GetActor()->ActorHasTag("Climpable"))
			{
					UE_LOG(LogTemp, Warning, TEXT("Hitted Wall of %s"), *Hit.GetActor()->GetFName().ToString())
				FVector WallLocation = Hit.Location;
				FVector WallNormal = Hit.Normal;
				ClimpCheckUpward(WallLocation, WallNormal);
			}
		}
	}
}

void AEternalGrace_ProtoCharacter::ClimpCheckUpward(FVector WallLocation, FVector WallNormal)
{
	FVector Destination = GetActorLocation() + (GetActorForwardVector() * 75.f);
	FVector Start = Destination + FVector(0.0f, 0.0f, 200.f);
	FHitResult Hit;
	bool bHitScan = UKismetSystemLibrary::SphereTraceSingle(World, Start, Destination, 10.0f, ETraceTypeQuery::TraceTypeQuery1, false, ClimpActorsToIgnore, EDrawDebugTrace::None, Hit, true, FLinearColor::Yellow, FLinearColor::Blue);
	//UE_LOG(LogTemp, Error, TEXT("Do Hill Edge Check"))
	if (bHitScan)
	{
	//	UE_LOG(LogTemp, Error, TEXT("Hill Edge Check Successfull"))
		FVector SocketLocation = GetMesh()->GetSocketLocation("s_climp");

		float HeightDistance = SocketLocation.Z - Hit.Location.Z;
		if (UKismetMathLibrary::InRange_FloatFloat(HeightDistance, -65.f, 0, true, true) && CurrentActionState != EActionState::Climbing)
		{
			HangOnLedge(WallLocation, WallNormal, Hit.Location);
		}
		//	UE_LOG(LogTemp, Error, TEXT("HeightRange Check failed: HeightDistance : %f"), HeightDistance)
	}

}

void AEternalGrace_ProtoCharacter::HangOnLedge(FVector SnappingPosition, FVector WallNormal, FVector HeightLocation)
{
	UE_LOG(LogTemp, Error, TEXT("Call Hang on ledge Function"))
	//Lock Player Position and Play Hanging Animation
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetCharacterMovement()->StopMovementImmediately();
	SetCurrentActionState(EActionState::Climbing);
	EGAnimInstance->Montage_Play(ClimpAnimation, 1.0f);
	EGAnimInstance->Montage_Pause();

	UCapsuleComponent* Capsule = GetCapsuleComponent();
	//Get Snapping Position
	FVector OffsetDistanceToWall = (WallNormal * WallDistanceOffset) + SnappingPosition;
	//Subtract Half of CharacterSize from height Location to position Hanging Height
	FVector TargetHeightLocation = FVector(0.0f, 0.0f, HeightLocation.Z - CapsuleHeightOffset);

	FVector TargetLocation = FVector(OffsetDistanceToWall.X, OffsetDistanceToWall.Y, TargetHeightLocation.Z);
	FRotator TargetRotation = UKismetMathLibrary::MakeRotFromX(-WallNormal);


	FLatentActionInfo ActionInfo;
	ActionInfo.CallbackTarget = this;

	UGameplayStatics::PlaySoundAtLocation(World, AttachToWallSound, TargetLocation);
	UKismetSystemLibrary::MoveComponentTo(Capsule, TargetLocation, TargetRotation, true, true, 0.1f, false, EMoveComponentAction::Move, ActionInfo);


}

void AEternalGrace_ProtoCharacter::Climb()
{
	if (CurrentActionState == EActionState::Climbing && !bIsCurrentlyClimbingUp)
	{
		bIsCurrentlyClimbingUp = true;
		EGAnimInstance->Montage_Play(ClimpAnimation);
	}
}

void AEternalGrace_ProtoCharacter::DropFromLedge()
{
	if (CurrentActionState == EActionState::Climbing && !bIsCurrentlyClimbingUp)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		SetCurrentActionState(EActionState::Idle);
		EGAnimInstance->Montage_Stop(0.25f);
	}
}

void AEternalGrace_ProtoCharacter::FinishClimbing(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	SetCurrentActionState(EActionState::Idle);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	bIsCurrentlyClimbingUp = false;
}
/*Late Initialization is called by GameMode after possessing the pawn to prevent Issues with Player Index, which is set by Player Controller but also used by Initialization of the Character.
The Problem is that Character uses its Player Index when e.g. Initializing Health Component*/
void AEternalGrace_ProtoCharacter::LateInitialization()
{
	//Health
	if (HealthComponent)
	{
		/*Todo: Fix issue that Player Index is set in constructor. Because of this, that value will be used in begin play instead of the index set by gamemode after construction*/
		HealthComponent->ShowHPBar(UGameplayStatics::GetPlayerController(World, PlayerIndex));
	}
}

void AEternalGrace_ProtoCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	SetCurrentActionState(EActionState::Idle);
}

void AEternalGrace_ProtoCharacter::Move(const FInputActionValue& Value)
{
	if (CurrentActionState != EActionState::Climbing)
	{
		// input is a Vector2D
		FVector2D MovementVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
	}

}

void AEternalGrace_ProtoCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AEternalGrace_ProtoCharacter::SaveData_Implementation()
{
	//Call General SaveData Behaviour. Its probably just a Log announcing that the Save Function gets triggered.
	Super::SaveData_Implementation();

	SaveGameObject = CurrentGameInstance->GetCurrentSaveGame();
	if (SaveGameObject)
	{
		//Update localSaveData Struct
		SaveDataInfo.UpdatePlayerData(GetActorTransform(), UpperArmor->GetArmorName(), UpperArmor->GetPantsName(), UpperArmor->GetHelmetsName(), &PlayerInventory->ArmorInventoryMap, &PlayerInventory->PantsInventoryMap, &PlayerInventory->HelmetInventoryMap, &PlayerInventory->WeaponInventory, &WeaponComponent->CurrentWeaponClass, &WeaponComponent->OffhandWeaponClass, &PlayerInventory->KeyItemInventoryMap);
		//Actually Save Data from Struct To SaveGame
		SaveGameObject->SavePlayerData(ObjectID, SaveDataInfo);
		UE_LOG(LogTemp, Warning, TEXT("%s called SavePlayerData on SaveGameID %s"), *ObjectID.ToString(), *SaveGameObject->SlotID);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SaveGameObject Could not be casted into UEternalGraceSaveGame. This Function was called by SaveData of %s"), *ObjectID.ToString())
	}
}


void AEternalGrace_ProtoCharacter::LoadData_Implementation()
{
	//Call General SaveData Behaviour. Its probably just a Log announcing that the  Function gets triggered.
	Super::LoadData_Implementation();

	SaveGameObject = CurrentGameInstance->GetCurrentSaveGame();
	if (SaveGameObject)
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentSaveGamesName is %s"), *SaveGameObject->SlotID);
		FPlayerSaveData* SaveData = SaveGameObject->LoadPlayerData(ObjectID);
		if (SaveData)
		{
			SaveDataInfo = *SaveData;

			UE_LOG(LogTemp, Display, TEXT("LOADING SUCESSFULL SaveDataMap Contained Object ID %s"), *ObjectID.ToString())
				SetActorTransform(SaveDataInfo.PlayerTransform);
			ChangeUpperArmor(SaveDataInfo.CurrentBreastArmorName);
			ChangePants(SaveDataInfo.CurrentPantsName);
			ChangeHelmet(SaveDataInfo.CurrentHelmetsName);
			EquipWeapon(SaveDataInfo.CurrentWeapon);
			EquipOffhandWeapon(SaveDataInfo.CurrentOffHandWeapon);
			//Load Inventory
			PlayerInventory->ArmorInventoryMap = SaveDataInfo.SavedArmorDataMap;
			PlayerInventory->PantsInventoryMap = SaveDataInfo.SavedPantsDataMap;
			PlayerInventory->HelmetInventoryMap = SaveDataInfo.SavedHelmetDataMap;
			PlayerInventory->WeaponInventory = SaveDataInfo.SavedWeaponInventory;
			PlayerInventory->KeyItemInventoryMap = SaveDataInfo.SavedKeyItemDataMap;


		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LOADING FAILED SaveDataMap does NOT Contained Object ID %s"), *ObjectID.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SaveGameObject Could not be casted into UEternalGraceSaveGame. This Function was called by LoadData of %s"), *GetName());
	}

}

void AEternalGrace_ProtoCharacter::Interact_Implementation()
{
	if (CurrentInteractable)
	{
		if (CurrentInteractable->Implements<UInteractable>()) //This if is almost redundant
		{
			IInteractable::Execute_GetInteractedWith(CurrentInteractable, this);
			IPlayable::Execute_HideInteractUI(this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s Does not implements Interactable Interface"), *CurrentInteractable->GetFName().ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No CurrentInteractable"));
	}
}

void AEternalGrace_ProtoCharacter::ShowInteractUI_Implementation(AActor* ActorToInteractWith)
{
	UE_LOG(LogTemp, Display, TEXT("Player shows Interact Interface"));
	//Get player Controller
	APlayerController* PossessingController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex); //<< Fehler hier
	AEG_PlayerController* EGController = Cast<AEG_PlayerController>(PossessingController);
	if (EGController)
	{
		//Send InteractedObject Info to Player Controller to tell which InteractUI to Show. The Controller probably holds a Pointer to the UI so he only shows one at an time.
		//Call ShowInteractUI Function of Controller
		EGController->ShowInteractInfoWidget();
		//Set Current Interactable
		CurrentInteractable = ActorToInteractWith;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Cast PlayerController in Interact Implementation (ProtoCharacter Class)"));
	}
}

void AEternalGrace_ProtoCharacter::HideInteractUI_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("Player hides Interact Interface"));
	//Get player Controller
	APlayerController* PossessingController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex);
	AEG_PlayerController* EGController = Cast<AEG_PlayerController>(PossessingController);
	if (EGController)
	{
		//Call HiInteractUI Function of Controller
		EGController->HideInteractInfoWidget();
		//Remove Current Interactable
		CurrentInteractable = nullptr;
	}
}

UCapsuleComponent* AEternalGrace_ProtoCharacter::GetHitBox_Implementation()
{
	if (WeaponComponent->CurrentWeaponObject)
	{
		return WeaponComponent->CurrentWeaponObject->GetHitbox();
	}
	UE_LOG(LogTemp, Error, TEXT("Failed to get Current Weapon in Attempt to get hitbox (ProtoCharacterClass)"))
		return nullptr;
}

AWeaponBase* AEternalGrace_ProtoCharacter::GetWeapon_Implementation()
{
	return WeaponComponent->CurrentWeaponObject;
}

AWeaponBase* AEternalGrace_ProtoCharacter::GetOffhandWeapon_Implementation()
{
	if (WeaponComponent->OffhandWeaponObject)
	{
		return WeaponComponent->OffhandWeaponObject;
	}
	else if (ShieldComponent->GetCurrentShieldObject())
	{
		return ShieldComponent->GetCurrentShieldObject();
	}
	else
	return nullptr;
}

void AEternalGrace_ProtoCharacter::Die_Implementation()
{
	Super::Die_Implementation();
	AEG_PlayerController* PlayerController = Cast<AEG_PlayerController>(UGameplayStatics::GetPlayerController(World,PlayerIndex));
	if(PlayerController)
	{
		PlayerController->HandlePlayerDeath(CurrentGameInstance);
	}
}

void AEternalGrace_ProtoCharacter::Stagger_Implementation(EAttackDirection Direction, float PoiseDamage, AActor* DamageInstigator)
{
	if (StaggerComponent)
	{
		bool bCharacterGetStaggered = StaggerComponent->GetStaggered(Direction, PoiseDamage, DamageInstigator);
		if (bCharacterGetStaggered == true)
		{
			SetCurrentActionState(EActionState::Staggered);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no StaggerComponent! (Enemy Class)"), *GetFName().ToString());
	}
}

void AEternalGrace_ProtoCharacter::PlayFootStepSound(FName FootSocket)
{
	//Check if Character wears armor and play it if true
	//Check Data Table for Ground Material and play assigned Sound
	UPhysicalMaterial* GroundMaterial = GetGroundMaterial();
	if (GroundMaterial && GroundSurfaceDataTable)
	{
		static const FString Context = FString("GroundContext");
		FSurfaceType* SurfaceRow = GroundSurfaceDataTable->FindRow<FSurfaceType>(GroundMaterial->GetFName(), Context);
		if (SurfaceRow)
		{
			UGameplayStatics::PlaySoundAtLocation(World, SurfaceRow->FootStepSound, FVector(GetMesh()->GetSocketLocation(FootSocket)));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SurfaceRow not found (Eternal grace proto character)"));
		}
	}
	if (UpperArmor->bIsWearingHeavyArmor)
	{
		UpperArmor->GetArmorSoundComponent()->Play();
	}
}

void AEternalGrace_ProtoCharacter::SetCurrentActionState(EActionState ActionState)
{
	Super::SetCurrentActionState(ActionState);
	EGAnimInstance->CurrentActionState = ActionState;
}


void AEternalGrace_ProtoCharacter::CancelGuard()
{
	//This is only triggered when Guard Button is let off of
	if (CurrentActionState == EActionState::Guarding)
	{
		SetCurrentActionState(EActionState::Idle);
	}
}

void AEternalGrace_ProtoCharacter::Jump()
{

	switch (CurrentActionState)
	{
	case EActionState::Idle:
		SetCurrentActionState(EActionState::Jumping);
		Super::Jump();
		break;
	case EActionState::Running:
		SetCurrentActionState(EActionState::Jumping);
		Super::Jump();
		break;
	default:
		InputBufferingComponent->SaveInput(EInputType::Jump);
		break;
	}
}
