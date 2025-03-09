// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Dev/CookingDevUserWidget.h"
#include <LevelContent/Cook/Ingredient.h>
#include "Kismet/GameplayStatics.h"
#include <LevelContent/Cook/Dev/CookingDevGameMode.h>
#include <Global/OC2Global.h>

void UCookingDevUserWidget::ServerSpawnIngredient_Implementation(EIngredientType Type)
{
	APlayerController* Controller = GetOwningPlayer();
	if (nullptr == Controller)
	{
		int a = 0;
	}

	ACookingDevGameMode* GameMode = Cast<ACookingDevGameMode>(GetWorld()->GetAuthGameMode());
	if (nullptr != GameMode)
	{
		GameMode->SpawnIngredient(Type);
	}
}

void UCookingDevUserWidget::ChangeState(EIngredientState State)
{
	if (true == CookingActor.IsEmpty())
	{
		return;
	}

	for (int i = 0; i < CookingActor.Num(); i++)
	{
		AIngredient* Ingredient = Cast<AIngredient>(CookingActor[i]);
		if (State == Ingredient->GetCurIngredientState())
		{
			return;
		}
		else
		{
			Ingredient->ChangeState(State);
		}
	}
}

void UCookingDevUserWidget::StartServer()
{
	FString OpenLevel = TEXT("");
	FString LevelPath = TEXT(""); // 레벨의 폴더 경로를 저장할 변수

	UOC2Global::GetAssetPackageName(UWorld::StaticClass(), TEXT("CookingDevLevel"), LevelPath);
	OpenLevel = FString::Printf(TEXT(":%s%s"), *Port, *LevelPath);
	// :30000/Game/Map/CookingDevLevel

	UGameplayStatics::OpenLevel(GetWorld(), *OpenLevel, true, TEXT("listen"));
}

void UCookingDevUserWidget::Connect()
{
	FString ConnectLevelName = FString::Printf(TEXT("%s:%s"), *IP, *Port);
	// 127.0.0.1:30000

	UGameplayStatics::OpenLevel(GetWorld(), FName(*ConnectLevelName));

	// Debug
	UE_LOG(LogTemp, Warning, TEXT("ConnectLevelName: %s"), *ConnectLevelName);
}
