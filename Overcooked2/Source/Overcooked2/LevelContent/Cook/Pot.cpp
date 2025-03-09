// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelContent/Cook/Pot.h"
#include <LevelContent/Cook/Ingredient.h>

APot::APot()
{
	SoupSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SoupSkeletalMeshComponent"));
	SoupSkeletalMeshComponent->SetupAttachment(StaticMeshComponent);
}

void APot::BeginPlay()
{
	ACooking::BeginPlay();
}

void APot::Tick(float DeltaTime)
{
	ACooking::Tick(DeltaTime);
	Cook(DeltaTime);
	ChangeAnimation();
	ChangeMaterialColor();
}

void APot::Cook(float DeltaTime)
{
	TimeElapsed += DeltaTime;
}

void APot::ChangeAnimation()
{
	if (nullptr == Ingredient)
	{
		return;
	}

	switch (PotState)
	{
	case EPotState::IDLE:
		break;
	case EPotState::BOIL:
		break;
	case EPotState::COOKED:
		break;
	case EPotState::BURNING:
		break;
	case EPotState::OVERCOOKED:
		break;
	default:
		break;
	}
}

void APot::ChangeMaterialColor()
{
}

// Pot은 조리를 할 모든 요건이 갖춰졌는가?
bool APot::CanCook()
{
	if (false == bIsOnStove) // 1. 가스레인지 위에 있냐
	{
		return false;
	}

	if (nullptr == Ingredient) // 2. 재료가 있냐
	{
		return false;
	}

	return true;
}

void APot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
}

bool APot::SetBoil(ACooking* Rice)
{
	if (false == bIsOnStove) // 1. 가스레인지 위에 있냐
	{
		return false; // 가장 먼저 Ingredient를 넣는 함수이므로 CanCook 함수로 체크하지 않는다.
	}

	AIngredient* RawRice = Cast<AIngredient>(Rice);
	if (nullptr == RawRice) // 2. 재료가 쌀이냐
	{
		return false;
	}
	if (EIngredientState::EIS_BOILABLE != RawRice->GetCurIngredientState()) // 3. Boil 할 수 있고, 조리가 안 된 상태가 맞냐
	{
		return false;
	}

	// 예외처리를 통과할 수 있는 재료는 쌀 뿐이므로 여기까지 오면 조리가 되지 않은 쌀이다.

	Ingredient = RawRice;
	PotState = EPotState::BOIL;


	// 스켈레탈 메시에 조리 애니메이션 재생해야함

	return true;
}

AIngredient* APot::GetCookedIngredient()
{
	Ingredient->ChangeState(EIngredientState::EIS_BOILED);

	AIngredient* CookedIngredient = Ingredient;
	Ingredient = nullptr;

	return CookedIngredient;
}

