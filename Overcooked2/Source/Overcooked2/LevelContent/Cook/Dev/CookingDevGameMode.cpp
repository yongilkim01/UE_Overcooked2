// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Dev/CookingDevGameMode.h"
#include <LevelContent/Cook/Ingredient.h>
#include <LevelContent/Cook/Dev/CookingDevHUD.h>
#include <LevelContent/Cook/Dev/CookingDevUserWidget.h>

ACookingDevGameMode::ACookingDevGameMode()
{
}

AIngredient* ACookingDevGameMode::SpawnIngredient(EIngredientType Type)
{
	FTransform Trans;
	AIngredient* Ingredient = GetWorld()->SpawnActorDeferred<AIngredient>(AIngredient::StaticClass(), Trans);

	Ingredient->Init(Type);

	FVector Location = FVector(0.0f, 0.0f, 100.0f);
	Trans.SetLocation(Location);

	Ingredient->FinishSpawning(Trans);

	if (nullptr != Widget)
	{
		Widget->AddTargetActor(Ingredient);
	}

	return Ingredient;
}

void ACookingDevGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ACookingDevGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
