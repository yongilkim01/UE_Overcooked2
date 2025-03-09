// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <LevelContent/Cook/Cooking.h>
#include "Ingredient.generated.h"

// 요리 재료
UCLASS()
class OVERCOOKED2_API AIngredient : public ACooking
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIngredient();

	// Network


	// 상자에서 꺼냈을 때
	UFUNCTION(BlueprintCallable)
	AIngredient* Init(EIngredientType Type);
	
	// 조리 완료 시
	UFUNCTION(BlueprintCallable)
	AIngredient* ChangeState(EIngredientState State);

	// 도마에서 썰어야 하는 재료야?
	UFUNCTION(BlueprintCallable)
	bool IsChoppable() const
	{
		return CurIngredientState == EIngredientState::EIS_CHOPPABLE;
	}

	// 프라이팬에 구워야 하는 재료야?
	UFUNCTION(BlueprintCallable)
	bool IsGrillable() const
	{
		return CurIngredientState == EIngredientState::EIS_GRILLABLE;
	}

	// 솥에서 밥을 지을 수 있어?
	UFUNCTION(BlueprintCallable)
	bool IsBoilable() const
	{
		return CurIngredientState == EIngredientState::EIS_BOILABLE;
	}

	UFUNCTION(BlueprintCallable)
	EIngredientType GetIngredientType() const 
	{
		return IngredientType;
	}

	UFUNCTION(BlueprintCallable)
	EIngredientState GetCurIngredientState() const
	{
		return CurIngredientState;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const FIngredientCookDataRow& CheckState(EIngredientState State);

	void Offset(FVector Pos, FRotator Rot);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	void DeactivateHighlight();
	void ActivateHighlight();


	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EIngredientType IngredientType = EIngredientType::EIT_NONE;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EIngredientState CurIngredientState = EIngredientState::EIS_NONE;


};
