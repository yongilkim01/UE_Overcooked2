// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelContent/Cook/Cooking.h"
#include "Pot.generated.h"

UENUM(BlueprintType)
enum class EPotState : uint8
{
	IDLE 		UMETA(DisplayName = "대기"),
	BOIL		UMETA(DisplayName = "조리 중"),
	COOKED		UMETA(DisplayName = "조리 완료"),
	BURNING		UMETA(DisplayName = "타는 중"),
	OVERCOOKED	UMETA(DisplayName = "태움"),

};

/**
 * 
 */
UCLASS()
class OVERCOOKED2_API APot : public ACooking
{
	GENERATED_BODY()
	
public:
	APot();

	// 처음 재료가 들어왔을 때 호출할 함수
	UFUNCTION(BlueprintCallable)
	bool SetBoil(ACooking* Rice);

	// 조리된 밥을 받아오는 함수
	UFUNCTION(BlueprintCallable)
	class AIngredient* GetCookedIngredient();

	// Pot이 현재 테이블 위에 놓여 있냐 아니냐. 매번 Table 쪽에서 캐릭터와 상호작용할 때 넣어줘야 하나?
	UFUNCTION(BlueprintCallable)
	void SetOnTable(bool Value)
	{
		bIsOnStove = Value;
	}

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Cook(float DeltaTime);

	void ChangeAnimation();
	void ChangeMaterialColor();

	bool CanCook();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	USkeletalMeshComponent* SoupSkeletalMeshComponent = nullptr;

	class AIngredient* Ingredient = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking", meta = (AllowprivateAccess = "true"))
	EPotState PotState = EPotState::IDLE;

	float TimeElapsed = 0.0f;

	bool bIsOnStove = false;

};
