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

AEG_PlayerController::AEG_PlayerController()
{
	InventoryClass = nullptr;
	InventorySlotClass = nullptr;
	InventoryObjectButtonWidget = nullptr;
	OwningCharacter = nullptr;
}

void AEG_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}


void AEG_PlayerController::ShowInventory()
{
	//Check if Inventory Class is assigned
	if (InventoryClass)
	{
		//Check if Inventory is already open. If so, just close and empty it.
		if(CurrentInventory && CurrentInventory->IsInViewport())
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
				for(TPair<FName,int> ArmorData : OwningCharacter->GetInventory()->ArmorInventoryMap)
				{
					FArmor* Armor = OwningCharacter->GetInventory()->GlobalArmorInventory->FindRow<FArmor>(ArmorData.Key, Context2, true);

					CurrentInventorySlot.Add(CreateWidget<UInventorySlot>(this, InventorySlotClass));
					if(CurrentInventorySlot[z])
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

				if(CurrentInventorySlot.Num() >=1)
				{
					CurrentInventorySlot[0]->SetKeyboardFocus();
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
}

void AEG_PlayerController::HideInteractInfoWidget()
{
	if(InteractInfoWidget && InteractInfoWidget->IsInViewport())
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
