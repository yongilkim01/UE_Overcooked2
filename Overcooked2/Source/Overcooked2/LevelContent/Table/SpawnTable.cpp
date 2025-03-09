// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Table/SpawnTable.h"
#include <Global/Data/IngredientDataTable.h>
#include "Kismet/GameplayStatics.h"
#include <Character/OC2Character.h>

ASpawnTable::ASpawnTable()
{
}

void ASpawnTable::BeginPlay()
{
	Super::BeginPlay();

}

void ASpawnTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnTable::SetIngredient(EIngredientType IngredientTypeSetting)
{
	IngredientType = IngredientTypeSetting;
}


AIngredient* ASpawnTable::SpawnIngredient(AActor* ChefActor/*, EIngredientType IngredientType*/)
{

	FActorSpawnParameters SpawnParameters; // 적절한 오버로딩 함수 호출을 위해(회전값 추가), FActorSpawnParameters 사용
	FVector Location = FVector();
	/*if (nullptr != ChefActor)
	{
		Location = GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);
	}*/
	Location = GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);
	FRotator Rotator = FRotator::ZeroRotator;

	AIngredient* NewIngredient = GetWorld()->SpawnActor<AIngredient>(AIngredient::StaticClass(), Location, Rotator, SpawnParameters);
	NewIngredient->Init(IngredientType);

	//재료 월드에 스폰, Init

	// 액터에 부착
	if (nullptr != ChefActor)
	{
		//NewIngredient->Interact(ChefActor);
	}

	if (nullptr == NewIngredient)
	{
		UE_LOG(LogTemp, Warning, TEXT("액터 스폰에 실패했습니다."));
		return nullptr;
	}

	return NewIngredient;
}

ACooking* ASpawnTable::Interact(AActor* ChefActor)
{
	AOC2Character* Chef = Cast<AOC2Character>(ChefActor);
	ACooking* TempCooking = nullptr;
	
	if (false == bIsOccupied)
	{
		if (true == Chef->IsHolding())
		{
			// 박스 위는 비어있고 셰프는 무언가를 들고 있다. 
			PlaceItem(TempCooking);
		}
		else
		{
			SpawnIngredient(ChefActor);
		}
	}
	else
	{
		// 박스 위에 무언가가 있음
		return PlacedItem;

		//PlacedItem이 외부 Cooking과 상호작용 하는 경우
		// 1. 조합 가능한 경우
		// 2. 빈접시와 요리가 있는 접시의 경우
	}

}

void ASpawnTable::PlaceItem(ACooking* Item)
{
	ACooking* TempCooking = Item;
	
	/*ECookingType TempCookingType = TempCooking->GetCookingType();

	if (ECookingType::ECT_INGREDIENT == TempCookingType)
	{

	}
	else
	{

	}*/
}
