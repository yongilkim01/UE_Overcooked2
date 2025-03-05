// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Plate.h"
#include <LevelContent/Cook/Ingredient.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
APlate::APlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CookingType = ECookingType::ECT_PLATE;

	IngredientMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IngredientMesh"));
	//PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlateMesh"));
	//RootComponent = PlateMesh;

	Rotation = FRotator(0.0f, 0.0f, 90.0f);
	SetActorRelativeRotation(Rotation);

	//FVector Offset = GetActorLocation() + FVector(0.0f, 0.0f, 30.0f);
	//IngredientMesh->AddRelativeLocation(Offset);

}

// Called when the game starts or when spawned
void APlate::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool APlate::IsDirtyPlate()
{
	if (nullptr == StaticMeshComponent)
	{
		return false;
	}

	return PlateState == EPlateState::DIRTY;
}

void APlate::WashPlate()
{
	if (true == IsDirtyPlate())
	{
		PlateState = EPlateState::EMPTY;
	}
}

void APlate::LoadDataTable(AIngredient* Ingredient)
{
	if (nullptr == IngredientDataTable)
	{
		IngredientDataTable = Ingredient->GetIngredientDataTable();
	}
	if (true == CookingDataTable.IsEmpty())
	{
		UOC2GameInstance* GameInst = Cast<UOC2GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		FName Name = GameInst->GetIngredientDataTableRowName(Ingredient->GetIngredientType());
		CookingDataTable = GameInst->GetIngredientCookDataRows(Name.ToString());
	}
}

void APlate::CheckAndChangeState(AIngredient* Ingredient)
{
	if (PrveState != Ingredient->GetCurIngredientState())
	{
		PrveState = Ingredient->GetCurIngredientState();

		for (int i = 0; i < CookingDataTable.Num(); i++)
		{
			if (Ingredient->GetCurIngredientState() == CookingDataTable[i].IngredientState)
			{
				Ingredient->GetStaticMeshComponent()->SetStaticMesh(CookingDataTable[i].CookMesh);
				Ingredient->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				Ingredient->SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
			}
		}
	}
}

bool APlate::Add(AIngredient* Ingredient)
{
	if (EIngredientState::EIS_NONE == Ingredient->GetCurIngredientState())
	{
		return false;
	}
	if (nullptr == StaticMeshComponent)
	{
		return false;
	}

	// 1. DataTable이 null이면 세팅
	LoadDataTable(Ingredient);

	// 2. IngredinetType이 바뀌었는지 체크하고 메시 변환
	CheckAndChangeState(Ingredient);

	Ingredients.Add(Ingredient);

	CookCheck();

	return true;
}

void APlate::CookCheck()
{

}