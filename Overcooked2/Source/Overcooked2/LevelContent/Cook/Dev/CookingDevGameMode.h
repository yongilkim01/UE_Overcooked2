// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include <Global/OC2Enum.h>
#include "CookingDevGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ACookingDevGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ACookingDevGameMode();

	UFUNCTION(BlueprintCallable)
	class AIngredient* SpawnIngredient(EIngredientType Type);

	UFUNCTION(BlueprintCallable)
	class UCookingDevUserWidget* GetWidget()
	{
		return Widget;
	}

	void SetWidget(class UCookingDevUserWidget* UserWidget)
	{
		Widget = UserWidget;
	}

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;	

private:
	class UCookingDevUserWidget* Widget = nullptr;
	
};
