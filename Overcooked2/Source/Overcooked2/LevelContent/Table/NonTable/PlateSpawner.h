// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Table/CookingTable.h"
#include "PlateSpawner.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API APlateSpawner : public ACookingTable
{
	GENERATED_BODY()

public:
	APlateSpawner();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SetPlate(class APlate* Plate);

	virtual ACooking* Interact(AActor* ChefActor) override;

	void PlaceItem(ACooking* ReceivedCooking) override;

	UFUNCTION(Reliable, NetMulticast)
	void GetOwnedPlate();
	void GetOwnedPlate_Implementation();
private:

	//int PlateNum = 0;

	//TMap<int, ACooking*> PlateMap;

	class APlate* OwnedPlate;
	ACooking* NewCooking = nullptr;
	bool bOwnedPlate = false;
};
