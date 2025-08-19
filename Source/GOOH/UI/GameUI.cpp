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
}

void UGameUI::SetStaminaBar(float Value)
{
	StaminaBar->SetPercent(Value);
}

void UGameUI::SetHealthBar(float Value)
{
	HealthBar->SetPercent(Value);
}

