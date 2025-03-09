// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <LevelContent/Cook/Cooking.h>
#include <Global/OC2Enum.h>

#include "Plate.generated.h"

UENUM(BlueprintType)
enum class EPlateState : uint8
{
	NONE,
	EMPTY,
	OCCUPIED,
	COMPLETED,
	DIRTY,
};

// 접시 ~ 접시에 올라간 조리된 요리 ~ 요리들의 조합 ~ 완성된 요리
UCLASS()
class OVERCOOKED2_API APlate : public ACooking
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlate();

	// 접시 위에 재료를 쌓는 모든 과정
	UFUNCTION(BlueprintCallable)
	bool Add(class AIngredient* Ingredient);

	UFUNCTION(BlueprintCallable)
	bool IsDirtyPlate();


	// 설거지 성공 시 호출
	UFUNCTION(BlueprintCallable)
	void WashPlate();

	UFUNCTION(BlueprintCallable)
	void SetPlateState(EPlateState State);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LoadDataTable(class AIngredient* Ingredient);

	void CheckAndChangeState(class AIngredient* Ingredient);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	TArray<FIngredientCookDataRow> CookingDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	TArray<class AIngredient*> Ingredients;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* IngredientMesh = nullptr; // 재료

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EPlateState PlateState = EPlateState::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking", meta = (AllowPrivateAccess = "true"))
	EIngredientState PrveState = EIngredientState::EIS_NONE;

};
