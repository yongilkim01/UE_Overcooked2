// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "TableDevPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ATableDevPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATableDevPlayerController();
	~ATableDevPlayerController();

	UFUNCTION(BlueprintCallable)
	void AddMappingContext(UInputMappingContext* MappingContext);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Table")
	UInputMappingContext* InputMappingContext = nullptr;

private:
	void Move(const FInputActionValue& Value);
};

