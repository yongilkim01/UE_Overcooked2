// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/CookingTable.h"
#include "Global/GameMode/OC2GameMode.h"
#include <Global/Data/IngredientDataTable.h>
#include <Global/OC2GameInstance.h>
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "LevelContent/Cook/Plate.h"
#include "Global/Data/OC2GlobalData.h"
#include "Kismet/GameplayStatics.h"

//#include "Global/Component/TimeEventComponent.h"

// Sets default values
ACookingTable::ACookingTable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	ComponentForCooking = CreateDefaultSubobject<USceneComponent>("Attachment");
	ComponentForCooking->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACookingTable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACookingTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACookingTable::PlaceItem(ACooking* ReceivedCooking)
{
	if (nullptr == ReceivedCooking)
	{
		return;
	}

	CookingPtr = ReceivedCooking;
	CookingPtr->SetCookingTable_Implementation(this);
	CookingPtr->AttachToComponent(ComponentForCooking, FAttachmentTransformRules::KeepRelativeTransform);
	CookingPtr->SetActorLocation(ComponentForCooking->GetComponentLocation());
	
	AIngredient* TempIngredient = Cast<AIngredient>(CookingPtr);
	if (nullptr != TempIngredient)
	{
		SetIngredientOffset(TempIngredient);
		CookingPtr->GetStaticMeshComponent()->SetRelativeRotation(IngreRotation);
		CookingPtr->GetStaticMeshComponent()->SetRelativeScale3D(IngreScale);
		CookingPtr->SetActorRelativeLocation(IngreLocation);

		if (EIngredientType::EIT_CUCUMBER == TempIngredient->GetIngredientType())
		{
			CookingPtr->AddActorWorldOffset(FVector::UnitZ() * 10.0f);
			CookingPtr->AddActorWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
		}

		if (EIngredientType::EIT_PRAWN == TempIngredient->GetIngredientType() && EIngredientState::EIS_CHOPPED ==TempIngredient->GetCurIngredientState())
		{
			CookingPtr->SetActorRelativeLocation(FVector::UnitX() * 10.0f + FVector::UnitY() * (-10.0f) /*+ FVector::UnitZ() * (-50.0f)*/);//AddActorWorldOffset(/*FVector::UnitX() * 15.0f + FVector::UnitY() * (-20.0f) +*/ FVector::UnitZ() * (-50.0f));
			CookingPtr->SetActorRelativeRotation(FRotator(0.0f, 90.0f, 100.0f));//SetActorWorldRotation(FRotator(0.0f, 90.0f, 135.0f));
		}
	}

	PlacingSoundEffect();
}

void ACookingTable::SetIngredientOffset(AIngredient* Ingredient)
{
	EIngredientType Type = Ingredient->GetIngredientType();
	UOC2GameInstance* GameInst = Cast<UOC2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FName Name = GameInst->GetIngredientDataTableRowName(Type);

	const FIngredientDataRow* IngredientDataTable = &GameInst->GetIngredientDataRow(Name);
	IngreRotation = IngredientDataTable->Rotation;
	IngreLocation = IngredientDataTable->Location;
	IngreScale = IngredientDataTable->Scale;
}

void ACookingTable::PlacingSoundEffect_Implementation()
{
	UGameplayStatics::PlaySound2D(GetWorld(), UOC2GlobalData::GetCharacterBaseSound(GetWorld(), "Putdown"));
}

void ACookingTable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACookingTable, CookingPtr);
}
