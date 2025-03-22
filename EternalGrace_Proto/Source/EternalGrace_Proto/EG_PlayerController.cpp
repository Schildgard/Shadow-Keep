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

		/*		//iterate through Armor Data Table
				TArray<FArmor*> Table;
				static const FString Context(TEXT("Context"));
				OwningCharacter->GetInventory()->GetArmorInventory()->GetAllRows<FArmor>(Context, Table);

				int i = 0;
				for (FArmor* Armor : Table)
				{
					//create an Inventory Slot for each row of the Inventory and assign Data to Slot
					CurrentInventorySlot.Add(CreateWidget<UInventorySlot>(this, InventorySlotClass));
					if (CurrentInventorySlot[i])
					{
						//CurrentInventorySlot[i]->AddToPlayerScreen();
						InventoryWrapBox->AddChildToWrapBox(CurrentInventorySlot[i]);
						//assign here Object Type and Name to InventorySlot
						CurrentInventorySlot[i]->SetImage(Armor->ThumpNailImage.LoadSynchronous());
						CurrentInventorySlot[i]->SetItemName(Armor->ArmorName);
						CurrentInventorySlot[i]->SetObjectType(Armor->ObjectType);
						CurrentInventorySlot[i]->GetSlotButton()->OnClicked.AddDynamic(CurrentInventorySlot[i], &UInventorySlot::UseInventorySlot);
						i++;
						UE_LOG(LogTemp, Warning, TEXT("Creat Slot for %s"), *Armor->ArmorName.ToString())
					}

				}

				//iterate through Pants Data Table
				TArray<FPants*> PantsTable;
				static const FString PantsContext(TEXT("PantsContext"));
				OwningCharacter->GetInventory()->GetPantsInventory()->GetAllRows<FPants>(PantsContext, PantsTable);
				int j = i;
				for (FPants* Pants : PantsTable)
				{
					//create an Inventory Slot for each row of the Inventory and assign Data to Slot
					CurrentInventorySlot.Add(CreateWidget<UInventorySlot>(this, InventorySlotClass));
					if (CurrentInventorySlot[j])
					{
						CurrentInventorySlot[j]->AddToPlayerScreen();
						InventoryWrapBox->AddChildToWrapBox(CurrentInventorySlot[j]);
						CurrentInventorySlot[j]->SetImage(Pants->ThumpNailImage.LoadSynchronous());
						CurrentInventorySlot[j]->SetItemName(Pants->PantsName);
						CurrentInventorySlot[j]->SetObjectType(Pants->ObjectType);
						CurrentInventorySlot[j]->GetSlotButton()->OnClicked.AddDynamic(CurrentInventorySlot[j], &UInventorySlot::UseInventorySlot);
						j++;
						UE_LOG(LogTemp, Warning, TEXT("Creat Slot for %s"), *Pants->PantsName.ToString())
					}
				}

				//iterate through Pants Data Table
				TArray<FHelmet*> HelmetTable;
				static const FString HelmetContext(TEXT("HelmetContext"));
				OwningCharacter->GetInventory()->GetHelmetInventory()->GetAllRows<FHelmet>(HelmetContext, HelmetTable);
				int u = j;
				for (FHelmet* Helmet : HelmetTable)
				{
					//create an Inventory Slot for each row of the Inventory and assign Data to Slot
					CurrentInventorySlot.Add(CreateWidget<UInventorySlot>(this, InventorySlotClass));
					if (CurrentInventorySlot[u])
					{
						CurrentInventorySlot[u]->AddToPlayerScreen();
						InventoryWrapBox->AddChildToWrapBox(CurrentInventorySlot[u]);
						CurrentInventorySlot[u]->SetImage(Helmet->ThumpNailImage.LoadSynchronous());
						CurrentInventorySlot[u]->SetItemName(Helmet->HelmetName);
						CurrentInventorySlot[u]->SetObjectType(Helmet->ObjectType);
						CurrentInventorySlot[u]->GetSlotButton()->OnClicked.AddDynamic(CurrentInventorySlot[u], &UInventorySlot::UseInventorySlot);
						u++;
						UE_LOG(LogTemp, Warning, TEXT("Creat Slot for %s"), *Helmet->HelmetName.ToString())
					}
				} */
				//TEST: iteriere durch inventory (TMap. Hole für jedes Item die Daten aus dem Data Table.

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
