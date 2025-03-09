// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <LevelContent/Cook/Cooking.h>
#include <Global/OC2Enum.h>
#include "CookingDevUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API UCookingDevUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Network
	UFUNCTION(BlueprintCallable, Reliable, Server)
	void ServerSpawnIngredient(EIngredientType Type);
	void ServerSpawnIngredient_Implementation(EIngredientType Type);


	UFUNCTION(BlueprintCallable)
	void AddTargetActor(ACooking* Actor)
	{
		if (false == CookingActor.IsEmpty())
		{
			ACooking* NewActor = CookingActor.Last();
			FVector Offset = FVector(0.0f, 50.0f, 0.0f);
			FVector AdjustLocation = Offset + NewActor->GetActorLocation();
			Actor->SetActorLocation(AdjustLocation);
		}
	
		CookingActor.Add(Actor);
	}

	UFUNCTION(BlueprintCallable)
	TArray<ACooking*>& GetTargetActor()
	{
		return CookingActor;
	}

	UFUNCTION(BlueprintCallable)
	ACooking* GetTargetActorIndex(int Index)
	{
		return CookingActor[Index];
	}

	UFUNCTION(BlueprintCallable)
	void Reset()
	{
		CookingActor.Empty();
	}

	UFUNCTION(BlueprintCallable)
	void ChangeState(EIngredientState State);


	UFUNCTION(BlueprintCallable)
	void StartServer();

	UFUNCTION(BlueprintCallable)
	void Connect();

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	TArray<ACooking*> CookingActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network", meta = (AllowPrivateAccess = "true"))
	FString IP = TEXT("127.0.0.1");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network", meta = (AllowPrivateAccess = "true"))
	FString Port = TEXT("30000");
};
