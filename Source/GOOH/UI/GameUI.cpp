#include "GameUI.h"

UGameUI::UGameUI(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{

}

void UGameUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (UserName) {
		UserName->SetText(FText::FromString(TEXT("Szczawik")));
	}
	if (HealthBar) {
		HealthBar->SetPercent(1.f);
	}
	if (StaminaBar) {
		StaminaBar->SetPercent(1.f);
	}

	if (WidgetSwitcher) {
		if (MenuWidgetClass) {
			UUserWidget* MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
			WidgetSwitcher->AddChild(MenuWidget);
		}
		WidgetSwitcher->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGameUI::SetStaminaBar(float Value) const
{
	StaminaBar->SetPercent(Value);
}

void UGameUI::SetHealthBar(float Value) const
{
	HealthBar->SetPercent(Value);
}

void UGameUI::SetWidgetOnDisplay(int8 Index) const
{
	WidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
	WidgetSwitcher->SetActiveWidgetIndex(Index);
}

void UGameUI::TakeWidgetFromDisplay() const
{
	WidgetSwitcher->SetVisibility(ESlateVisibility::Collapsed);
}

