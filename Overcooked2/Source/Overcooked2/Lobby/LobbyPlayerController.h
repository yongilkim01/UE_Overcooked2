// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

class ALobbyGameState;

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	
public:
	UFUNCTION(Client, Reliable)
	void Client_SetChefHeadName(const FString& NewChefHeadName);

	UFUNCTION(Client, Reliable)
	void Client_SetUserIndex(int InUserIndex);

	UFUNCTION(NetMulticast, Reliable)
	void UpdateChefTexture(int Index);

private:
};
