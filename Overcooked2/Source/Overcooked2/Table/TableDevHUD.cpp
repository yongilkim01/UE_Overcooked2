// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/TableDevHUD.h"
#include <Blueprint/UserWidget.h>
#include <Table/UI/TableDevUserWidget.h>

ATableDevHUD::ATableDevHUD()
{
}

ATableDevHUD::~ATableDevHUD()
{
}

void ATableDevHUD::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr != UserWidgetClass)
	{
		Widget = CreateWidget<UTableDevUserWidget>(GetWorld(), UserWidgetClass);

		if (nullptr != Widget)
		{
			Widget->AddToViewport();
		}
	}
}

void ATableDevHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
