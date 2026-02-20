#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameUI.generated.h"

class UButton;
class UWidgetSwitcher;
class UProgressBar;

UCLASS()
class GOOH_API UGameUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> GameOverClass;

private:
	UPROPERTY()
	APlayerController* Controller;

public:
	UFUNCTION()
	void SetStaminaBar(float Value) const;

	UFUNCTION()
	void SetHealthBar(float Value) const;

	UFUNCTION()
	void SetWidgetByIndex(uint8 Index) const;

	UFUNCTION()
	void BackToGameWidget() const;

	UPROPERTY()
	bool bIsMenuModeEnable = false;

	UFUNCTION()
	void SetMenuMode() const {
		if (Controller) {
			Controller->SetInputMode(FInputModeUIOnly());
			Controller->bShowMouseCursor = true;
			Controller->bEnableClickEvents = true;
			Controller->bEnableMouseOverEvents = true;
		}
	};
	UFUNCTION()
	void RemoveMenuMode() const {
		if (Controller) {
			Controller->SetInputMode(FInputModeGameOnly());
			Controller->bShowMouseCursor = false;
			Controller->bEnableClickEvents = false;
			Controller->bEnableMouseOverEvents = false;
		}
	};
};
