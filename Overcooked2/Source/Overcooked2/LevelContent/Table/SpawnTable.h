// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Table/CookingTable.h"
#include <Global/OC2Enum.h>
#include "SpawnTable.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ASpawnTable : public ACookingTable
{
	GENERATED_BODY()
	
public:
	ASpawnTable();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetIngredient(EIngredientType IngredientTypeSetting);

	UFUNCTION(BlueprintCallable)
	class AIngredient* SpawnIngredient(AActor* ChefActor/*, EIngredientType Ingredient*/);

	UFUNCTION(BlueprintCallable)
	virtual ACooking* Interact(AActor* ChefActor) override;

	UFUNCTION(BlueprintCallable)
	virtual void PlaceItem(ACooking* Item) override;
protected:

private:
	
	EIngredientType IngredientType = EIngredientType::EIT_NONE;
	ACooking* PlacedItem = nullptr;
};
