// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/TableDevPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ATableDevPlayerController::ATableDevPlayerController()
{
}

ATableDevPlayerController::~ATableDevPlayerController()
{
}

void ATableDevPlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void ATableDevPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

}

void ATableDevPlayerController::AddMappingContext(UInputMappingContext* MappingContext)
{
	if (nullptr == GetLocalPlayer())
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	InputSystem->ClearAllMappings(); // 모든 매핑 초기화
	InputSystem->AddMappingContext(MappingContext, 0); // 우선순위 1순위
}

void ATableDevPlayerController::Move(const FInputActionValue& Value)
{
}
