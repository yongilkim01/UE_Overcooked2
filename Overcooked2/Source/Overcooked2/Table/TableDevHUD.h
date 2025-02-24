// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TableDevHUD.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ATableDevHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ATableDevHUD();
	~ATableDevHUD();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UTableDevUserWidget> UserWidgetClass = nullptr;

private:
	class UTableDevUserWidget* Widget = nullptr;

};
