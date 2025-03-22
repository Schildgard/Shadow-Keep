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
#include "EternalGrace_GameInstance.h"
#include "ArmorComponent.h"
#include "EG_PlayerController.h"
#include "InventoryComponent.h"
#include "Armor.h"
#include "Pants.h"
#include "Interactable.h"
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

	UpperArmor->GetHelmetMesh()->SetupAttachment(HeadMesh);
	UpperArmor->GetHelmetMesh()->SetLeaderPoseComponent(HeadMesh);

	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");

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

void AEternalGrace_ProtoCharacter::TriggerCurrentInteractable()
{
	Interact_Implementation();
}

void AEternalGrace_ProtoCharacter::BeginPlay()
{
	Super::BeginPlay();
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

void AEternalGrace_ProtoCharacter::Move(const FInputActionValue& Value)
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
	//	SaveDataInfo.SetPlayerTransform(GetActorTransform(), UpperArmor->GetArmorName(), UpperArmor->GetPantsName(), UpperArmor->GetHelmetsName(), PlayerInventory->GetArmorInventory(), PlayerInventory->GetPantsInventory(), PlayerInventory->GetHelmetInventory());
		SaveDataInfo.UpdatePlayerData(GetActorTransform(), UpperArmor->GetArmorName(), UpperArmor->GetPantsName(), UpperArmor->GetHelmetsName(), &PlayerInventory->ArmorInventoryMap, &PlayerInventory->PantsInventoryMap, &PlayerInventory->HelmetInventoryMap);
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
		UE_LOG(LogTemp, Error, TEXT("CUrrentSaveGamesName is %s"), *SaveGameObject->SlotID);
		FPlayerSaveData* SaveData = SaveGameObject->LoadPlayerData(ObjectID);
		if (SaveData)
		{
			SaveDataInfo = *SaveData;

			UE_LOG(LogTemp, Display, TEXT("LOADING SUCESSFULL SaveDataMap Contained Object ID %s"), *ObjectID.ToString())
				SetActorTransform(SaveDataInfo.PlayerTransform);
			ChangeUpperArmor(SaveDataInfo.CurrentBreastArmorName);
			ChangePants(SaveDataInfo.CurrentPantsName);
			ChangeHelmet(SaveDataInfo.CurrentHelmetsName);
			//Load Inventory
		//	for (FArmor Armor : SaveDataInfo.SavedArmorData)
		//	{
		//		PlayerInventory->AddArmorToInventory(Armor);
		//		UE_LOG(LogTemp, Display, TEXT("Loaded %s into Inventory from Savefile"), *Armor.ArmorName.ToString())
		//	}
		//	for (FPants Pants : SaveDataInfo.SavedPantsData)
		//	{
		//		PlayerInventory->AddPantsToInventory(Pants);
		//		UE_LOG(LogTemp, Display, TEXT("Loaded %s into Inventory from Savefile"), *Pants.PantsName.ToString())
		//	}
		//	for (FHelmet Helmet : SaveDataInfo.SavedHelmetData)
		//	{
		//		PlayerInventory->AddHelmetToInventory(Helmet);
		//		UE_LOG(LogTemp, Display, TEXT("Loaded %s into Inventory from Savefile"), *Helmet.HelmetName.ToString());
		//	}
			PlayerInventory->ArmorInventoryMap.Empty();
			PlayerInventory->ArmorInventoryMap.Append(SaveDataInfo.SavedArmorDataMap);
			PlayerInventory->PantsInventoryMap.Empty();
			PlayerInventory->PantsInventoryMap.Append(SaveDataInfo.SavedPantsDataMap);
			PlayerInventory->HelmetInventoryMap.Empty();
			PlayerInventory->HelmetInventoryMap.Append(SaveDataInfo.SavedHelmetDataMap);


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
	UE_LOG(LogTemp, Display, TEXT("Player calls Interact Implementation"));
	if (CurrentInteractable)
	{
		if (CurrentInteractable->Implements<UInteractable>()) //This if is almost redundant
		{
			IInteractable::Execute_GetInteractedWith(CurrentInteractable, this);
			Execute_SaveData(this);
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
