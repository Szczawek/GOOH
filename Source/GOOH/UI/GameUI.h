
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include <Components/CanvasPanel.h>
#include <Components/WidgetSwitcher.h>
#include <Components/ProgressBar.h>
#include <Components/Button.h>
#include "GameUI.generated.h"


UCLASS()
class GOOH_API UGameUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> UserName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> StaminaBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widgets")
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
	void SetWidgetOnDisplay(uint8 Index, bool bChangeMenuMode) const;

	UFUNCTION()
	void TakeWidgetFromDisplay() const;

	UFUNCTION()
	void SetMenuMode() const {
		if (Controller) {
			Controller->SetInputMode(FInputModeGameAndUI());
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
