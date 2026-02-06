#include "GameUI.h"

void UGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	Controller = GetOwningPlayer();
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
		if (GameOverClass) {
			UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverClass);
			WidgetSwitcher->AddChild(GameOverWidget);
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

void UGameUI::SetWidgetOnDisplay(uint8 Index, bool bChangeMenuMode) const
{
	if (bChangeMenuMode) {
		SetMenuMode();
		WidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
	}
	WidgetSwitcher->SetActiveWidgetIndex(Index);
}

void UGameUI::TakeWidgetFromDisplay() const
{
	RemoveMenuMode();
	WidgetSwitcher->SetVisibility(ESlateVisibility::Collapsed);
}

