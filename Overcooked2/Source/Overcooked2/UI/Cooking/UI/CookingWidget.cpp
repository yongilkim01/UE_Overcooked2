// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Cooking/UI/CookingWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Global/Data/OrderDataTable.h"



void UCookingWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Orders.SetNum(5);


    Orders[0] = Order_0;
    Orders[1] = Order_1;
    Orders[2] = Order_2;
    Orders[3] = Order_3;
    Orders[4] = Order_4;


    // Test Setting
    CurOrderCount = 2;

    float StartPos = 0.0f;

    for (int i = 0; i < Orders.Num(); i++)
    {
        Orders[i]->SetRenderTranslation({ StartPos + ImageOffset * (i + 1), 0.0f });

        StartPos += ImageSize;
    }

    for (int i = CurOrderCount; i < Orders.Num(); i++)
    {
        Orders[i]->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UCookingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    InvalidateLayoutAndVolatility();

}


void UCookingWidget::OrderComplete()
{

    if (Orders[CompleteOrderNum] == nullptr) return;

    if (GetWorld()->GetTimerManager().IsTimerActive(OpacityTimerHandle)) return;

    if (Orders[CompleteOrderNum]->GetVisibility() != ESlateVisibility::Collapsed)
    {
        CurOrderCount -= 1;

        TArray<UWidget*> Children;
        Children = Orders[CompleteOrderNum]->GetAllChildren();

        FString TargetPrefix = TEXT("IngredientPanel_");

        for (UWidget* Child : Children)
        {
            if (UCanvasPanel* Panel = Cast<UCanvasPanel>(Child))
            {
                FString PanelName = Panel->GetName();
                if (PanelName.StartsWith(TargetPrefix))
                {
                    Panel->SetRenderTranslation({ 0.0f, -IngredientArrivePos });

                }
            }
        }

        GetWorld()->GetTimerManager().SetTimer(OpacityTimerHandle, this, &UCookingWidget::UpdateImageOpacity, 0.01f, true);
    }

}

void UCookingWidget::CreateNewOrder(FOrder& order)
{
    if (CurOrderCount >= Orders.Num()) return; 

    if (GetWorld()->GetTimerManager().IsTimerActive(MoveTimerHandle)) return;

    NewOrderNum = CurOrderCount;

    FVector2D ScreenSize = UWidgetLayoutLibrary::GetViewportSize(this);

    Orders[NewOrderNum]->SetVisibility(ESlateVisibility::Visible);
    Orders[NewOrderNum]->SetRenderTransformAngle(-20.0f);
    Orders[NewOrderNum]->SetRenderOpacity(1.0f);
    Orders[NewOrderNum]->SetRenderTranslation({ ScreenSize.X * 1.5f, 0.0f });
    Orders[NewOrderNum]->SetRenderScale({ 1.0f, 1.2f });

    CurOrderCount += 1;


    {
        FString texturepath = TEXT("/Game/Resources/UI/Order/Dish/ui_cheeseburger.ui_cheeseburger");
        class UTexture2D* texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *texturepath));

        UImage* dishimage = FindChildImage("Dish_", Orders[NewOrderNum]);

        if (dishimage == nullptr) return;
        
        if (order.OrderTexutre != nullptr)
        {
            dishimage->SetBrushFromTexture(order.OrderTexutre);
        }
        else
        {
            dishimage->SetBrushFromTexture(texture);
        }
                
            
    }

    SettingIngredientImages(order);

    GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &UCookingWidget::MoveNewOrder, 0.01f, true);
}



void UCookingWidget::MoveNewOrder()
{


    if (Orders[NewOrderNum] == nullptr) return;

    FVector2D curpos = Orders[NewOrderNum]->GetRenderTransform().Translation;

    float curangle = Orders[NewOrderNum]->GetRenderTransform().Angle;

    if (NewOrderNum != 0)
    {
        ArrivePos = ImageOffset;
        for (int i = 0; i < CurOrderCount - 1; i++)
        {
            UImage* img = FindChildImage("OrderBackground_", Orders[i]);

            float scale = img->GetRenderTransform().Scale.X;
            ArrivePos += ImageSize * scale + ImageOffset;
        }

    }
    else if (NewOrderNum == 0)
    {
        ArrivePos = ImageOffset;
    }

    if (curpos.X <= ArrivePos && curangle >= 0.0f)
    {
        Orders[NewOrderNum]->SetRenderScale({ 1.0f, 1.0f });
        Orders[NewOrderNum]->SetRenderTranslation({ ArrivePos,0.0f });
        Orders[NewOrderNum]->SetRenderTransformAngle({ 0.0f });

        MoveTimeElapsed = 0.0f;
        IngredientTimeElapsed = 0.0f;

        if (GetWorld()->GetTimerManager().IsTimerActive(IngredientTimerHandle)) return;

        GetWorld()->GetTimerManager().ClearTimer(IngredientTimerHandle);
        GetWorld()->GetTimerManager().SetTimer(IngredientTimerHandle, this, &UCookingWidget::UpdateIngredientImagePosition, 0.01f, true);



        return;
    }
    else
    {
        if (curpos.X > ArrivePos)
        {
            Orders[NewOrderNum]->SetRenderTranslation(curpos - FVector2D(TargetOffset.X + MoveTimeElapsed, 0));

        }
        if (curangle < 0.0f)
        {
            Orders[NewOrderNum]->SetRenderTransformAngle(curangle + 0.7f);
        }
    }

    MoveTimeElapsed += 0.1f;
}

void UCookingWidget::SettingIngredientImages(FOrder& order)
{
    if (order.RequireIngredients.IsEmpty())
    {
        //return;
    }


    UCanvasPanel* panel = FindChildPanel("IngredientPanel_", Orders[NewOrderNum]);
    if (panel == nullptr) return;

    UCanvasPanel* ibackpanel = FindChildPanel("IBackImg_", panel);
    if (ibackpanel == nullptr) return;

    UCanvasPanel* imgpanel = FindChildPanel("I_Img_", panel);
    if (imgpanel == nullptr) return;

    int testnum = 3;

    if (NewOrderNum == 1)
    {
        testnum = 2;
    }


    // for (int i = 0; i < order.RequireIngredients.Num(); i++)
    for (int i = 0; i < testnum; i++)
    {
        TArray<UWidget*> backpanelchind = ibackpanel->GetAllChildren();
        UWidget* ibackimg = backpanelchind[i];


        if (ibackimg != nullptr)
        {
            UCanvasPanelSlot* backslot = Cast<UCanvasPanelSlot>(ibackimg->Slot);
            UImage* bimg = FindChildImage("BIngredient" + FString::FromInt(i), ibackpanel);
            UImage* img = FindChildImage("IngredientImg" + FString::FromInt(i), imgpanel);
            img->SetVisibility(ESlateVisibility::Visible);
            bimg->SetVisibility(ESlateVisibility::Visible);

            UImage* dishimg = FindChildImage("Dish_", Orders[NewOrderNum]);
            dishimg->SetRenderTranslation({ 0.0f, 0.0f });

            UImage* backimg = FindChildImage("OrderBackground_", Orders[NewOrderNum]);
            backimg->SetRenderScale({ 1.0f, 1.0f });
            backimg->SetRenderTranslation({ 0.f, 0.0f });


            // if(order.RequireIngredients[i].IngredientState != EIngredientState::EIS_NONE)
            if (i == 1)
            {
                backslot->SetSize(IShortSize);
                
            }
            /*if (order.RequireIngredients[i].IngredientTexture != nullptr)
            {
                img->SetBrushFromTexture(order.RequireIngredients[i].IngredientTexture);
            }*/
            //else
            {
                FString texturepath = TEXT("/Game/Resources/UI/Order/Ingredient/Fish_Icon.Fish_Icon");
                class UTexture2D* texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *texturepath));

                img->SetBrushFromTexture(texture);
            }
        }
    }

    //if (order.RequireIngredients.Num() < 3)
    if (testnum < 3)
    {
        //for (int i = order.RequireIngredients.Num() - 1; i < 3; i++)
        for (int i = testnum; i < 3; i++)
        {
            UImage* img = FindChildImage("IngredientImg" + FString::FromInt(i), imgpanel);
            UImage* bimg = FindChildImage("BIngredient" + FString::FromInt(i), ibackpanel);
            img->SetVisibility(ESlateVisibility::Hidden);
            bimg->SetVisibility(ESlateVisibility::Hidden);
        }

        
        UImage* dishimg = FindChildImage("Dish_" , Orders[NewOrderNum]);
        dishimg->SetRenderTranslation({-30.0f, 0.0f});

        UImage* backimg = FindChildImage("OrderBackground_", Orders[NewOrderNum]);
        backimg->SetRenderScale({ 0.7f, 1.0f });
        backimg->SetRenderTranslation({ -30.f, 0.0f });

    }

}



void UCookingWidget::UpdateIngredientImagePosition()
{
    if (Orders[NewOrderNum] == nullptr) return;
    
    UCanvasPanel* panel = FindChildPanel("IngredientPanel_", Orders[NewOrderNum]);

    if (panel == nullptr) return;

    FVector2D curpos = panel->GetRenderTransform().Translation;

    if (curpos.Y >= IngredientArrivePos)
    {
        panel->SetRenderTranslation({ 0.0f, IngredientArrivePos });

        IngredientTimeElapsed = 0.0f;

        GetWorld()->GetTimerManager().ClearTimer(IngredientTimerHandle);
        GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
        return;

    }
    else
    {
        panel->SetRenderTranslation(curpos + FVector2D(0.0f, IngredientTargetOffset.Y + IngredientTimeElapsed));
    }
            

    IngredientTimeElapsed += 0.1f;

}


void UCookingWidget::UpdateImageOpacity()
{

    if (Orders[CompleteOrderNum]->GetRenderOpacity() <= 0.0f)
    {
        Orders[CompleteOrderNum]->SetVisibility(ESlateVisibility::Collapsed);
        UpdateImagePosition();

        GetWorld()->GetTimerManager().ClearTimer(OpacityTimerHandle);
        return;
    }

    float curcolor = Orders[CompleteOrderNum]->GetRenderOpacity();
    Orders[CompleteOrderNum]->SetRenderOpacity(FMath::Clamp(curcolor - OpacityOffset, 0.0f, 1.0f));

}



void UCookingWidget::UpdateImagePosition()
{

    if (CompleteOrderNum + 1 == Orders.Num())
    {
        return;
    }

    class UCanvasPanel* Panel = Orders[CompleteOrderNum];

    for (int i = CompleteOrderNum; i < CurOrderCount; i++)
    {

        Orders[i] = Orders[i + 1];
        FinalPos = ImageOffset;

        for (int j = 0; j < i; j++)
        {
            UImage* img = FindChildImage("OrderBackground_", Orders[j]);
            float scale = img->GetRenderTransform().Scale.X;

            FinalPos += ImageSize * scale + ImageOffset;
        }

        Orders[i]->SetRenderTranslation({ FinalPos, 0.0f });
    }

    Orders[CurOrderCount] = Panel;

    return;

}

UImage* UCookingWidget::FindChildImage(const FString& name, UCanvasPanel* canvase)
{
    FString TargetPrefix = name;

    TArray<UWidget*> Children;
    Children = canvase->GetAllChildren();

    for (UWidget* Child : Children)
    {
        if (UImage* image = Cast<UImage>(Child))
        {
            FString imgname = image->GetName();
            if (imgname.StartsWith(TargetPrefix))
            {
                return image;
            }
        }
    }

    return nullptr;
}

UCanvasPanel* UCookingWidget::FindChildPanel(const FString& name, UCanvasPanel* canvase)
{
    FString TargetPrefix = name;

    TArray<UWidget*> Children;
    Children = canvase->GetAllChildren();

    for (UWidget* Child : Children)
    {
        if (UCanvasPanel* panel = Cast<UCanvasPanel>(Child))
        {
            FString panelname = panel->GetName();
            if (panelname.StartsWith(TargetPrefix))
            {
                return panel;
            }
        }
    }

    return nullptr;
}