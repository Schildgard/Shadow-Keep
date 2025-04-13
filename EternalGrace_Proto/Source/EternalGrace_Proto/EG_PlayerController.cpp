// Fill out your copyright notice in the Description page of Project Settings.


#include "EG_PlayerController.h"
#include "Components/Button.h"
#include "InventorySlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Inventory.h"
#include "Components/WrapBox.h"
#include "ArmorComponent.h"
#include "InventoryComponent.h"
#include "Armor.h"
#include "Pants.h"
#include "WeaponBase.h"
#include "ObtainWidget.h"
#include "ItemObtainContainerWidget.h"
#include "Components/StackBox.h"
#include "BlendingWidget.h"
#include "EternalGrace_GameInstance.h"
#include "Kismet/GameplayStatics.h"

AEG_PlayerController::AEG_PlayerController()
{
	InventoryClass = nullptr;
	InventorySlotClass = nullptr;
	InventoryObjectButtonWidget = nullptr;
	OwningCharacter = nullptr; // Owning Character is set by the Game Mode. It can not be setted in Begin Play because the Game Mode does spawn no default pawn.
}

void AEG_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}


void AEG_PlayerController::ShowYouDiedScreen()
{
	if(YouDiedScreenClass)
	{
		YouDiedWidget = CreateWidget<UBlendingWidget>(this, YouDiedScreenClass);
		if(YouDiedWidget)
		{
			//Intentionally dont add to PlayerScreen, because when Multiplayer this Widgets only pups up when both Players die
			YouDiedWidget->AddToViewport();
			YouDiedWidget->PlayAnimation(YouDiedWidget->BlendingAnimation);
			DisableInput(this);
			FWidgetAnimationDynamicEvent ReturnToMainMenuDelegate;
			ReturnToMainMenuDelegate.BindDynamic(this, &AEG_PlayerController::ReturnToMainMenu);
			YouDiedWidget->BindToAnimationFinished(YouDiedWidget->BlendOutAnimation, ReturnToMainMenuDelegate);
			//Enable Input later
		}
	}
}

void AEG_PlayerController::HideYouDiedScreen()
{
	if(YouDiedWidget && YouDiedWidget->IsInViewport())
	{
		YouDiedWidget->RemoveFromRoot();
	}
}

void AEG_PlayerController::HandlePlayerDeath(UEternalGrace_GameInstance* CurrentGameInstance)
{
	ShowYouDiedScreen();
	//DELETE SAVEGAME
	CurrentGameInstance->DeleteFile(CurrentGameInstance->GetActiveSaveGameSlot());
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(CurrentGameInstance, &UEternalGrace_GameInstance::ResetGameStatus);
	//RESET GAMEINSTANCE VALUES
	//CurrentGameInstance->ResetGameStatus();
	//REMOVE Second Player Splitscreen
}

void AEG_PlayerController::ShowInventory()
{
	//Check if Inventory Class is assigned
	if (InventoryClass)
	{
		//Check if Inventory is already open. If so, just close and empty it.
		if (CurrentInventory && CurrentInventory->IsInViewport())
		{
			CurrentInventorySlot.Empty();
			CurrentInventory->RemoveFromParent();
			FInputModeGameOnly GameOnlyInputMode;
			SetInputMode(GameOnlyInputMode);
			bShowMouseCursor = false;

			return;
		}
		CurrentInventory = CreateWidget<UInventory>(this, InventoryClass);
		if (CurrentInventory)
		{
			//Add Inventory to viewport and enable Mouse Cursor
			CurrentInventory->AddToPlayerScreen();
			bShowMouseCursor = true;

			//Check if General Class for Inventory Slot is assigned
			if (InventorySlotClass)
			{
				UWrapBox* InventoryWrapBox = CurrentInventory->GetWrapBox();

				int z = 0;
				static const FString Context2(TEXT("TestContext"));
				for (TPair<FName, int> ArmorData : OwningCharacter->GetInventory()->ArmorInventoryMap)
				{
					FArmor* Armor = OwningCharacter->GetInventory()->GlobalArmorInventory->FindRow<FArmor>(ArmorData.Key, Context2, true);

					CurrentInventorySlot.Add(CreateWidget<UInventorySlot>(this, InventorySlotClass));
					if (CurrentInventorySlot[z])
					{
						CurrentInventorySlot[z]->AddToPlayerScreen();
						InventoryWrapBox->AddChildToWrapBox(CurrentInventorySlot[z]);
						CurrentInventorySlot[z]->SetImage(Armor->ThumpNailImage.LoadSynchronous());
						CurrentInventorySlot[z]->SetItemName(Armor->ArmorName);
						CurrentInventorySlot[z]->SetObjectType(Armor->ObjectType);
						CurrentInventorySlot[z]->GetSlotButton()->OnClicked.AddDynamic(CurrentInventorySlot[z], &UInventorySlot::UseInventorySlot);
					}
					z++;

				}

				;
				for (TPair<FName, int> ArmorData : OwningCharacter->GetInventory()->PantsInventoryMap)
				{
					FPants* Armor = OwningCharacter->GetInventory()->GlobalPantsInventory->FindRow<FPants>(ArmorData.Key, Context2, true);

					CurrentInventorySlot.Add(CreateWidget<UInventorySlot>(this, InventorySlotClass));
					if (CurrentInventorySlot[z])
					{
						CurrentInventorySlot[z]->AddToPlayerScreen();
						InventoryWrapBox->AddChildToWrapBox(CurrentInventorySlot[z]);
						CurrentInventorySlot[z]->SetImage(Armor->ThumpNailImage.LoadSynchronous());
						CurrentInventorySlot[z]->SetItemName(Armor->PantsName);
						CurrentInventorySlot[z]->SetObjectType(Armor->ObjectType);
						CurrentInventorySlot[z]->GetSlotButton()->OnClicked.AddDynamic(CurrentInventorySlot[z], &UInventorySlot::UseInventorySlot);
					}
					z++;

				}


				for (TPair<FName, int> ArmorData : OwningCharacter->GetInventory()->HelmetInventoryMap)
				{
					FHelmet* Armor = OwningCharacter->GetInventory()->GlobalHelmetInventory->FindRow<FHelmet>(ArmorData.Key, Context2, true);

					CurrentInventorySlot.Add(CreateWidget<UInventorySlot>(this, InventorySlotClass));
					if (CurrentInventorySlot[z])
					{
						CurrentInventorySlot[z]->AddToPlayerScreen();
						InventoryWrapBox->AddChildToWrapBox(CurrentInventorySlot[z]);
						CurrentInventorySlot[z]->SetImage(Armor->ThumpNailImage.LoadSynchronous());
						CurrentInventorySlot[z]->SetItemName(Armor->HelmetName);
						CurrentInventorySlot[z]->SetObjectType(Armor->ObjectType);
						CurrentInventorySlot[z]->GetSlotButton()->OnClicked.AddDynamic(CurrentInventorySlot[z], &UInventorySlot::UseInventorySlot);
					}
					z++;

				}

				for (TSubclassOf<AWeaponBase> Weapon : OwningCharacter->GetInventory()->WeaponInventory)
				{
					CurrentInventorySlot.Add(CreateWidget<UInventorySlot>(this, InventorySlotClass));
					if (CurrentInventorySlot[z])
					{
						//create reference instance to get values
						AWeaponBase* WeaponInstance = Weapon->GetDefaultObject<AWeaponBase>();

						CurrentInventorySlot[z]->AddToPlayerScreen();
						InventoryWrapBox->AddChildToWrapBox(CurrentInventorySlot[z]);
						CurrentInventorySlot[z]->SetImage(WeaponInstance->ThumpnailImage.LoadSynchronous());
						CurrentInventorySlot[z]->SetWeapon(Weapon);
						CurrentInventorySlot[z]->SetObjectType(EObjectType::Weapon);
						CurrentInventorySlot[z]->GetSlotButton()->OnClicked.AddDynamic(CurrentInventorySlot[z], &UInventorySlot::UseInventorySlot);
					}
					z++;
				}

				if (CurrentInventorySlot.Num() >= 1)
				{
					CurrentInventorySlot[0]->SetUserFocus(this);
				}

				FInputModeGameAndUI InputMode;
				SetInputMode(InputMode);
			}
		}
	}
}

void AEG_PlayerController::ShowInteractInfoWidget()
{
	UE_LOG(LogTemp, Display, TEXT("PlayerController now opens InteractInfoWidget"));
	if (InteractInfoWidgetClass && !InteractInfoWidget)
	{
		InteractInfoWidget = CreateWidget<UUserWidget>(this, InteractInfoWidgetClass);
		InteractInfoWidget->AddToPlayerScreen();
	}

}

void AEG_PlayerController::HideInteractInfoWidget()
{
	if (InteractInfoWidget && InteractInfoWidget->IsInViewport())
	{
		InteractInfoWidget->RemoveFromParent();
		InteractInfoWidget = nullptr;
	}
}

AEternalGrace_ProtoCharacter* AEG_PlayerController::GetOwningCharacter()
{
	return OwningCharacter;
}

void AEG_PlayerController::SetOwningCharacterVariable(AEternalGrace_ProtoCharacter* SetCharacter)
{
	OwningCharacter = SetCharacter;
}

void AEG_PlayerController::ShowObtainWidget(FName ObjectID, TSoftObjectPtr<UTexture2D> ItemImage)
{
	if (ObtainedItemsContainerClass)
	{
		if (!ObtainedItemsContainer)
		{
			ObtainedItemsContainer = CreateWidget<UItemObtainContainerWidget>(this, ObtainedItemsContainerClass);

		}
		if (ObtainedItemsContainer && !ObtainedItemsContainer->IsInViewport())
		{
			ObtainedItemsContainer->AddToPlayerScreen();
			ObtainedItemsContainer->SetUserFocus(this);
		}
		if (ObtainWidgetClass)
		{
			UObtainWidget* Obtain = CreateWidget<UObtainWidget>(this, ObtainWidgetClass);
			Obtain->InitializeWidget(ObjectID, ItemImage);
			ObtainedItemsContainer->StackBox->AddChildToStackBox(Obtain);
		}
	}
}

void AEG_PlayerController::ReturnToMainMenu()
{
	// Gets Calle via Delegate when Death Screen is finished Blending out
	//Load MainMenu
	UGameplayStatics::OpenLevel(GetWorld(), "Map_MainMenu");
	EnableInput(this);
}
