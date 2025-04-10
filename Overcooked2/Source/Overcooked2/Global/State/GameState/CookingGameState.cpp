// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/State/GameState/CookingGameState.h"
#include "Overcooked2.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "LevelContent/Cook/Plate.h"
#include "LevelContent/Cook/Cooking.h"

#include "UI/Cooking/CookingHUD.h"
#include "UI/Cooking/UI/CookingWidget.h"

#include "Global/GameMode/CookingGameMode.h"
#include "Global/Data/OC2GlobalData.h"
#include "Global/OC2GameInstance.h"
#include "Global/Manager/StageManager.h"

ACookingGameState::ACookingGameState()
{
}

void ACookingGameState::BeginPlay()
{
	Super::BeginPlay();

	OrderNumberArray.Add(1);
	OrderNumberArray.Add(0);
	OrderNumberArray.Add(0);
	OrderNumberArray.Add(1);
	OrderNumberArray.Add(0);
	OrderNumberArray.Add(0);
	OrderNumberArray.Add(1);
	OrderNumberArray.Add(1);
	OrderNumberArray.Add(0);

	ChangeState(ECookingStageState::ESS_WAITING_TO_START);

}

void ACookingGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurStatgeState)
	{
	case ECookingStageState::ESS_WAITING_TO_START:
		WaitingToStart(DeltaTime);
		break;
	case ECookingStageState::ESS_IN_PROGRESS:
		InProgress(DeltaTime);
		break;
	case ECookingStageState::ESS_WAITING_POST_MATCH:
		WaitingPostMatch(DeltaTime);
		break;
	}

}

void ACookingGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACookingGameState, CurStatgeState);
}

void ACookingGameState::ChangeState(ECookingStageState ChangeState)
{
	if (true == HasAuthority())
	{
		CurStatgeState = ChangeState;
		switch (CurStatgeState)
		{
		case ECookingStageState::ESS_WAITING_TO_START:
			EntryWaitingToStart();
			break;
		case ECookingStageState::ESS_IN_PROGRESS:
			EntryInProgress();
			break;
		case ECookingStageState::ESS_WAITING_POST_MATCH:
			EntryWaitingPostMatch();
			break;
		}
	}
}

void ACookingGameState::EntryWaitingToStart()
{
	SetMatchState(MatchState::WaitingToStart);
}

void ACookingGameState::WaitingToStart(float DeltaTime)
{
}

void ACookingGameState::EntryInProgress()
{
}

void ACookingGameState::InProgress(float DeltaTime)
{
	
}

void ACookingGameState::EntryWaitingPostMatch()
{
}

void ACookingGameState::AddPlate(APlate* Plate)
{
	PlateArray.Add(Plate);
}

APlate* ACookingGameState::GetPlate(int Index)
{
	if (PlateArray.Num() > 0)
	{
		APlate* LastPlate = PlateArray.Last();
		PlateArray.Pop();

		return LastPlate;
	}
	else
	{
		UE_LOG(OVERCOOKED_LOG, Display, TEXT("PlateArray is empty!"));
		return nullptr;
	}
}

void ACookingGameState::WaitingPostMatch(float DeltaTime)
{
}


void ACookingGameState::Multicast_CompleteOrder_Implementation(int OrderIndex, int InScore)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			if (-1 != OrderIndex)
			{
				CookingHUD->CookWidget->OrderComplete(OrderIndex, InScore);
			}
		}
	}
}

void ACookingGameState::Multicast_StartGame_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			CookingHUD->CookWidget->StartGame();
		}
	}
}

void ACookingGameState::Multicast_CreateNewOrder_Implementation(FOrder Order)
{
	// 모든 클라이언트에서 실행됨
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			FOrder CurOrder = Order;
			OrderList.Push(Order);
			CookingHUD->CookWidget->CreateNewOrder(Order);
		}
	}
}

void ACookingGameState::Multicast_BlinkOrderUI_Implementation()
{
	// 모든 클라이언트에서 실행됨
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			CookingHUD->CookWidget->WrongOrder();
		}
	}
}

void ACookingGameState::Multicast_SettingTimer_Implementation(float DeltaTime)
{
	// 모든 클라이언트에서 실행됨
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			CookingHUD->CookWidget->StartTimerTick(DeltaTime);
		}
	}
}

void ACookingGameState::Server_SubmitPlate_Implementation(ACooking* Cooking)
{
	if (true == HasAuthority())
	{
		APlate* Plate = Cast<APlate>(Cooking);

		if (nullptr == Plate)
		{
			UE_LOG(OVERCOOKED_LOG, Error, TEXT("Plate is nullptr"));
			return;
		}

		UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(GetWorld());

		if (nullptr == GameInstance)
		{
			UE_LOG(OVERCOOKED_LOG, Error, TEXT("GameInstance is nullptr"));
			return;
		}

		TArray<FRecipe> Recipes;

		for (int i = 0; i < Plate->GetIngredients().Num(); i++)
		{
			FRecipe Recipe;

			Recipe.IngredientState = Plate->GetIngredient(i).IngredientState;
			Recipe.IngredientType = Plate->GetIngredient(i).IngredientType;

			Recipes.Add(Recipe);
		}

		if (true == GameInstance->CheckRecipe(Recipes))
		{
			ACookingGameMode* GameMode = Cast<ACookingGameMode>(UGameplayStatics::GetGameMode(this));

			if (nullptr != GameMode)
			{
				FOrder Order;

				Order = UOC2GlobalData::GetOrderByIngredients(GetWorld(), Plate);

				int InScore = Order.RequireIngredients.Num() * UOC2Const::ScoreValue;

				if (FeverCount == 0)
				{
					InScore += UOC2Const::TipValue;

				}
				else
				{
					InScore += UOC2Const::TipValue * FeverCount;
				}

				int OrderIndex = GameMode->StageManager->CompleteOrder(Order, InScore);

				if (0 == OrderIndex)
				{
					FeverCount++;
				}
				else
				{
					FeverCount = 0;
				}

				Multicast_SetFeverUI(FeverCount);
			}
		}

		Plate->Multicast_SubmitPlate();
	}
}

void ACookingGameState::Multicast_SetFeverUI_Implementation(int InFeverCount)
{
	// 모든 클라이언트에서 실행됨
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			if (0 == InFeverCount)
			{
				InFeverCount = 1;
			}
			CookingHUD->CookWidget->CheckFeverTime(InFeverCount);
		}
	}
}

void ACookingGameState::Multicast_AddScore_Implementation(int InScore)
{
	// 모든 클라이언트에서 실행됨
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	int Test = InScore;
	if (nullptr != PlayerController)
	{
		ACookingHUD* CookingHUD = Cast<ACookingHUD>(PlayerController->GetHUD());

		if (nullptr != CookingHUD && nullptr != CookingHUD->CookWidget)
		{
			CookingHUD->CookWidget->WrongOrder();
		}
	}
}

void ACookingGameState::Server_MovePlate_Implementation(ACooking* Cooking)
{
	if (true == HasAuthority())
	{
		APlate* Plate = Cast<APlate>(Cooking);

		if (nullptr == Plate)
		{
			UE_LOG(OVERCOOKED_LOG, Error, TEXT("Plate is nullptr"));
			return;
		}

		UOC2GameInstance* GameInstance = UOC2Global::GetOC2GameInstance(GetWorld());

		if (nullptr == GameInstance)
		{
			UE_LOG(OVERCOOKED_LOG, Error, TEXT("GameInstance is nullptr"));
			return;
		}

		ACookingGameMode* GameMode = Cast<ACookingGameMode>(UGameplayStatics::GetGameMode(this));

		if (nullptr != GameMode)
		{
			Plate->Multicast_MovePlate();
		}
	}
}

void ACookingGameState::OnRep_MatchState()
{
	Super::OnRep_MatchState();


}

int ACookingGameState::FindOrderIndex(FOrder& Order)
{
	for (int i = 0; i < OrderList.Num(); i++)
	{
		if (Order.OrderTexutre == OrderList[i].OrderTexutre)
		{
			return i;
		}
	}

	return -1;
}