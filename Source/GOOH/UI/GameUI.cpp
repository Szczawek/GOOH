#include "GameUI.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"

void UGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	Controller = GetOwningPlayer();
	if (HealthBar) {
		HealthBar->SetPercent(1.f);
	}
	if (StaminaBar) {
		StaminaBar->SetPercent(1.f);
	}
	if (WidgetSwitcher) {
		WidgetSwitcher->SetVisibility(ESlateVisibility::Collapsed);
		if (MenuWidgetClass) {
			TObjectPtr<UUserWidget> MenuWidget = CreateWidget<UUserWidget>(GetWorld(),MenuWidgetClass);
			WidgetSwitcher->AddChild(MenuWidget);
		}
		if (GameOverClass) {
			TObjectPtr<UUserWidget> GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverClass);
			WidgetSwitcher->AddChild(GameOverWidget);
		}
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

void UGameUI::SetWidgetByIndex(uint8 Index) const
{
	if (!bIsMenuModeEnable) {
		SetMenuMode();
		WidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
	}
	WidgetSwitcher->SetActiveWidgetIndex(Index);
}

void UGameUI::BackToGameWidget() const
{
	RemoveMenuMode();
	WidgetSwitcher->SetVisibility(ESlateVisibility::Collapsed);
}

